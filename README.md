# cub3D

*This project has been created as part of the 42 curriculum by afahs, raelhara.*

## Description

cub3D is a 3D graphical representation of a maze from a first-person perspective, inspired by the world-famous Wolfenstein 3D game (the first FPS ever). The project uses raycasting techniques to render a 3D environment from a 2D map, similar to classic games from the early 1990s.

The goal is to create a dynamic view inside a maze where the player can navigate using keyboard controls. The project implements fundamental graphics programming concepts including texture mapping, raycasting algorithms, and real-time rendering.

**Key Features:**
- First-person 3D maze navigation
- Raycasting engine for rendering walls with textures
- Different wall textures based on orientation (North, South, East, West)
- Customizable floor and ceiling colors
- Minimap display (toggle with M key)
- Smooth player movement and rotation
- Mouse look controls

## Instructions

### Prerequisites

The project requires:
- Linux operating system (tested on Ubuntu 24)
- MinilibX graphics library
- gcc compiler
- make

### Compilation

```bash
# Clone the repository
git clone [repository-url]
cd cub3D

# Compile the project
make

# Clean object files
make clean

# Remove all generated files
make fclean

# Recompile everything
make re
```

### Execution

```bash
./cub3D [path/to/map.cub]
```

**Example:**
```bash
./cub3D maps/complete_map.cub
```

### Controls

- **W/A/S/D** or **Arrow Keys**: Move forward/left/backward/right
- **Left/Right Arrows**: Rotate camera
- **Mouse Movement**: Look around (horizontal rotation)
- **M**: Toggle minimap display
- **ESC**: Exit the program
- **X** (close button): Exit the program

### Map File Format

Maps use the `.cub` extension and must follow this format:

```
NO ./textures/north_texture.xpm
SO ./textures/south_texture.xpm
WE ./textures/west_texture.xpm
EA ./textures/east_texture.xpm

F 220,100,0
C 225,30,0

1111111111111
1000000000001
1000000N00001
1000000000001
1111111111111
```

**Map Elements:**
- `NO`, `SO`, `WE`, `EA`: Texture paths for each wall direction
- `F`: Floor color (RGB format: R,G,B)
- `C`: Ceiling color (RGB format: R,G,B)
- `1`: Wall
- `0`: Empty space
- `N/S/E/W`: Player starting position and orientation

**Map Rules:**
- Map must be closed/surrounded by walls (1s)
- Exactly one player starting position
- All configuration elements must be present
- Textures must be in XPM format
- RGB values must be between 0-255

## Project Structure

### Parsing & Validation (`afahs`)

**Files:**
- `parsing.c` - Main parsing logic and initialization
- `parse_color.c` - RGB color parsing
- `parse_texture.c` / `parse_texture_utils.c` - Texture path parsing
- `parse_helpers.c` - Utility functions for parsing
- `parse_map_validate.c` - Map character validation
- `parse_utils.c` / `parse_utils_2.c` - Map closure and edge validation
- `validation.c` - File extension and texture validation

**Key Features:**
- Robust .cub file parsing with whitespace handling
- Comprehensive map validation:
  - Edge checking (all edges must be walls or spaces)
  - Flood fill algorithm to ensure map is properly closed
  - Player position validation
  - Character validation
- Duplicate definition detection
- Missing element detection
- Detailed error messages

### Rendering Engine (`raelhara`)

**Files:**
- `render.c` - Core rendering loop and pixel manipulation
- `raycast.c` - Raycasting algorithm implementation
- `draw_scene.c` - Scene rendering and ray casting
- `draw_wall.c` - Wall texture selection and rendering
- `minimap.c` - Minimap rendering
- `game.c` - MLX initialization and texture loading
- `init_game.c` - Game state initialization

**Key Features:**
- DDA (Digital Differential Analysis) raycasting
- Texture mapping with proper wall distance calculation
- Wall shading based on orientation (N/S walls darker than E/W)
- Efficient column-by-column rendering
- Minimap with player position indicator

### Common Components (`afahs & raelhara`)

**Files:**
- `main.c` - Entry point and game loop
- `close.c` - Cleanup and exit handling
- `map.c` / `map_*.c` - Map loading and memory management

**Responsibilities:**
- Memory leak prevention (Valgrind tested)
- Norminette compliance (25 lines per function, 5 functions per file)
- Error handling throughout the codebase
- Proper resource cleanup on exit

## Resources

### Classic References

**Raycasting:**
- [Lode's Computer Graphics Tutorial - Raycasting](https://lodev.org/cgtutor/raycasting.html) - Comprehensive raycasting tutorial
- [Wolfenstein 3D Source Code](https://github.com/id-Software/wolf3d) - Original game source
- [Ray-Casting Tutorial for Game Development](https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/)

**Graphics Programming:**
- [MinilibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx)
- [42 Docs - MiniLibX](https://github.com/42Paris/minilibx-linux)

**Map Validation:**
- [Flood Fill Algorithm](https://en.wikipedia.org/wiki/Flood_fill)

### AI Usage

**Claude (Anthropic) was used for:**

1. **Code Review and Debugging:**
   - Identifying memory leaks and suggesting fixes
   - Debugging Valgrind output
   - Norminette compliance refactoring (splitting functions, file organization)

2. **Documentation:**
   - Generating this README.md
   - Creating clear error messages

**AI was NOT used for:**
- Core raycasting algorithm implementation
- Texture rendering logic
- Game loop and player movement
- MLX integration and event handling
- Map file format design

## Technical Choices

### Parsing Architecture

The parsing system uses a two-phase approach:
1. **Configuration Phase**: Parses texture paths and colors
2. **Map Phase**: Loads and validates the map grid

This separation allows for flexible map file formatting while maintaining strict validation.

### Map Validation

Three-layer validation approach:
1. **Edge Checking**: Fast O(n) check of all four borders
2. **Character Validation**: Ensures only valid map characters
3. **Flood Fill**: Confirms playable area is fully enclosed

### Rendering Optimization

- Column-by-column rendering for cache efficiency
- Perpendicular wall distance calculation prevents fisheye effect
- Texture coordinate calculation optimized for integer arithmetic
- Minimap rendering only when enabled

## Memory Management

- All allocated memory is properly freed on exit
- Texture paths freed in cleanup_all()
- Map grid freed in free_map()
- No memory leaks (Valgrind verified)
- Proper error handling with cleanup on failure paths

## Known Limitations

- Maps must be rectangular (padded with spaces)
- Only XPM texture format supported
- Mouse controls limited to horizontal rotation
- No support for sprites or enemies (bonus features)

## Authors

**afahs:**
- Parsing and validation system
- Map file format handling
- Error management
- Memory leak prevention

**raelhara:**
- Raycasting engine
- Texture rendering
- Player movement and controls
- Minimap implementation

**Joint Responsibilities:**
- Norminette compliance
- Code review and debugging
- Testing and validation
- Documentation