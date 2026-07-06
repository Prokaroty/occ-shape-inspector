# AGENTS.md

本仓库是一个公开的 C++/OCCT 工程项目，用于构建 CAD Shape 检测工具 `occ-shape-inspector`。

## 项目目标

项目最终目标是实现一个轻量级 CAD 模型检测工具：

* 支持导入 STEP / BREP 等 CAD 模型；
* 支持输出拓扑统计信息；
* 支持检测常见几何/拓扑问题；
* 支持在 GUI 右侧列出问题列表；
* 支持点击问题后在 3D Viewer 中定位和高亮；
* 为后续 `cad-repair-lab` 修复项目提供可复用的检测核心。

本项目不是完整 CAD 系统，也不是商用几何修复内核。第一阶段重点是检测、定位、报告和工程结构。

## 依赖边界

必须严格遵守以下依赖规则：

* `core` 不允许依赖 Qt、GUI、Viewer。
* `io` 可以依赖 OCCT，但不允许依赖 Qt、GUI、Viewer。
* `analyzer` 可以依赖 OCCT 和 `core`，但不允许依赖 Qt、GUI、Viewer。
* `report` 可以依赖 `core` 和 `analyzer`，但不允许依赖 Qt、GUI、Viewer。
* `viewer` 可以依赖 Qt 和 OCCT AIS，但不允许包含检测逻辑。
* `gui` 可以依赖 Qt、viewer、core、io、analyzer、report。
* 检测逻辑必须放在 `analyzer`，不能写进 `MainWindow`、`OccViewerWidget` 或其他 Qt Widget。
* Viewer 只负责显示 Shape、清理显示、高亮问题位置、视图交互，不负责判断模型问题。
* GUI 只负责组织用户操作、显示列表、调用 analyzer、把 issue 传给 viewer。

## 代码规范

* 使用 C++17。
* 使用 CMake 管理工程。
* 类名、函数名、文件名使用英文。
* public API 一旦创建，除非明确要求，不要随意重命名。
* 不要随便添加临时函数污染全局命名空间。
* 不要把无关逻辑塞进已有类。
* 不要引入全局可变状态。
* 不要在构造函数中做重型计算或文件 IO。
* 新增 public class / public method 时添加必要的 Doxygen 注释。
* 注释可以使用中文，但要说明真实工程意图，不要写空泛注释。
* 非平凡逻辑需要尽量补测试。
* 不提交生成文件、二进制文件、私有 CAD 模型、内部日志、本地路径、账号密钥或公司内部信息。

## 模块规划

建议模块如下：

* `osi::core`

  * 基础数据结构：`ShapeIssue`、`ShapeStatistics`、`IssueLocation`、`ShapeDocument`。
* `osi::io`

  * 模型导入：STEP / BREP Loader。
* `osi::analyzer`

  * 拓扑统计、问题检测、风险分类。
* `osi::report`

  * 文本报告、JSON 报告。
* `osi::viewer`

  * OCCT AIS Viewer 封装和问题高亮。
* `app::cli`

  * 命令行检测工具。
* `app::gui`

  * Qt GUI 程序入口。

## CMake 要求

必须提供以下选项：

* `OSI_BUILD_CLI`：默认 `ON`。
* `OSI_BUILD_GUI`：默认 `OFF`。
* `OSI_BUILD_TESTS`：默认 `ON`。
* `OSI_WITH_OCCT`：默认 `ON`。
* `OSI_WITH_QT`：默认跟随 `OSI_BUILD_GUI`。

要求：

* 没有 Qt 时，core / io / analyzer / report / cli 仍应可以构建。
* 没有 OCCT 时，工程至少应能 configure，并给出明确提示；不要让 CMake 报一堆难懂错误。
* GUI 必须是可选目标，不能影响 CLI 和核心库构建。

## 第一阶段优先级

第一阶段不要直接实现完整 GUI，也不要实现修复功能。优先完成：

1. 工程骨架；
2. core 数据结构；
3. OCCT ShapeLoader；
4. 拓扑统计；
5. analyzer 基础问题检测；
6. CLI 输出报告；
7. 文档说明；
8. 测试结构。

## 禁止事项

* 禁止把 analyzer 设计成 Qt Model。
* 禁止让 `ShapeIssue` 直接依赖 `QVariant`、`QString`、`QColor`、`QWidget` 等 Qt 类型。
* 禁止让检测器直接操作 Viewer。
* 禁止在 Viewer 中遍历模型并判断 FreeEdge、SmallFace 等问题。
* 禁止把修复逻辑提前混入 inspector。
* 禁止提交公司内部模型或日志。
