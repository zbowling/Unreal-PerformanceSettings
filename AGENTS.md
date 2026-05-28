# Agent Instructions — Unreal PerformanceSettings

Unreal Engine demo for Meta Quest letting developers interactively explore how render scale, MSAA, Fixed Foveated Rendering, CPU/GPU levels, Application SpaceWarp, and passthrough affect framerate and visual fidelity. Includes a CPU + GPU stress tester.

## Source-of-truth files (read these first, do not duplicate their contents in this file)

For setup, build steps, SDK versions, and project layout, read:

- `README.md` — official setup, per-setting explanations, and links to Meta's docs
- `PerformanceSettings.uproject` — Unreal engine version and plugins
- `Source/PerformanceSettings/` — C++ runtime module and build targets
- `Config/` — engine `.ini` files (including AndroidManifest `meta-data` for Quest 2 dual-core and Quest 3 CPU<->GPU trade)
- `LICENSE` — license terms

## Quest / Horizon-specific notes

- This is a **C++** project (unlike the blueprint-only LayerSample). Right-click `.uproject` to generate the Visual Studio solution, then build via the IDE — opening it directly in the editor without compiling the module first will fail.
- Two important options are **not runtime-tunable** — they are AndroidManifest `meta-data` entries:
  - Quest 2 **dual-core mode**: `com.oculus.dualcorecpuset` = `true`.
  - Quest 3 **CPU<->GPU trade**: `com.oculus.trade_cpu_for_gpu_amount` (e.g. `1`).
  Toggling them in the in-game UI will not take effect; they require a rebuild.
- Dynamic Resolution is also project-settings-only (requires stopping the game to change), not a runtime slider.
- Dynamic Resolution is the recommended default; manual render-scale tuning is included mainly for contrast.

## Meta Quest tooling

This repository is part of the Meta Quest / Horizon OS ecosystem (a sample, library, template, or related project — the bespoke intro above describes which). Use that intro and the source-of-truth files it references for project-specific decisions; don't restate or invent facts from memory.

When the user asks anything about Quest device behavior, build / deploy / debug / capture flows, on-device performance, or Horizon OS APIs, reach for these tools instead of generic Unreal answers:

- **`hzdb`** — Quest-aware ADB wrapper (device list, install / launch / stop, logs, screenshots, Perfetto traces, on-device docs search). Already wired up as an MCP server via `.mcp.json`, `.vscode/mcp.json`, and `.cursor/mcp.json`. Also runnable directly: `npx -y @meta-quest/hzdb <subcommand>`.
- **Meta Quest Agentic Tools** — the full skill set, including Unreal-specific skills: [github.com/meta-quest/agentic-tools](https://github.com/meta-quest/agentic-tools). Install per your client (Claude Code: `/plugin install meta-vr@meta-quest`; Gemini CLI: `gemini extensions install https://github.com/meta-quest/agentic-tools`; Cursor / VS Code: install the **Meta Horizon** extension from the Marketplace).

A few behavior expectations:

- **Read this repo's files first.** Before answering anything project-specific, read `README.md` and whichever source-of-truth files the intro above points at. Don't restate their contents in chat — quote or link instead.
- **Use `hzdb` for device-side work.** Anything that touches an attached Quest (install, launch, logs, screenshot, capture, manifest inspection) goes through `hzdb`, not raw `adb`.
- **Check live Horizon OS docs before answering API questions.** `hzdb docs search "..."` queries the live docs; training data on Horizon OS APIs goes stale fast.
- **Don't fabricate SDK / engine versions.** If a version isn't visible in this repo's files, say so rather than guessing.
