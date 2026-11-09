# Documentation Schemas

**Document ID:** GLOBAL-DOCUMENTATION-SCHEMAS

**Schema version:** 1

**Kind:** reference

**Lifecycle:** current

**Subject:** structural documentation schema and its derivation from PixieEngine and PixieInspector

**Surface:** index

**Boundary:** global

**Authority role:** normative

**Mutation:** authored

**Indexing:** include

[`documentation-schema.json`](documentation-schema.json) is the structural
contract for document metadata, module manifests, the two schedulers, archive
records, stable record IDs, and validation vocabulary. The meaning of that
vocabulary is owned by the
[documentation policy](../governance/documentation-policy.md).

## Derivation

The schema is a fork of PixieEngine `docs/schemas/documentation-schema.json`
(`pixieengine.documentation/1`), observed on 2026-10-12 at PixieEngine
`46ee6aa`, with the .NET additions PixieInspector made to it
(`pixieinspector.documentation/1`, observed the same day at PixieInspector
`25d56df`). Both are partial derivations of NekoLib's, the model Void Agent
Workplace extracted. PixieEngine is the nearest relative for the C++ side and
PixieInspector for the C# side; both carry the owner's latest decisions about
the model.

At the derivation baseline this was documentation-only, without a VAW binding.
The current workspace binding routes shared governance and external project
profiles; PixieLib source, layout/API contracts and evidence remain independently
owned. See [current development guidance](../development.md#void-agent-workplace).

| Area | PixieEngine | PixieInspector | PixieLib | Reason |
|---|---|---|---|---|
| Module boundaries | One language each | One view or the core | A concept in both languages: `primitives`, `math`, `events`, `generator` | One library in two languages with one contract; owner decision, 2026-10-12 |
| Manifest language | `cpp` or `csharp` | `csharp` | `cpp`, `csharp`, or both | Same reason |
| Manifest artifact | Libraries and executables | `managed-assembly`, `executable`, `none` | `header-only`, `static-library`, `managed-assembly`, `analyzer`, `none` | `pixie::pixie` is headers only, `pixie::math` compiles one file, the generator is a Roslyn analyzer |
| Manifest distribution | `shipped-runtime`, `editor-only`, `tooling`, `unshipped` | `package`, `sample`, `unshipped` | `package`, `source`, `unshipped` | C# ships on nuget.org, C++ is taken from a release tag through `FetchContent` |
| `Target frameworks` | Not present | Present | Present, `none` for a C++-only module | The C# side targets `net10.0` and `net481` |
| Public API oracle | A module's public header | The compiled assembly | Both: the public headers and the compiled assembly, with no accepted baseline | Both languages are public |
| Cross-language contract | Not present | Not present | `docs/layout.md`, a global normative reference | The layout and the shared semantics belong to every module; 36 source comments cite the path |
| Portals | Not used | Not used | `docs/notas.md`, pointing to PXL-ARCHIVE-001 | 18 source comments cite the path of the pre-policy notes |
| Samples | `<sample directory>/README.md` | `hosts/<host>/README.md` | None; the tests and CI are the evidence | No sample exists |
| Working agreements | Commit rules inside the policy | The policy plus `docs/development.md` | The policy plus `docs/development.md` | As PixieInspector |
| Commit prefixes | `(feat)`, `(fix)`, `(refactor)`, `(docs)` | Adds `(testing)` and `(chore)` | `(feat)`, `(fix)`, `(refactor)`, `(docs)` | This repository's practice |
| Branch | Nothing pushed without an owner decision | `rework-claude`, `main` by owner decision | `main`; a session limited to its own branch fast-forwards `main` and deletes the branch | This repository's practice since 2026-10-08 |
| Release | Not covered | Not covered | The `Version` of the package and the publish workflow | PixieLib is published |
| Validation vocabulary | Includes `rendering`, `gpu`, `ipc` | Without `ipc` | Adds `cross-language-parity`, `sanitizer`, and `package`; no UI or GPU terms | The evidence here is bits compared across languages, sanitizers, and the package |
| Imported records | Archived the same way | Body in its language, English header | The same as PixieInspector | The pre-policy documents are in Portuguese and cited from source |
| Stable IDs | `PIXIE-` | `PXI-` | `PXL-` | Owner decision, 2026-10-12 |

The remaining derivation reused the metadata vocabulary, archive/router shape,
root `DEPENDENCIES.md`, `.local/` and on-demand module surfaces. The current
planning and closure model is described below and owned by the local policy;
changes in another repository do not automatically change this contract.

See the policy enforcement section for the current structural checks.

## Workspace contract adoption — 2026-11-06

The table above records the historical derivation. Current planning uses ROADMAP
for rationale, phases and acceptance criteria; TODO for accepted work; optional
TASKLIST for decomposition. New closures preserve roadmap intent and outcome,
link evidence and important nested decisions, and record the Git planning
baseline. Earlier archive records remain unchanged; completed TODO/task lists
are not copied into new closures. The project policy owns this adoption.
