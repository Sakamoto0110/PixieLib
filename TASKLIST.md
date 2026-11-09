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
- When the parent completes, remove its active section. Preserve the roadmap
  outcome, evidence links and important nested decisions through the
  [closure flow](docs/history/README.md); Git retains the full breakdown.

The full rules are in the
[documentation policy](docs/governance/documentation-policy.md#scheduling-todomd-and-tasklistmd),
and the section shape is in the
[template](docs/templates/TASKLIST-SECTION.md).

## Open breakdowns

No open breakdowns at the documentation migration baseline, 2026-11-06.
This entry point is retained for future large TODO items; no decomposition is
required merely to populate the file. Completed outcomes and their original
breakdowns are routed by the [history index](docs/history/README.md).
