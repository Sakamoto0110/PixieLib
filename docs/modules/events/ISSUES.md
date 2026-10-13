# Events Issues

**Document ID:** EVENTS-ISSUES

**Schema version:** 1

**Kind:** roadmap/status

**Lifecycle:** current

**Subject:** the confirmed defects of the events module

**Surface:** issues

**Boundary:** events

**Authority role:** normative

**Mutation:** authored

**Indexing:** include

One record per confirmed defect of the events. A record stays after its fix,
with `Status: fixed`; the contract is the [technical reference](REFERENCE.md).

## `EVENTS-ISSUE-001` — A Forward link is lost in another module

**Status:** fixed

**Severity:** high: a write through a null pointer, a read of freed memory, or
an `Invoke` that never ends

**Affected releases:** 0.1.0 and 0.2.0, which have `Forward` since `1f61f6b`

**Symptom:** a handler linked by `Forward` in one module and moved in another
writes through a null pointer. A copy made in the other module carries the
link, and invoking it after the target dies reads freed memory. A `Forward`
back from the other module is not refused as a loop, and the next `Invoke`
recurses until the stack runs out.

**Trigger:** two modules that each compile the events, such as an executable
and a DLL on Windows, or shared libraries that hide their symbols elsewhere,
with `Forward` called in one and the copy, the move, or the second `Forward` in
the other. A handler tells a link apart by the type tag of the callable it
stores for it, and each module has its own tag per type.

**Evidence:** reproduced on 2026-10-13 at `5895ec2`, with an executable and a
shared library built with `-fvisibility=hidden`: on g++ 13 with the
AddressSanitizer, a `SEGV` in `TakeFrom` on the move and a
`heap-use-after-free` in `Dispatch` on the copy; on Clang 18, without the
sanitizers, the loop accepted, the copy carrying the link, and a crash on the
move.

**Workaround:** call `Forward`, and copy and move the linked handlers, in one
module only.

**Intended fix:** a link is marked in its slot, not by the type of its
callable; done in `448ce89`
([PXL-ARCHIVE-009](../../archive/events-forward-across-modules-2026-10-13.md)).

**Fix release:** the first release after 0.2.0

**Roadmap:** none

## `EVENTS-ISSUE-002` — `-=` with a handler that has a link removes nothing

**Status:** fixed

**Severity:** low: the callbacks stay, and `Remove` returns `false`

**Affected releases:** 0.1.0 and 0.2.0

**Symptom:** after `a += b`, `a -= b` removes nothing when `b` has a `Forward`
link.

**Trigger:** `+=` with a handler does not copy its links, but `-=` with a
handler looks for all of its entries, the links included.

**Evidence:** reproduced on 2026-10-13 at `5895ec2` with g++ 13: `a.Remove(b)`
returns `false`.

**Workaround:** remove the callbacks one by one.

**Intended fix:** `-=` skips links, as `+=` does; done in `448ce89`
([PXL-ARCHIVE-009](../../archive/events-forward-across-modules-2026-10-13.md)).

**Fix release:** the first release after 0.2.0

**Roadmap:** none
