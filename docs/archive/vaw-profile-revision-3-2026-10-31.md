# VAW Profile Revision 3

**Document ID:** ARCHIVE-VAW-PROFILE-REVISION-3-20261031

**Schema version:** 1

**Kind:** roadmap/status

**Lifecycle:** historical

**Subject:** the owner's adoption of VAW workflow, premises, validation and versioning capabilities for PixieLib, in parity with PixieEngine

**Surface:** archive-record

**Boundary:** global

**Authority role:** evidence

**Mutation:** snapshot

**Indexing:** include

**Archive ID:** PXL-ARCHIVE-012

**Type:** decision

**Origin:** owner decision

**Author:** claude

**Reference date:** 2026-10-31

**Commits:** with this record (parent `dbfbe88f145e35a2bdf4b238093aefc40c985df0`); VAW `01bcf94`, `8ce42fe` on its local branch `codex/pixie-workplace`

**Current state:** the [development guide](../development.md#void-agent-workplace)

## Decision

On 2026-10-31 the three Pixie repositories moved into the VAW checkout's managed
Pixie family. After PixieEngine's profile gained VAW's working capabilities, the
owner asked for the same here ("pode fazer a revisao completa entao para manter
paridade com review 2 da engine"). Revision 2 of `pixielib` refined its routes and
validation list from the repository's real layout and documented checks;
revision 3 adds workflow state, continuity, premises, repository state,
validation, package validation and versioning (`void.versioning/2`), and the
`pixielib-documentation` and `pixielib-release` skills. Campaigns stay frozen. The
product version is owned by `version.json` since `f421ec7`.

## Verification

- `verify-profile-adapters` valid for `pixielib`/3 and `verify-platform` valid.
- The binding resolves `bound` at revision 3 with all 7 routes, from the
  explicit registry; Claude lists the same seven skills as for PixieEngine.
- One workflow cycle under `.local/vaw/records/`: created, started, paused with a
  checkpoint on `version.json`, verified compatible; one added byte made it
  incompatible and the byte-identical restoration compatible again; resumed,
  completed, and a stale expected revision was refused.
- The profile's validation list passed: the C++ build and `pixie_tests` (`all checks passed`; with the Visual Studio generator the binary is `build/tests/Release/`), and the C# solution with its `net10.0` checks (`all checks passed`).
