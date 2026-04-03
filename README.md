# 🍩 Spinning Geometry — ASCII 3D Renderer in C++

A terminal-based 3D renderer written in C++ that animates geometric figures using ASCII characters and classic rasterization techniques. Includes a spinning **Donut (Torus)**, a **Cube**, and a **Pyramid**.

```
         .  .....  ....
      ,;=~:::::::::::::~;=,
    ,=::::::::::::::::::::::::=,
   ;:::::::::::::::::::::::::::;
   ~::::::::::::::::::::::::::::~
   ~::::::::::::::::::::::::::::~
    =::::::::::::::::::::::::=
      ~;::::::::::::::::;~
          ....   ....
```

---

## ✨ Features

- **3D Torus (Donut)** rendered with rotation on X and Z axes
- **3D Cube** with perspective projection
- **3D Pyramid** with surface shading
- ASCII-based lighting using the Z-component of surface normals
- Z-buffer implementation to handle depth correctly
- Interactive menu to select which figure to render
- Singleton `Display` class that manages screen and Z-buffer state

---

## 📁 Project Structure

```
SpinningDonut/
├── Main.cpp               # Entry point — interactive menu
├── SpinningDonut.h/.cpp   # Torus rendering logic
├── SpinningCube.h/.cpp    # Cube rendering logic
├── SpinningPyramid.h/.cpp # Pyramid rendering logic
├── ScreenHandler.h/.cpp   # Display singleton (screen buffer + Z-buffer)
└── SpinningDonut.vcxproj  # Visual Studio project file
```

---

## ⚙️ How It Works

### 3D Projection

Each 3D point is projected onto a 2D screen using perspective projection:

```
screenX = (X * FOV / Z) * scaleX + width / 2
screenY = (Y * FOV / Z) * scaleY + height / 2
```

### Torus Equation

The surface of the torus is parameterized by two angles, `phi` (θ₁) and `theta` (θ₂):

```
X = (R1 + R2 * cos(theta)) * cos(phi)
Y = (R1 + R2 * cos(theta)) * sin(phi)
Z = R2 * sin(theta)
```

Where:
- `R1 = 10` — distance from the center of the tube to the center of the torus
- `R2 = 3`  — radius of the tube

### ASCII Shading

Lighting is computed from the Z-component of the surface normal after rotation. The result maps to a character palette:

```cpp
const char DONUT_CHARS[] = { '.', ',', '-', '~', ':', ';', '=', '#' };
```

Darker characters (`.`, `,`) represent low light; brighter ones (`=`, `#`) represent surfaces facing the light source.

### Z-Buffer

The `Display` class maintains a Z-buffer to avoid drawing closer pixels over farther ones:

```cpp
if (depth > zBuffer[X][Y]) {
    zBuffer[X][Y] = depth;
    screen[X][Y] = symbol;
}
```

---

## 🚀 Getting Started

### Requirements

- Windows OS (uses `system("cls")` and ANSI escape codes)
- C++17 or later
- Visual Studio 2022 (or any C++ compiler with `<thread>` support)

### Build & Run (Visual Studio)

1. Clone the repository:
   ```bash
   git clone https://github.com/Batox369/SpinningDonut.git
   ```
2. Open `SpinningDonut.slnx` in Visual Studio
3. Build the project (`Ctrl + Shift + B`)
4. Run (`F5`)

### Build & Run (g++ / MinGW)

```bash
g++ -std=c++17 -o SpinningDonut Main.cpp SpinningDonut.cpp SpinningCube.cpp SpinningPyramid.cpp ScreenHandler.cpp
./SpinningDonut
```

---

## 🎮 Usage

When you run the program, a menu will appear:

```
------| FIGURAS 3D |------
(1). Dona
(2). Cubo
(3). Piramide

(0). Salir
```

Select a number and press Enter to watch the figure spin. Press `Ctrl + C` to stop the animation and return to the menu.

---

## 🧠 Concepts Explored

| Concept | Description |
|---|---|
| Parametric surfaces | Torus, cube, and pyramid defined via angles and parameters |
| Rotation matrices | X and Z axis rotations applied to 3D points |
| Perspective projection | 3D → 2D mapping using FOV and distance |
| Normal vectors | Used for per-point lighting computation |
| Z-buffering | Depth sorting to avoid overdraw artifacts |
| Singleton pattern | `Display` class manages a single shared screen buffer |

---

## 📷 Preview

The rendering uses ASCII art directly in the terminal. Enable ANSI escape codes in your Windows terminal for the best experience (Windows Terminal or VS Code integrated terminal recommended).

---

## 📜 License

This project is open source and free to use for educational purposes.

---

*Made by [Batox369](https://github.com/Batox369)*
