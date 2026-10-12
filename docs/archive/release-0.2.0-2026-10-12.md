# Release 0.2.0

**Document ID:** ARCHIVE-RELEASE-020-20261012

**Schema version:** 1

**Kind:** roadmap/status

**Lifecycle:** historical

**Subject:** the publication of PixieLib 0.2.0 on nuget.org and the tag v0.2.0

**Surface:** archive-record

**Boundary:** global

**Authority role:** evidence

**Mutation:** snapshot

**Indexing:** include

**Archive ID:** PXL-ARCHIVE-008

**Type:** decision

**Origin:** owner decision

**Author:** claude

**Reference date:** 2026-10-12

**Commits:** `a632d38df99e0906a73c651f0e0b4579e3ca38a7`, and with this record (parent `a632d38df99e0906a73c651f0e0b4579e3ca38a7`)

**Reference commit:** `a632d38df99e0906a73c651f0e0b4579e3ca38a7`, the published commit and the target of the tag

## What happened

On 2026-10-12 the owner asked for 0.2.0 to be published. The version had been
raised in `a2c0eff`, the commit that separated the point from the vector and
added the integer length and the segment, because that change breaks the API
of 0.1.0 (8.1, item 6, in
[PXL-ARCHIVE-001](decision-notes-2026-10-12.md)).

Before the run, the README's C++ example was pointed at `v0.2.0` in
`a632d38`, since the README goes into the package. The session then started
`.github/workflows/publish.yml` on `main` at the owner's request, instead of
the owner starting it on GitHub. The run (GitHub Actions run 38282396644)
passed the five CI jobs, pushed `PixieLib.0.2.0.nupkg` and its symbols to
nuget.org, and created the tag `v0.2.0` on `a632d38` with the release
[PixieLib 0.2.0](https://github.com/Sakamoto0110/PixieLib/releases/tag/v0.2.0).

## What changed for consumers

Compared with 0.1.0: the point lost point + point, point × k, point / k, and
-point; point - point gives a vector; a point and a rectangle move by a
vector; a 3x3 matrix transforms a point; the point has `Lerp`; the integer
vec2 has `Length`, `LengthSquared`, `Distance`, and `DistanceSquared`; and
the segment is new. Consumers pinned to 0.1.0, as PixieInspector, are not
affected until they update.

## Verification

- The push log of the run says both packages were created on nuget.org, and the
  tag and the release exist on GitHub.
- Right after the run, nuget.org's index still listed only 0.1.0: a new version
  is indexed some minutes after the push. Not checked here: the package as
  nuget.org shows it once indexed.
