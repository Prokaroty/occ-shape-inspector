# occ-shape-inspector

`occ-shape-inspector` is a lightweight C++/OCCT-oriented CAD shape inspection
tool. The first version focuses on project structure, core data types, analyzer
interfaces, reports, and a CLI entry point. Real OCCT loading, topology analysis,
viewer integration, and GUI workflows are intentionally staged for later phases.

## Current Scope

- C++17 CMake project skeleton.
- `osi::core` data structures for documents, issues, locations, and statistics.
- `osi::analyzer` placeholder API that returns a report from a document.
- `osi::report` API for storing statistics and shape issues.
- `osi-inspect` CLI with `--help` and placeholder model inspection output.
- Smoke tests without external test frameworks.

## Build

```sh
cmake --preset default
cmake --build --preset default
ctest --preset default
```

If Open CASCADE is not available, configuration still succeeds and prints a
clear message. The current phase does not require Qt or OCCT to build the core
libraries or CLI.

## CLI

```sh
osi-inspect --help
osi-inspect path/to/model.step
```

The CLI currently creates a placeholder document and report. Real STEP/BREP
loading will be added when the OCCT loader module is implemented.
