# Graphics Engine

A modern OpenGL graphics engine built from scratch in C++20 as a personal learning project.

The goal of this project is not only to learn modern OpenGL, but also to design a clean, maintainable rendering architecture similar to what would be found in a small game engine.

---

## Tech Stack

- **Language:** C++20
- **Graphics API:** OpenGL 3.3 Core
- **Windowing:** GLFW
- **OpenGL Loader:** GLAD
- **Math Library:** GLM _(future milestones)_
- **Build System:** CMake + Ninja
- **Package Manager:** vcpkg

---

## Project Structure

```text
assets/
├── shaders/

external/
└── glad/

src/
├── core/
├── graphics/
├── math/
├── scene/
└── utils/
```

---

## Current Architecture

```text
Application
│
├── Window
│
└── Renderer
    │
    ├── Shader
    └── Mesh
```

---

## Screenshots

### First Triangle (v0.1.0)

<details>
  <summary></summary>
  
  <img src="docs/images/First%20Triangle.png" width="400" alt="First Triangle">
  
</details>

---

## Planned Roadmap

- [x] Window creation
- [x] OpenGL initialization
- [x] Shader abstraction
- [x] Mesh abstraction
- [x] Render first triangle
- [ ] Vertex colors
- [ ] Index buffers (EBOs)
- [ ] Transform system
- [ ] Camera
- [ ] Texture loading
- [ ] Model loading
- [ ] Lighting
- [ ] Scene graph
- [ ] Engine MVP

---

## Building

### Requirements

- CMake 3.20+
- Ninja
- MSYS2 MinGW
- vcpkg

### Clone

```bash
git clone <repo-url>
cd GraphicsEngine
```

### Configure

```bash
cmake --preset default
```

### Build

```bash
cmake --build --preset default
```

### Run

```bash
./build/GraphicsEngine.exe
```

---

## Purpose

This repository serves as a long-term graphics programming journal. Rather than following a tutorial verbatim, each feature is implemented incrementally with an emphasis on understanding the underlying graphics pipeline, engine architecture, and modern C++ design.
