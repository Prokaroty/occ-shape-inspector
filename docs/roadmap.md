# Roadmap

## Phase 0: Project Skeleton

- Establish CMake options and module directories.
- Add core data structures.
- Add analyzer/report interfaces.
- Add placeholder CLI and smoke tests.

## Phase 1: OCCT ShapeLoader

- Add STEP/BREP loading in `osi::io`.
- Keep loader independent from Qt, GUI, and viewer code.
- Preserve configure behavior when OCCT is unavailable.
- Collect first real topology statistics for CLI reports.

## Phase 2: Topology Statistics

- Traverse loaded OCCT shapes.
- Populate compounds, solids, shells, faces, wires, edges, vertices, and bounding box.

## Phase 3: Basic Issue Detection

- Detect invalid shapes, free edges, small edges, small faces, degenerated edges,
  open wires, and non-manifold edges in `osi::analyzer`.
- Current detector work starts with invalid shapes, free edges, small edges, small
  faces, and degenerated edges. Open wires and non-manifold edges remain future work.

## Phase 4: Qt GUI Skeleton

- Add optional Qt targets.
- Keep GUI as orchestration and presentation only.

## Phase 5: OCCT AIS Viewer

- Add viewer module for display, camera interaction, selection, and highlight.
- Keep detection logic out of viewer classes.

## Phase 6: Issue List And Highlight

- Show `ShapeReport` issue lists in the GUI.
- Highlight selected `IssueLocation` entries in the viewer.

## Phase 7: cad-repair-lab Interface Preparation

- Keep inspection results stable enough for repair workflows.
- Reserve extension points for repair recommendations without implementing repair
  operations in this project.
