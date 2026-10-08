#include <stdexcept>
#include <string>

#include <pixie/events/pxEventHandler.hpp>

#include "check.hpp"

namespace {
    std::string log;
    void A() { log += 'a'; }
    void B() { log += 'b'; }
    void C() { log += 'c'; }

    std::string Run(pxEventHandler<>& e) {
        log.clear();
        e.Invoke();
        return log;
    }

    struct Logger {
        char name;
        void Write() { log += name; }
    };

    // Counts the copies alive, to see that every callback the handler held is destroyed.
    struct Tracked {
        static inline int alive = 0;
        char name;
        Tracked(char n) : name(n) { ++alive; }
        Tracked(const Tracked& o) : name(o.name) { ++alive; }
        Tracked(Tracked&& o) noexcept : name(o.name) { ++alive; }
        ~Tracked() { --alive; }
        void operator()() const { log += name; }
    };

    // Runs `action` from its destructor, once armed: a callable whose destruction uses the handler.
    struct OnDestroy {
        bool* armed;
        pxEventHandler<>* handler;
        void operator()() const {}
        ~OnDestroy() {
            if (*armed) {
                *armed = false;
                *handler += &C;
            }
        }
    };
}

static void TestOrderAndIdentity() {
    pxEventHandler<> e;
    PX_CHECK(e.IsEmpty() && Run(e).empty());
    e += &A;
    e += &B;
    e += &A;
    PX_CHECK(e.Count() == 3 && Run(e) == "aba");

    // As C#: the same callback twice runs twice, and -= removes the last one.
    e -= &A;
    PX_CHECK(Run(e) == "ab");
    PX_CHECK(!e.Remove(&C));
    PX_CHECK(e.Remove(&A) && Run(e) == "b");

    // Member functions, with the syntax closest to `e += obj.Method`.
    Logger x{ 'x' }, y{ 'y' };
    e += { &x, &Logger::Write };
    e += { &y, &Logger::Write };
    PX_CHECK(Run(e) == "bxy");
    e -= { &x, &Logger::Write };
    PX_CHECK(Run(e) == "by");

    // A lambda with captures is removed through its pxCallback, not through a new lambda.
    int hits = 0;
    pxEventHandler<>::Callback counter = [&hits] { ++hits; };
    e += counter;
    e -= [&hits] { ++hits; };
    PX_CHECK(e.Count() == 3);
    e -= counter;
    PX_CHECK(e.Count() == 2);

    e.Clear();
    PX_CHECK(e.IsEmpty() && Run(e).empty());
}

static void TestJoinAndCopy() {
    pxEventHandler<> a, b;
    a += &A;
    a += &B;

    // += with a handler copies its callbacks as they are now.
    b += &C;
    b += a;
    a += &C;
    PX_CHECK(Run(b) == "cab" && Run(a) == "abc");
    a += a;
    PX_CHECK(Run(a) == "abcabc");

    // Joining the same handler twice leaves its callbacks alone (the 2023 bug deleted them).
    pxEventHandler<> twice;
    twice += a;
    twice += a;
    PX_CHECK(Run(a) == "abcabc" && Run(twice) == "abcabcabcabc");

    // -= with a handler removes the last run that matches all of it, in order, as C# does.
    pxEventHandler<> ab;
    ab += &A;
    ab += &B;
    a -= ab;
    PX_CHECK(Run(a) == "abcc");
    pxEventHandler<> ac;
    ac += &A;
    ac += &C;
    PX_CHECK(!a.Remove(ac) && Run(a) == "abcc");
    a -= a;
    PX_CHECK(a.IsEmpty());

    // A copy owns its callbacks: it outlives the original and is destroyed on its own.
    {
        auto* original = new pxEventHandler<>();
        *original += Tracked('t');
        *original += [] { log += 'l'; };
        pxEventHandler<> copy = *original;
        pxEventHandler<> assigned;
        assigned += &A;
        assigned = *original;
        PX_CHECK(Tracked::alive == 3);
        delete original;
        PX_CHECK(Run(copy) == "tl" && Run(assigned) == "tl");

        pxEventHandler<> moved = std::move(copy);
        PX_CHECK(copy.IsEmpty() && Run(moved) == "tl");
        assigned = std::move(moved);
        PX_CHECK(moved.IsEmpty() && Run(assigned) == "tl" && Tracked::alive == 1);
    }
    PX_CHECK(Tracked::alive == 0);
}

static void TestReentrancy() {
    // A callback that removes itself does not make the next one be skipped (the 2023 bug).
    {
        pxEventHandler<> e;
        pxEventHandler<>::Callback self;
        self = [&e, &self] { log += 's'; e -= self; };
        e += self;
        e += &A;
        PX_CHECK(Run(e) == "sa" && e.Count() == 1);
        PX_CHECK(Run(e) == "a");
    }

    // One added during an Invoke runs from the next one on, as C# does.
    {
        pxEventHandler<> e;
        e += [&e] { log += 'x'; e += &B; };
        PX_CHECK(Run(e) == "x" && e.Count() == 2);
        PX_CHECK(Run(e) == "xb" && e.Count() == 3);
    }

    // One removed before its turn does not run; Clear stops the rest.
    {
        pxEventHandler<> e;
        e += [&e] { log += 'x'; e -= &B; };
        e += &B;
        e += &C;
        PX_CHECK(Run(e) == "xc" && e.Count() == 2);
        e += [&e] { log += 'k'; e.Clear(); };
        e += &A;
        PX_CHECK(Run(e) == "xck" && e.IsEmpty());
    }

    // An Invoke inside an Invoke.
    {
        pxEventHandler<> e;
        int depth = 0;
        e += [&] { log += 'i'; if (depth++ == 0) e.Invoke(); };
        e += &A;
        PX_CHECK(Run(e) == "iiaa");
    }

    // A callback that destroys the handler: the Invoke stops there.
    {
        auto* e = new pxEventHandler<>();
        *e += &A;
        *e += [e] { log += 'd'; delete e; };
        *e += &B;
        PX_CHECK(Run(*e) == "ad");
    }

    // A callback that throws: the handler stays usable, and a removal made before it still lands.
    {
        pxEventHandler<> e;
        e += [&e] { log += 'r'; e -= &A; };
        e += [] { throw std::runtime_error("callback"); };
        e += &A;
        bool thrown = false;
        try {
            Run(e);
        } catch (const std::runtime_error&) {
            thrown = true;
        }
        PX_CHECK(thrown && log == "r" && e.Count() == 2);
        e.Clear();
        e += &B;
        PX_CHECK(Run(e) == "b");
    }

    // The destructor of a removed callable may use the handler.
    {
        pxEventHandler<> e;
        bool armed = false;
        e += OnDestroy{ &armed, &e };
        e += &A;
        armed = true;
        e.Clear();
        PX_CHECK(!armed && e.Count() == 1 && Run(e) == "c");
    }
}

static void TestArguments() {
    // A value reaches every callback as the caller passed it.
    pxEventHandler<std::string> e;
    std::string seen;
    e += [](std::string s) { s += '!'; };
    e += [&seen](const std::string& s) { seen = s; };
    e.Invoke("hi");
    PX_CHECK(seen == "hi");

    // A reference reaches them as the same object.
    pxEventHandler<int&> refs;
    refs += [](int& v) { ++v; };
    refs += [](int& v) { v *= 10; };
    int value = 1;
    refs(value);
    PX_CHECK(value == 20);
}

void TestEventHandler() {
    TestOrderAndIdentity();
    TestJoinAndCopy();
    TestReentrancy();
    TestArguments();
}
