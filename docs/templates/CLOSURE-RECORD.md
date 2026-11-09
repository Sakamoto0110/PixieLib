# Roadmap Closure Template

**Document ID:** TEMPLATE-CLOSURE-RECORD

**Schema version:** 1

**Kind:** reference

**Lifecycle:** current

**Subject:** template for a completed roadmap outcome and evidence disposition

**Surface:** template

**Boundary:** global

**Authority role:** non-normative

**Mutation:** authored

**Indexing:** exclude

Create one record under `docs/history/<subject>-YYYY-MM-DD.md` and link it
from the history index. Copy the accepted roadmap block, not the complete TODO
and TASKLIST. Retain important nested decisions or link their existing owner.

```text
# <Outcome title>

**Document ID:** HISTORY-<SUBJECT>-YYYYMMDD
**Schema version:** 1
**Kind:** audit
**Lifecycle:** historical
**Subject:** <completed outcome>
**Surface:** audit
**Boundary:** global or a module key
**Authority role:** evidence
**Mutation:** snapshot
**Indexing:** include
**Reference date:** YYYY-MM-DD
**Roadmap source:** original path and phase ID, or explicit legacy limitation
**Planning baseline:** repository and full commit containing the last active plan
**Work IDs:** TODO and relevant child IDs
**Outcome:** delivered result and exact scope
**Evidence:** links to canonical records
**Limitations:** unverified, excluded or deferred scope

## Preserved roadmap

Accepted roadmap block, unchanged. For legacy work without one, state the
limitation and link the original accepted scope; do not invent a roadmap.

## Acceptance disposition

Map each criterion to evidence or an explicit decision. Keep native, managed,
cross-language, package and downstream-consumer claims distinct.

## Important nested decisions

Only details that would otherwise be lost, preferably linked to their owner.
Git retains the full prior TODO and task breakdown.
```

Do not create a new acceptance claim merely because documentation moves.
Existing historical records remain at their original paths and baselines.
