# Design

`occ-shape-inspector` is split into small modules so detection logic can be reused by
the future `cad-repair-lab` project without pulling in GUI or viewer dependencies.

## Module Boundaries

- `osi::core` owns lightweight data structures such as `ShapeIssue`, `IssueLocation`,
  `ShapeStatistics`, and `ShapeDocument`. It must not depend on Qt, GUI classes,
  viewer classes, or OCCT AIS.
- `osi::io` will own model loading such as STEP and BREP import. It may depend on
  OCCT, but it must not depend on Qt, GUI, or viewer code.
- `osi::analyzer` owns topology statistics and issue detection. It may depend on OCCT
  and `osi::core`, but it must not depend on Qt or viewer code.
- `osi::report` owns report assembly and formatting around `ShapeStatistics` and
  `ShapeIssue`. It must not depend on Qt, GUI, or viewer code.
- `osi::viewer` will own display, selection, camera interaction, and issue highlight
  rendering. It must not decide whether a model has free edges, small faces, invalid
  shapes, or other analyzer-level problems.
- `app::gui` will coordinate user actions, show issue lists, call `osi::analyzer`, and
  pass selected `ShapeIssue`/`IssueLocation` data to the viewer for highlighting.

## Data Flow

1. A model file is loaded into a `ShapeDocument`.
2. `ShapeAnalyzer` analyzes the document with `ShapeAnalyzerOptions`.
3. `ShapeStatisticsCollector` records topology counts and bounding box.
4. `ShapeIssueDetector` produces a vector of `ShapeIssue`.
5. The analyzer returns a `ShapeReport`.
6. CLI output, report exporters, and future GUI panels read `ShapeReport`.
7. The future viewer receives only display/highlight requests derived from
   `ShapeIssue` and `IssueLocation`.

The analyzer does not depend on Qt. The analyzer does not depend on the viewer.
Detection results move through `ShapeIssue` and `ShapeReport`, which keeps the
inspection core reusable outside the desktop GUI. The CLI also does not depend on
Qt; it loads files, calls analyzer/report APIs, and prints text output only.

## OCCT Integration

OCCT dependencies are target-scoped:

- `osi::io` uses OCCT readers to load STEP and BREP files.
- `osi::analyzer` uses OCCT topology traversal and bounding-box APIs to collect
  statistics.
- `osi::core`, `osi::report`, and the CLI public surface do not expose Qt or viewer
  dependencies.

The CLI phase links only OCCT modeling and data-exchange toolkits needed for file
loading, topology traversal, geometric properties, and shape checks. It deliberately
does not link OCCT visualization toolkits such as AIS, V3d, OpenGL, Draw, or viewer
test libraries. GUI/Viewer phases will introduce AIS/V3d/OpenGL dependencies through
separate optional targets.

`ShapeDocument` uses a small type-erased native handle for the loaded CAD shape. The
public core header stores `std::shared_ptr<void>` plus a native type string such as
`TopoDS_Shape`, so core headers do not include OCCT headers. `osi::io` creates the
OCCT `TopoDS_Shape` and attaches it to the document. `osi::analyzer` checks the type
string and casts the handle only inside OCCT-aware implementation files.

This is intentionally simple. It keeps Phase 1 buildable when OCCT is disabled, avoids
leaking OCCT headers through every consumer of `ShapeDocument`, and leaves room for a
future PIMPL or stronger native-shape wrapper if the project needs richer ownership
semantics.

## Issue Locations And Highlighting

`IssueLocation` stores shape path and sub-shape indices without viewer types. Later GUI
code can pass a selected `ShapeIssue` to the viewer, and the viewer can map the stored
location back to a displayed OCCT sub-shape for highlighting. The analyzer still owns
the decision that an issue exists; the viewer only displays and highlights.

The issue list is not a Qt model and is not viewer state. It is plain analyzer output:
`ShapeIssue` carries type, severity, message, and `IssueLocation`. A future GUI should
display these objects directly or adapt them at the boundary, while a future viewer
should use `IssueLocation` only to locate and highlight the relevant sub-shape.

## Phase 0 Constraints

Phase 2 adds a first-pass `ShapeIssueDetector` for invalid shapes, free edges, small
edges, small faces, and degenerated edges. It still does not implement full GUI, AIS
viewer integration, repair logic, or automatic fixes. Repair is outside the current
inspector phase.
