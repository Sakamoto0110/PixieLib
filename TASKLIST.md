# PixieLib Task List

**Document ID:** GLOBAL-TASKLIST

**Schema version:** 1

**Kind:** roadmap/status

**Lifecycle:** current

**Subject:** task breakdowns of promoted TODO.md entries

**Surface:** task-scheduler

**Boundary:** global

**Authority role:** scheduler

**Mutation:** authored

**Indexing:** include

## Purpose

This file is a child scheduler of [`TODO.md`](TODO.md). Each section breaks
one promoted `TODO.md` entry into tasks, so that large entries, or entries made
of many small tasks, do not pollute `TODO.md`.

- Every section names its parent in `Parent`, and the parent links back here
  in `Breakdown`.
- Tasks decompose the parent's accepted scope and never extend it.
- When the parent completes, its section moves to
  [`docs/archive/`](docs/archive/README.md).

The full rules are in the
[documentation policy](docs/governance/documentation-policy.md#scheduling-todomd-and-tasklistmd),
and the section shape is in the
[template](docs/templates/TASKLIST-SECTION.md).

## Open breakdowns

## `PXL-TODO-001` — Adopt the documentation policy

**Parent:** PXL-TODO-001

- `PXL-TASK-001` — **Status:** done — Archive the six documents written before
  the policy, whole, as PXL-ARCHIVE-001 to 006 (`9601cea`).
- `PXL-TASK-002` — **Status:** done — The policy, schema, templates, archive
  router, the notes portal, and the four proposals (`94a1a83`).
- `PXL-TASK-003` — **Status:** in-progress — The root documents, the
  development guide, the documentation index, and the layout contract in
  English.
- `PXL-TASK-004` — **Status:** open — The module documents of `primitives`.
- `PXL-TASK-005` — **Status:** open — The module documents of `math`.
- `PXL-TASK-006` — **Status:** open — The module documents of `events`.
- `PXL-TASK-007` — **Status:** open — The module documents of `generator`.
- `PXL-TASK-008` — **Status:** open — The self-review of each module, then the
  completion record.
