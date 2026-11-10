# PixieLib filesystem and authority map

**Document ID:** GLOBAL-FILESYSTEM
**Schema version:** 1
**Kind:** reference
**Lifecycle:** current
**Subject:** PixieLib ownership, parent workspace and focused documentation routes
**Surface:** index
**Boundary:** global
**Authority role:** non-normative
**Mutation:** authored
**Indexing:** include

PixieLib owns its C++/C# primitives, math, events and shared layout contracts.
The [workspace entry](../../.governance/workplaces/void-workplace/README.md)
routes shared coordination; the [workspace policy](../../.governance/workplaces/void-workplace/docs/governance/documentation-policy.md)
is the adopted parent for applicable shared constraints. The
[local policy](docs/governance/documentation-policy.md#authority) names the
technical owners and local procedures. Consuming a peer contract does not
make that peer the parent of PixieLib.

## Where to read and work

| Area | Owner and direct reading route |
|---|---|
| `cpp/include/`, `dotnet/PixieLib/` | Native and managed primitives; [primitives reference](docs/modules/primitives/REFERENCE.md) |
| `cpp/math/`, managed math types | Native and managed math; [math reference](docs/modules/math/REFERENCE.md) |
| `cpp/events/` | Native events; [events reference](docs/modules/events/REFERENCE.md) |
| `dotnet/PixieLib.Generators/` | Managed source generation; [generator reference](docs/modules/generator/REFERENCE.md) |
| `docs/layout.md` | Shared fields, order, sizes and numeric semantics; [layout contract](docs/layout.md) |
| `cpp/CMakeLists.txt`, `dotnet/*.slnx`, project files and `.github/workflows/` | Build, package and delivery wiring; [development guide](docs/development.md), [dependencies](DEPENDENCIES.md) |
| `cpp/tests/`, `dotnet/PixieLib.Tests/` | Bounded native/managed evidence; [validation procedures](docs/development.md) |
| Root planners | [ROADMAP](ROADMAP.md) owns local direction and criteria; [TODO](TODO.md) owns accepted work; optional [TASKLIST](TASKLIST.md) decomposes large items |
| `docs/modules/` | [Module index](docs/README.md#contracts) routes technical references and manifests |
| `docs/history/`, `docs/archive/` | Existing outcomes, original provenance and baseline limits; [history and evidence](docs/README.md#history-and-evidence) |
| `docs/notas.md` | Preserved [decision-notes portal](docs/notas.md) used by source comments |
| `.local/`, build output, `bin/`, `obj/` | Local execution state, retained evidence or generated artifacts; their location grants no deletion permission |

## Authority and coordination

The parent workspace owns shared documentation constraints and cross-project
coordination. PixieLib retains its source, layout/API contracts, build wiring,
product acceptance and local work. A catalog provides discovery and does not
create a second scheduler or transfer technical ownership. Workspace work does
not authorize a new PixieLib feature or qualification task.

The local [commit rules](docs/governance/documentation-policy.md#commits) and
[release procedure](docs/governance/documentation-policy.md#releases) remain
the project specialization, including work on `main` and owner publication.
Documentation integration does not authorize commits, pushes or releases.

## Standalone reading and evidence

Local contracts and source builds remain usable without a workspace checkout.
An unavailable parent or peer route is disclosed as unavailable and not validated;
its absence is not a waiver of an applicable requirement. Full workspace checks
require the declared participating scopes.

Preserve local evidence, logs and recovery material until explicit cleanup
authorization establishes their disposition. Historical records retain their
original IDs, bytes and limitations. Structural documentation checks do not
establish numeric parity, runtime behavior, package delivery or consumer acceptance.
