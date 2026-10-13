# PixieLib Promoted Work

**Document ID:** GLOBAL-TODO

**Schema version:** 1

**Kind:** roadmap/status

**Lifecycle:** current

**Subject:** formally promoted work, gates, dependencies, and completion criteria

**Surface:** work-scheduler

**Boundary:** global

**Authority role:** scheduler

**Mutation:** authored

**Indexing:** include

**Last promotion decision:** 2026-10-13

## Purpose and admission rule

This file is the only scheduler. It holds work that the owner has formally
promoted, with enough scope to execute and close it. It does not hold ideas
([`docs/proposals/`](docs/proposals/README.md)), direction
([`ROADMAP.md`](ROADMAP.md)), or completion narratives
([`docs/archive/`](docs/archive/README.md)).

An entry whose scope is large, or that decomposes into many small tasks, keeps
one entry here and its breakdown in [`TASKLIST.md`](TASKLIST.md). The rules are
in the [documentation policy](docs/governance/documentation-policy.md#scheduling-todomd-and-tasklistmd).

Unchecked or `promoted` entries are not automatically authorized to start; an
entry may keep an explicit gate after promotion.

## Current promoted work

### `PXL-TODO-002` — Keep the events' Forward links sound across modules

**Status:** in-progress

**Source:** owner decision on 2026-10-13, to apply the session's
recommendations for the events; the defects are
[EVENTS-ISSUE-001 and EVENTS-ISSUE-002](docs/modules/events/ISSUES.md)

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

`PXL-TODO-001` (adopt the documentation policy) completed on 2026-10-12. Its
entry, breakdown, and verification are preserved in the
[archive record](docs/archive/documentation-foundation-2026-10-12.md).
