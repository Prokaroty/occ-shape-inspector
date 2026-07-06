# AGENTS.md

This repository is a public C++/OCCT engineering project for CAD shape inspection.

## Project Goal

Build a small desktop and CLI tool for inspecting CAD shape topology, geometry statistics, and repair risks.

The project should eventually provide:

- A simple GUI for opening CAD models.
- A 3D viewer for displaying imported shapes.
- A right-side issue list for detected problems.
- Issue highlighting in the viewer.
- A reusable core library for future repair workflows.

## Engineering Rules

- Use C++17.
- Use CMake as the build system.
- Keep the core library independent from Qt.
- Keep the analyzer independent from GUI and viewer code.
- Do not place detection logic inside Qt widgets.
- Do not introduce global mutable state.
- Do not rename existing public APIs unless explicitly requested.
- Do not add temporary helper functions into unrelated files.
- Prefer explicit ownership and clear module boundaries.
- Add Doxygen comments for public classes and public methods.
- New non-trivial logic should include tests.
- Avoid committing generated files, binaries, private models, local paths, logs, or credentials.

## Dependency Policy

- OCCT is required for shape loading and topology analysis.
- Qt is optional and only required for the GUI.
- The CLI and core analyzer must build without Qt.
- Do not vendor OCCT or Qt into this repository.
- Do not commit private CAD models.

## Code Organization

Expected modules:

- `osi::core`: shared data structures such as ShapeIssue and ShapeStatistics.
- `osi::io`: CAD model loading.
- `osi::analyzer`: topology and geometry issue detection.
- `osi::report`: text or JSON report generation.
- `osi::viewer`: viewer abstraction and issue highlighting.
- `app::cli`: command line entry point.
- `app::gui`: Qt desktop UI.

## First Development Priority

Start with a minimal but buildable project:

1. Create CMake project structure.
2. Add core data structures.
3. Add an OCCT-based shape loader.
4. Add topology statistics.
5. Add a CLI command to inspect a model.
6. Add unit tests for data structures.
7. Keep GUI as an optional later target.
