# Release 0.2.1

**Document ID:** ARCHIVE-RELEASE-021-20261027

**Schema version:** 1

**Kind:** roadmap/status

**Lifecycle:** historical

**Subject:** publication and public package verification of PixieLib 0.2.1

**Surface:** archive-record

**Boundary:** global

**Authority role:** evidence

**Mutation:** snapshot

**Indexing:** include

**Archive ID:** PXL-ARCHIVE-010

**Type:** completion

**Origin:** PXL-TODO-003

**Author:** codex

**Reference date:** 2026-10-27

**Commits:** `d79f4dc4ded1a4628cebd099449158f837c4c2f2`, `cd2dcc0564244aa6c0a6fc3520fb7d16df1a3324`, `37589642260663d314b95f1222e8007963e2199d`, and with this record (parent `37589642260663d314b95f1222e8007963e2199d`)

**Reference commit:** `37589642260663d314b95f1222e8007963e2199d`, the published revision and tag target

## Decision and scope

On 2026-10-27 the owner authorized publication. PXL-TODO-003 promoted a
bounded patch release: publish the already merged native events fix
`448ce891c26c0f2fc9546450f3c01c7215612056`, raise the package version to
0.2.1, and point the packaged README's FetchContent example at `v0.2.1`.
The fix and its original evidence remain in
[PXL-ARCHIVE-009](events-forward-across-modules-2026-10-13.md).

No managed API or implementation changed: the only change under `dotnet/`
relative to v0.2.0 is the package Version. No consumer integration, vendoring,
or dependency upgrade was included. Consumers update separately.

## Preparation and publication

All VAW routes passed with status `bound`, scope `all`, profile `pixielib/1`,
before preparation and again after the changes. The checkout was clean at
`4fead247e51f0de9969ea9f7cb236fb0b35e29b7`; remote main matched it. Neither
NuGet 0.2.1 nor GitHub v0.2.1 existed. The configured author and committer
were the owner's identity from the documentation policy.

The policy requires work on main. GitHub reported main unprotected and no
rulesets or open PRs, so preparation went directly to main with separate
configuration and documentation commits; no PR or merge was needed.

- `d79f4dc` promoted PXL-TODO-003 with its scope, gates, and completion criteria.
- `cd2dcc0` raised Version to 0.2.1.
- `3758964` moved the README FetchContent example to the future tag v0.2.1.

[Preparation CI](https://github.com/Sakamoto0110/PixieLib/actions/runs/40547888568)
passed all five jobs on `3758964`: GCC with ASan/UBSan, Clang, MSVC, and C# on
Linux and Windows, including .NET Framework 4.8.1 on Windows and package builds.
The existing
[Publish workflow](https://github.com/Sakamoto0110/PixieLib/actions/runs/40548121417)
was then dispatched on main at that exact revision. Its five CI jobs and its
publish job passed. Trusted publishing pushed the package and symbols; the
workflow created the tag and release. No tag was created manually and no
existing package or tag was replaced.

## Public verification

- [GitHub release v0.2.1](https://github.com/Sakamoto0110/PixieLib/releases/tag/v0.2.1)
  exists, with package and symbols assets. Both its target and remote
  `refs/tags/v0.2.1` are `37589642260663d314b95f1222e8007963e2199d`.
- The public
  [NuGet v2 package download](https://www.nuget.org/api/v2/package/PixieLib/0.2.1)
  returned HTTP 200. Its nuspec identifies version 0.2.1 and repository commit
  `37589642260663d314b95f1222e8007963e2199d`. It contains the net10.0 and
  net481 assemblies and the README pointing at v0.2.1.
- Every ZIP entry of the GitHub nupkg matches the corresponding entry downloaded
  from NuGet by SHA-256. NuGet adds only `.signature.p7s`.
  `dotnet nuget verify --all` on the public package exited 0 and identified
  the NuGet.org repository signature.
- GitHub nupkg SHA-256:
  `935249aa35a5fc47fdc4ed18a84250c1e90000e02ce982a13f59e8d6c8788fa0`.
  NuGet signed nupkg SHA-256:
  `8f346c2a52074c30635bb391cdaacfdd9cff7de8345eae38ece335d9664407f8`.

At this verification snapshot the NuGet v3 flat-container index still listed
only 0.1.0 and 0.2.0, and its direct 0.2.1 URL returned 404, despite the public
v2 download succeeding. V3 indexing and gallery visibility were not confirmed.
Publication and package provenance are verified by the public signed download;
this record does not claim that every NuGet endpoint had converged.

## Completion criteria

PXL-TODO-003 is complete: the existing Publish workflow succeeded, the intended
tag/release exists, and the public NuGet package was downloaded and verified at
the intended revision. No task breakdown was needed. The scope and gates above
preserve the promoted entry; this record preserves the publication evidence.
