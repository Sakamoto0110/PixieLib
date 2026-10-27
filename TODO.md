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

**Last promotion decision:** 2026-10-27

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

### PXL-TODO-003: Publish 0.2.1

- **Status:** promoted by owner decision on 2026-10-27.
- **Scope:** publish the already merged native events fix from `448ce89`;
  raise the package version to 0.2.1 and update the README FetchContent tag.
  No managed API change, dependency upgrade, or consumer integration.
- **Gates:** verify all VAW routes and release authorities, confirm the version
  and tag are unused, and require green CI on the preparation revision on main.
- **Completion:** run the existing Publish workflow on main, verify its success,
  the public NuGet package and GitHub tag/release at the intended revision,
  and archive the outcome with exact commits and evidence links.

`PXL-TODO-002` (keep the events' Forward links sound across modules) completed
on 2026-10-13. Its entry and verification are preserved in the
[archive record](docs/archive/events-forward-across-modules-2026-10-13.md).

`PXL-TODO-001` (adopt the documentation policy) completed on 2026-10-12. Its
entry, breakdown, and verification are preserved in the
[archive record](docs/archive/documentation-foundation-2026-10-12.md).
