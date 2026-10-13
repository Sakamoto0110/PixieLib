# Events Forward Links Across Modules

**Document ID:** ARCHIVE-EVENTS-FORWARD-ACROSS-MODULES-20261013

**Schema version:** 1

**Kind:** roadmap/status

**Lifecycle:** historical

**Subject:** the Forward links of the events made sound across modules, and -= skipping links, completing PXL-TODO-002

**Surface:** archive-record

**Boundary:** events

**Authority role:** evidence

**Mutation:** snapshot

**Indexing:** include

**Archive ID:** PXL-ARCHIVE-009

**Type:** completion

**Origin:** PXL-TODO-002

**Author:** claude

**Reference date:** 2026-10-13

**Commits:** `ccdac401c9a15a1fa0198e6fb09a8c5c9da3efa9`, `448ce891c26c0f2fc9546450f3c01c7215612056`, and with this record (parent `448ce891c26c0f2fc9546450f3c01c7215612056`)

**Current state:** [`docs/modules/events/REFERENCE.md`](../modules/events/REFERENCE.md), [`docs/modules/events/ISSUES.md`](../modules/events/ISSUES.md)

## What happened

On 2026-10-13 the owner asked whether the session had recommendations for the
events and, if so, to apply them. A review of the events at `5895ec2` found two
defects. Both were reproduced and recorded as EVENTS-ISSUE-001 and
EVENTS-ISSUE-002 when the entry was promoted (`ccdac40`), and fixed in
`448ce89`.

1. **EVENTS-ISSUE-001.** A handler told a `Forward` link apart by the type tag
   of the callable it stored for it, a private `ForwardBinding`. Each module
   (an executable, a DLL, or a shared library that hides its symbols) has its
   own tag per type, so code in another module did not see the link: a move
   there left the link pointing at the old target through a null pointer, a
   copy there carried the link without its subscription, and a loop back from
   there was accepted. Now the slot holds the target itself, and the callback
   of a link is empty; `pxCallback`'s private `Target`, used only for this, is
   gone.
2. **EVENTS-ISSUE-002.** `+=` with a handler did not copy its links, but `-=`
   with a handler looked for them, so `a += b; a -= b` removed nothing when `b`
   had a link. A link is now an entry of the list but not a callback: `-=`
   skips it on both sides, as `+=` and copies already did.

The review considered one more change and kept the code as it was: `Invoke`
still takes its arguments by value. Taking them by `const` reference would save
a copy, but a callback that changes the caller's variable would then change
what the next callbacks see, which 4.3 rules out (section 4.6 of
[PXL-ARCHIVE-001](decision-notes-2026-10-12.md)).

## The entry, as it closed

### `PXL-TODO-002` — Keep the events' Forward links sound across modules

**Status:** completed

**Source:** owner decision on 2026-10-13, to apply the session's
recommendations for the events; the defects are EVENTS-ISSUE-001 and
EVENTS-ISSUE-002

**Accepted scope:** a handler tells a `Forward` link apart by a field of its
slot instead of by the type of a callable, so that a handler linked in one
module (a DLL, or a shared library that hides its symbols) can be copied,
moved, and checked for loops in another; `-=` with a handler skips links, as
`+=` does; a test with a second module, built as a shared library that hides
its symbols; the events reference states what equality holds across modules.
Out of scope: making callbacks built in different modules compare equal.

**Dependencies and gates:** none.

**Completion criteria:** both issues reproduced before the fix and fixed; the
C++ tests pass on g++ with the sanitizers, on Clang, and on MSVC in CI; the new
tests fail on deliberately broken copies, the code before the fix among them;
the events reference, manifest, and issues describe the result; the completion
is archived.

**Breakdown:** none

## Verification

- Before the fix, at `5895ec2`, an executable and a shared library built with
  `-fvisibility=hidden`, the library calling `Forward`: on g++ 13 with the
  AddressSanitizer, a `SEGV` in `TakeFrom` when the executable moved the
  target, and a `heap-use-after-free` in `Dispatch` when it invoked its copy of
  the source after the target died; the loop back was accepted. On Clang 18,
  without the sanitizers, the same loop and copy, and a crash on the move. With
  g++, `a.Remove(b)` returned `false` after `a += b` when `b` had a link.
- After the fix, the C++ tests print `all checks passed` on g++ 13 with the
  address and undefined-behavior sanitizers (in Release, as CI builds, and in
  Debug), with `_GLIBCXX_DEBUG`, with `PIXIE_MATH=OFF`, and on Clang 18 in
  Release. CI run 38357734597 on `448ce89` passed its five jobs; on MSVC the
  second module is a real DLL, `pixie_tests_module.dll`.
- 26 deliberately broken copies, each built with g++ and the sanitizers and
  run, all failed. The first 23 are those of section 4.6 of
  [PXL-ARCHIVE-001](decision-notes-2026-10-12.md), three of the five about links
  rewritten for the new code. The new ones: `-=` finding a link (a check
  fails); `-=` with a handler counting its links (a check fails); and the two
  headers as they were before the fix, which the test across modules stops
  with a null pointer in `TakeFrom`.
- Not checked: the code before the fix on MSVC, where only the fix ran; a
  platform other than Linux and Windows.

## Observations

- Equality between callbacks built in different modules stays per module,
  outside the scope; the reference now says so.
- No version was raised: the C# package does not change. C++ consumers that
  fetch a tag get the fix with the next release, which the owner decides.
