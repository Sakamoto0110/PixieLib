# Documentation Index

**Document ID:** GLOBAL-DOCUMENTATION-INDEX

**Schema version:** 1

**Kind:** reference

**Lifecycle:** current

**Subject:** index of the PixieLib documentation

**Surface:** index

**Boundary:** global

**Authority role:** non-normative

**Mutation:** authored

**Indexing:** include

Choose a route by the question you need to answer. The
[documentation policy](governance/documentation-policy.md#authority) names the
owner of each fact. Contracts define requirements; records describe what was
verified at a specific baseline.

## At the root

| Document | Responsibility |
|---|---|
| [README](../README.md) | C++/C# package entry points and usage examples |
| [Roadmap](../ROADMAP.md) | Product direction, rationale, phase outcomes, criteria and guardrails |
| [TODO](../TODO.md) | Accepted active work, status, dependencies and gates |
| [TASKLIST](../TASKLIST.md) | Optional decomposition of large TODO entries |
| [Dependencies](../DEPENDENCIES.md) | Toolchain, GLM and managed dependencies, status and candidates |

## Contracts

The [layout contract](layout.md) owns shared fields, order, sizes and numeric
semantics across C++ and C#. It is referenced by source comments and remains
separate from both implementation notes and measured evidence.

| Module | Introduction | Reference | Manifest | Languages |
|---|---|---|---|---|
| Primitives | [Overview](modules/primitives/README.md) | [Contract](modules/primitives/REFERENCE.md) | [Routes](modules/primitives/MANIFEST.md) | C++, C# |
| Math | [Overview](modules/math/README.md) | [Contract](modules/math/REFERENCE.md) | [Routes](modules/math/MANIFEST.md) | C++, C# |
| Events | [Overview](modules/events/README.md) | [Contract](modules/events/REFERENCE.md) | [Routes](modules/events/MANIFEST.md) | C++ |
| Source generator | [Overview](modules/generator/README.md) | [Contract](modules/generator/REFERENCE.md) | [Routes](modules/generator/MANIFEST.md) | C# |

Public C++ headers and the compiled managed assembly expose the API; module
references explain their contracts without duplicating the global layout table.
The [0.1.0-to-0.2.0 migration](modules/primitives/migrations/0.1.0-to-0.2.0.md)
records the point/vector compatibility change. The
[events issue register](modules/events/ISSUES.md) retains fixed defects and evidence.

## Working

| Guide | Contents |
|---|---|
| [Development](development.md) | Native/managed builds, numeric comparison, CI and release procedures |
| [Documentation changes](development.md#documentation-changes) | Structural review and history preservation without reopening product tests |
| [Proposals](proposals/README.md) | Unpromoted math and formatting ideas, with their original scope |

## History and evidence

| Route | Contents |
|---|---|
| [Completed outcomes](history/README.md) | Documentation adoption, event fixes, 0.2.1 publication and bounded MSVC qualification |
| [Legacy archive](archive/README.md) | Complete earlier chronology, original evidence and imported Portuguese records |
| [Decision-notes portal](notas.md) | Stable source-comment route to the pre-policy notes and their numbered decisions |

## Structure

| Document | Contents |
|---|---|
| [Policy](governance/documentation-policy.md) | Fact ownership, planning, evidence, closure and commit rules |
| [Schemas](schemas/README.md) | Metadata vocabulary and historical derivation |
| [Phase template](templates/ROADMAP-ITEM.md) | Rationale, outcome, scope and criteria |
| [TODO template](templates/TODO-ENTRY.md) | Work linked to a roadmap phase |
| [Task template](templates/TASKLIST-SECTION.md) | Child decomposition without duplicating the parent |
| [Closure template](templates/CLOSURE-RECORD.md) | Preserved roadmap intent, outcome, evidence and Git baseline |
| [Other templates](templates/) | Module, proposal and legacy record shapes |
