# 3d-renderer

## Installation and Build

For automatic build, use the script:
```
source setup.sh
```

## Run

To run the program:
```
./renderer [model1] [color1] [coordinates1] [model2] [color2] [coordinates2] ...
```

### Parameters:
- **Model**: The name of the file from the `models` folder without the `.obj` extension.
- **Color**: Hexadecimal color code (e.g., `0xff0000` for red). If the color is not specified, the model will be displayed in white.
- **Coordinates**: Three numbers specifying the position of the model in the scene (x y z). If the coordinates are not specified, the model will be placed at the origin (0 0 0).

### Examples:
```
./renderer teapot                           # White teapot at the origin
./renderer teapot 0xff0000                  # Red teapot at the origin
./renderer teapot 0xff0000 1.5 2.0 -1.0     # Red teapot at position (1.5, 2.0, -1.0)
./renderer teapot 0xff0000 cow 0x0000ff     # Red teapot and blue cow at the origin
./renderer teapot 0xff0000 1 2 3 cow        # Red teapot at position (1, 2, 3) and white cow at the origin
```

## Adding Your Own Models

You can add your own 3D models:
1. Import the model into Blender
2. Export it to the `.obj` format without normals and textures
3. Place it in the `models` folder

## Controls

- **Movement**: WASD, Shift (up), Ctrl (down)
- **Camera Rotation**: QE, arrow keys
- **Rendering Modes**: 1, 2
  - **1**: Wireframe (triangle mesh)
  - **2**: Filled triangles with lighting
