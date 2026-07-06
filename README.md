# occ-shape-inspector

`occ-shape-inspector` is a lightweight C++/OCCT-oriented CAD shape inspection
tool. The current Phase 1 build focuses on command-line model loading, topology
statistics, core data types, analyzer interfaces, and reports. Viewer integration,
GUI workflows, issue detection, and repair operations are intentionally staged for
later phases.

## Current Scope

- C++17 CMake project skeleton.
- `osi::core` data structures for documents, issues, locations, and statistics.
- `osi::io` OCCT-backed loader for STEP and BREP files when Open CASCADE is found.
- `osi::analyzer` topology statistics collection for loaded OCCT shapes.
- `osi::report` API for storing statistics and shape issues.
- `osi-inspect` CLI with `--help` and real statistics output when OCCT is enabled.
- Smoke tests without external test frameworks.

Current supported file extensions:

- `.step`
- `.stp`
- `.brep`
- `.rle`

## Build

```sh
cmake --preset default
cmake --build --preset default --config Debug
ctest --preset default
```

Important CMake options:

- `OSI_BUILD_CLI`: builds `osi-inspect`, default `ON`.
- `OSI_BUILD_GUI`: reserved for future GUI work, default `OFF`.
- `OSI_BUILD_TESTS`: builds smoke tests, default `ON`.
- `OSI_WITH_OCCT`: enables OCCT-backed loading/statistics when Open CASCADE is found,
  default `ON`.
- `OSI_WITH_QT`: follows `OSI_BUILD_GUI`; Qt is not required for Phase 1.

If Open CASCADE is not available, configuration still succeeds and prints a clear
message. In that case the CLI builds, but loading a supported CAD file reports that
OCCT support is disabled. Use `-DOSI_WITH_OCCT=OFF` or the `no-occt` preset to test
that path explicitly.

## CLI

```sh
osi-inspect --help
osi-inspect path/to/model.step
```

Example output:

```text
File: path/to/model.step
Load status: success

Shape statistics:
  Compounds: 1
  CompSolids: 0
  Solids: 2
  Shells: 2
  Faces: 128
  Wires: 256
  Edges: 384
  Vertices: 192

Bounding box:
  Min: 0.000000, 0.000000, 0.000000
  Max: 10.000000, 20.000000, 30.000000

Issues: 0
```

The current phase does not implement Qt GUI, viewer highlighting, repair logic, or
issue detection. Issue lists are expected to remain empty until the basic analyzer
checks are added in a later phase.
