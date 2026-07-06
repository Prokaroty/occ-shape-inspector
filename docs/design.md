# Design

`occ-shape-inspector` is split into small modules so detection logic can be reused by
the future `cad-repair-lab` project without pulling in GUI or viewer dependencies.

## Module Boundaries

- `osi::core` owns lightweight data structures such as `ShapeIssue`, `IssueLocation`,
  `ShapeStatistics`, and `ShapeDocument`. It must not depend on Qt, GUI classes,
  viewer classes, or OCCT AIS.
- `osi::io` will own model loading such as STEP and BREP import. It may depend on
  OCCT in later phases, but it must not depend on Qt, GUI, or viewer code.
- `osi::analyzer` owns topology statistics and issue detection. It may depend on OCCT
  and `osi::core` in later phases, but it must not depend on Qt or viewer code.
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
3. The analyzer returns a `ShapeReport`.
4. CLI output, report exporters, and future GUI panels read `ShapeReport`.
5. The future viewer receives only display/highlight requests derived from
   `ShapeIssue` and `IssueLocation`.

The analyzer does not depend on Qt. The analyzer does not depend on the viewer.
Detection results move through `ShapeIssue` and `ShapeReport`, which keeps the
inspection core reusable outside the desktop GUI.

## Phase 0 Constraints

Phase 0 does not implement real OCCT loading, full GUI, AIS viewer integration, or
repair logic. `ShapeDocument` intentionally avoids storing `TopoDS_Shape` directly
so the core module remains buildable without OCCT. Later phases can add OCCT-backed
adapters or PIMPL storage without breaking the public core data contract.
