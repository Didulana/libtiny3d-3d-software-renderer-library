# libtiny3d — Lightweight 3D Software Renderer

A C-based software renderer that converts 3D object data into 2D images using custom math, transformations, and lighting — all without using any external graphics libraries.

Developed as part of CO1020: Computer Systems Programming  
University of Peradeniya — Department of Computer Engineering  
June 2025

## Features

- Custom 3D vector and matrix math engine
- Floating-point canvas with subpixel precision and bilinear blending
- Matrix-based 3D transformations: translation, rotation, scaling
- Perspective projection and circular viewport clipping
- Wireframe rendering with anti-aliased lines
- Lambertian lighting with multiple dynamic light sources
- Bézier interpolation for animation paths
- Demo with synchronized cube and soccer ball animations rendered frame-by-frame

## Build Instructions

### Requirements

- C compiler with C99 support (e.g. GCC, Clang)
- GNU Make
- ImageMagick (optional, for GIF generation)

### To Build the Project

```bash
make
```

### To Run the Demo

```bash
make demo_lighting
./build/demo_lighting
```

This generates 120 frames in `build/` as `frame_000.pgm` to `frame_119.pgm`.

### Convert Frames to GIF (Optional)

If ImageMagick is installed:

```bash
cd build
magick -delay 5 -loop 0 frame_*.pgm demo.gif
```

## Project Structure

```
libtiny3d/
├── include/          # Header files
├── src/              # Core source files (canvas, math3d, renderer, lighting)
├── tests/            # Unit tests and feature test cases
├── visual_tests/     # Final animated demo and assets
├── build/            # Generated frames and output binaries
├── documentation/    # Project report and presentation
├── Makefile
├── README.md
└── GroupXX_project.pdf
```

## Notes

- Output images (`.pgm`, `.gif`) are excluded from version control using `.gitignore`
- All 3D geometry is defined in code (no external mesh importing)
- Lighting is calculated per-line using direction vectors
- Rendering is entirely CPU-based with no external graphics APIs

## Authors

This project was developed as a 4-week group assignment to explore the fundamentals of 3D rendering, graphics math, and systems-level programming in C. It emphasizes accuracy, performance, and a deep understanding of transformation pipelines.
