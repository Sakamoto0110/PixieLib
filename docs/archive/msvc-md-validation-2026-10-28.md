# MSVC /MD Validation

**Document ID:** ARCHIVE-MSVC-MD-20261028

**Schema version:** 1

**Kind:** audit

**Lifecycle:** historical

**Subject:** bounded C++ build and test validation with the shared MSVC runtime

**Surface:** archive-record

**Boundary:** global

**Authority role:** evidence

**Mutation:** snapshot

**Indexing:** include

**Archive ID:** PXL-ARCHIVE-011

**Type:** completion

**Origin:** PXL-TODO-004

**Author:** codex

**Reference date:** 2026-10-28

**Commits:** with this record (parent `2f2c9444cfdac8e9fb7ab36c4c51885c5c11327a`)

**Reference commit:** `2f2c9444cfdac8e9fb7ab36c4c51885c5c11327a`

## Authorized scope and gates

The owner requested this bounded validation in support of Engine
PIXIE-PROPOSAL-010, whose coordinating request identified Engine baseline
`f634eee`. PXL-TODO-004 covers validating every VAW route, reading the relevant
authorities, building and testing PixieLib with MSVC /MD, inspecting for /MT
assumptions, and preserving durable evidence. Completion requires the actual
compiler flags and successful tests, rather than configuration alone.
No API change, implementation, consumer edit, package publication, or release
is included. This record preserves the promoted scope and its completed result;
no task breakdown was needed.

## Baseline and authorities

The PixieLib checkout was clean at the reference commit and origin/main matched.
VAW resolution with CompatibilityScope All returned `bound`, profile
`pixielib/1`, checking build, documentation, generator, managed, and native.
The development guide, dependencies, documentation policy, layout contract,
primitives/math/events references, both CMakeLists files, and the existing
cross-module events tests were read for this validation.

Toolchain: Windows x64, Visual Studio 18 2026 Community, MSVC
19.51.36260.0 (installed toolset directory 14.51.36231), CMake 4.3.1-msvc1,
Windows SDK 10.0.26100.0. CMake was invoked from the Visual Studio installation
because it was not on PATH. GLM was fetched at its existing pinned commit
`8d1fd52e5ab5590e2c81768ace50c72bae28f2ed`; no dependency was upgraded.

## Reproduction

From the repository root, with Visual Studio's CMake/CTest binaries on PATH,
use a new build directory; do not reuse an unknown build cache:

```powershell
& C:/Users/Nekovih/dev/VAW/scripts/resolve-project-binding.ps1 `
    -RepositoryPath C:/Users/Nekovih/dev/PixieLib -CompatibilityScope All
cmake -S cpp -B build/msvc-md-validation -G "Visual Studio 18 2026" -A x64 `
    -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL
cmake --build build/msvc-md-validation --config Release --verbose
ctest --test-dir build/msvc-md-validation -C Release -V
cmake --build build/msvc-md-validation --config Debug --verbose
ctest --test-dir build/msvc-md-validation -C Debug -V
```

The execution used an isolated ignored directory. Every command above completed
with exit 0. Both builds reported zero warnings and zero errors. Both CTest
runs passed 1/1 (`pixie_tests`) and printed `all checks passed`; Release took
1.46 seconds and Debug 1.40 seconds in CTest. This is one test executable
containing the primitive, math, and event checks, not one individual assertion.

## Runtime evidence and inspection

| Compiled target | Release | Debug |
|---|---|---|
| pixie_math (static library) | /MD | /MD |
| pixie_tests_module (DLL) | /MD | /MD |
| pixie_tests (EXE) | /MD | /MD |

Both verbose build logs contain three compiler command groups, all with /MD
and none with /MT or /MTd. The generated projects select
`RuntimeLibrary=MultiThreadedDLL`. Debug deliberately uses /MD, not /MDd,
because the explicit CMake setting applies to every configuration.

`dumpbin /directives` on pixie_math.lib reports `/DEFAULTLIB:MSVCRT` in both
configurations. `dumpbin /dependents` on each EXE and DLL reports MSVCP140.dll,
VCRUNTIME140.dll, and VCRUNTIME140_1.dll. The static math target therefore does
not imply a statically linked CRT. Primitives, events, and GLM are header-only
targets and take the runtime of their compiling consumer.

A scoped source search of cpp, .github, README.md, DEPENDENCIES.md, and docs
for /MT, /MTd, /MD, /MDd, MSVC_RUNTIME_LIBRARY, CMP0091, LIBCMT, _DLL, _MT,
NODEFAULTLIB, DEFAULTLIB, and pragma library directives found no matches before
this evidence record was written. Inspection of both CMakeLists files found no
runtime override or forced static CRT setting. Successful /MD builds and the
cross-module events tests support compatibility in this tested configuration;
the negative search alone is not a proof about all possible consumers.

## Evidence fingerprints

SHA-256 of the retained local execution logs (these are fingerprints, not
portable substitutes for the summarized results above):

| Log | SHA-256 |
|---|---|
| configure | `2d87e72075bde2600c44fa8c9672c7a5c764ae2bd0348954f145f7ab079fabc7` |
| Release build | `b058880ac2b1aaa57453d15028db4d0218e97942e50627623733b146c728b0da` |
| Release CTest | `d3808ac9cd6a22bf97631a6164f3f4035aaf0689d832bff72b66271967bc9070` |
| Debug build | `83ae0122532b8c04c6922c94cb044c212d2d8b0948ac4c82b7c0723782f2411e` |
| Debug CTest | `03a1309116d0d7dd3d4a21000f5f2092de7a8d64daf7dd543e6cb4c710464949` |

## Conclusion and limits

PXL-TODO-004 is complete: this PixieLib revision builds and passes its C++
tests with /MD in Release and Debug on the toolchain above. No /MT requirement
was identified in the inspected scope, and no source or build-system change
was needed. A consuming CMake project must set its runtime choice before
creating PixieLib's compiled target and keep its own targets compatible;
PixieLib does not enforce a global consumer policy.

This does not validate Engine's vendored copy, its single-DLL packaging,
export allowlist, allocator ownership across arbitrary module boundaries,
or the development MCP channel and its publication isolation. No Engine,
Inspector, managed test, sanitizer, publication, or remote-release operation
was executed for this request. Consumer integration remains the Engine's
separate acceptance responsibility.
