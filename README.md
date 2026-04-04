# Spinning Geometry — ASCII 3D Renderer in C++

A terminal-based 3D renderer written in C++ that animates geometric figures using ASCII characters and classic rasterization techniques. Includes a spinning **Donut (Torus)**, a **Cube**, and a **Pyramid**.

*Inspired by Andy Sloane's classic "Donut math" article and the Green Code video.*

---

## Features

- **3D Donut** rendered with rotation on X and Z axes
- **3D Cube** with perspective projection
- **3D Pyramid** with surface shading
- ASCII-based lighting using the Z-component of surface normals
- Z-buffer implementation to handle depth correctly
- Interactive menu to select which figure to render
- Singleton `Display` class that manages screen and Z-buffer state

---

## How It Works

### 3D Projection

Each 3D point is projected onto a 2D screen using perspective projection:

```
screenX = (X * FOV / Z) * scaleX + width / 2
screenY = (Y * FOV / Z) * scaleY + height / 2
```

### Donut Equation

The surface of the donut is parameterized by two angles, `phi` (θ₁) and `theta` (θ₂):

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

## Usage

When you run the program, a menu will appear:

```
------| SPINNING GEOMETRY |------
(1). Donut
(2). Cube
(3). Pyramid

(0). EXIT
```

Select a number and press Enter to watch the figure spin. Press `Ctrl + C` to stop the animation and return to the menu.

---

## Concepts Explored

| Concept | Description |
|---|---|
| Parametric surfaces | Torus, cube, and pyramid defined via angles and parameters |
| Rotation matrices | X and Z axis rotations applied to 3D points |
| Perspective projection | 3D → 2D mapping using FOV and distance |
| Normal vectors | Used for per-point lighting computation |
| Z-buffering | Depth sorting to avoid overdraw artifacts |
| Singleton pattern | `Display` class manages a single shared screen buffer |

---

## Preview

The rendering uses ASCII art directly in the terminal. Enable ANSI escape codes in your Windows terminal for the best experience (Windows Terminal or VS Code integrated terminal recommended).

---

This project is open source and free to use for educational purposes.

*Made by [Batox369](https://github.com/Batox369)*
