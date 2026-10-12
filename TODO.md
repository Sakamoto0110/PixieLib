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

**Last promotion decision:** 2026-10-12

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

### `PXL-TODO-001` — Adopt the documentation policy

**Status:** in-progress

**Source:** owner decision on 2026-10-12, accepting the migration map of the
`inventory/design` step (questions 10.1 to 10.9)

**Accepted scope:** the documentation policy, schema, and templates derived from
PixieEngine's, with PixieInspector's .NET additions; the six documents written
before the policy archived whole, in Portuguese, as imported records; the notes
portal; the four ideas the notes left for later as proposals; the root
documents, the development guide, and the layout contract in English; the
module documents (`MANIFEST.md`, `README.md`, `REFERENCE.md`) of `primitives`,
`math`, `events`, and `generator`, written from the current source. Out of
scope: any change to product code or public API, and the promotion of any
proposal.

**Dependencies and gates:** the owner accepts the migration map; met on
2026-10-12.

**Completion criteria:** every document named in the scope exists with valid
metadata and resolving links; every module reference is checked claim by claim
against the source; no product code changes; the completion is archived.

**Breakdown:** [`TASKLIST.md`](TASKLIST.md#pxl-todo-001--adopt-the-documentation-policy)
