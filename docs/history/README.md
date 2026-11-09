# History and evidence

**Document ID:** GLOBAL-HISTORY-INDEX

**Schema version:** 1

**Kind:** reference

**Lifecycle:** current

**Subject:** routes to completed PixieLib outcomes and their original evidence

**Surface:** index

**Boundary:** global

**Authority role:** non-normative

**Mutation:** authored

**Indexing:** include

This index removes completed-work summaries from active planning without
copying their records. The [legacy archive](../archive/README.md) retains the
original bodies, identifiers, dates, task snapshots, commands and commit references.
A record is evidence for its named baseline, not a claim about a later consumer.

## Completed work

| Work | Outcome and source | Recorded boundary |
|---|---|---|
| PXL-TODO-001 | [Documentation foundation](../archive/documentation-foundation-2026-10-12.md) | Policy, four module boundaries and preserved pre-policy documents |
| PXL-TODO-002 | [Event links across modules](../archive/events-forward-across-modules-2026-10-13.md) | Forward-link ownership and handler removal fixes; exact issue and verification scope retained |
| PXL-TODO-003 | [0.2.1 publication](../archive/release-0.2.1-2026-10-27.md) | Recorded workflow, tag, package/signature provenance and appended public-indexing reconciliation |
| PXL-TODO-004 | [MSVC runtime compatibility](../archive/msvc-md-validation-2026-10-28.md) | Release and Debug both tested with `/MD`; this is not `/MDd` or Engine integration qualification |

The [0.2.0 release](../archive/release-0.2.0-2026-10-12.md) and the
[0.1.0-to-0.2.0 migration](../modules/primitives/migrations/0.1.0-to-0.2.0.md)
retain their own compatibility context. The [archive router](../archive/README.md)
is the complete chronology; this index is a route to outcomes and evidence,
not a second chronological register.

## Evidence boundaries

The MSVC record qualifies one PixieLib revision and explicit runtime setting.
It does not qualify Engine's vendored copy, shared-DLL packaging, allocator
ownership across arbitrary module boundaries or private automation. Consumer
adoption remains separate from a library build or release.

The publication record preserves its first snapshot and the later reconciliation
of NuGet indexing. Read both instead of treating the initial indexing delay as
current state. No release endpoint, workflow or package was rechecked during
this documentation change.

[Development procedures](../development.md) describe how to obtain evidence.
Their presence does not record a new execution. The [layout contract](../layout.md),
module references and [events issue register](../modules/events/ISSUES.md)
remain unchanged by this reorganization.

## Future closure

New closures preserve the accepted roadmap block and its outcome in one
Markdown record here, with evidence links, important nested decisions and the
Git planning baseline. Remove completed TODO/task entries instead of copying
both planners into another archive. Use the
[closure template](../templates/CLOSURE-RECORD.md).

Legacy work may have no standalone roadmap block; name that limitation and
reference the original accepted scope rather than inventing one. Existing
archive records and the [decision-notes portal](../notas.md) stay at their paths.

Documentation migration baseline: PixieLib
`12a57318f44e82d57a49c4d4657fcc0597c7eac0`. This routing change introduces no
new product acceptance, publication or consumer upgrade.
