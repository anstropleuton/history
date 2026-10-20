use raylib::prelude::*;

/// Cell state.
#[derive(Clone, Copy, PartialEq, Eq, Debug)]
enum CellState {
    Alive,
    Dead,
    Born,
    Killed,
}

/// Individual cell in grid.
#[derive(Clone, Debug)]
struct Cell {
    state: CellState,
}

/// Grid of cells.
#[derive(Debug)]
struct Grid {
    /// Grid width.
    width: i32,
    /// Grid height.
    height: i32,
    /// Access pattern for cell in grid: cells[y * width + x]
    cells: Vec<Cell>,
}

/// Grid members
impl Grid {
    /// Create a new grid from size.
    fn new(width: i32, height: i32) -> Self {
        let size = (width * height) as usize;
        let mut grid = Grid {
            width,
            height,
            cells: vec![
                Cell {
                    state: CellState::Alive
                };
                size
            ],
        };

        if width >= 3 && height >= 3 {
            // Glider
            grid.get_cell(0, 0).state = CellState::Alive;
            grid.get_cell(1, 1).state = CellState::Alive;
            grid.get_cell(2, 1).state = CellState::Alive;
            grid.get_cell(0, 2).state = CellState::Alive;
            grid.get_cell(1, 2).state = CellState::Alive;
        }

        return grid;
    }

    /// Resize the grid by preserving as much data in order as possible.
    fn resize(&mut self, width: i32, height: i32) {
        let size = (width * height) as usize;
        let mut new_cells = vec![
            Cell {
                state: CellState::Alive
            };
            size
        ];

        let min_width = if width > self.width {
            width
        } else {
            self.width
        };
        let min_height = if height > self.height {
            height
        } else {
            self.height
        };

        for x in 0..min_width {
            for y in 0..min_height {
                new_cells[(y * min_width + x) as usize] = self.get_cell(x, y).clone();
            }
        }

        self.width = width;
        self.height = height;
        self.cells = new_cells;
    }

    /// Get reference to cell at x, y.
    fn get_cell(&mut self, x: i32, y: i32) -> &mut Cell {
        let x = ((x % self.width) + self.width) % self.width;
        let y = ((y % self.height) + self.height) % self.height;
        return &mut self.cells[(y * self.width + x) as usize];
    }

    /// Get number of alive neighbors.
    fn neighbors(&mut self, x: i32, y: i32) -> i32 {
        let mut neighbors = 0;

        for dx in -1..=1 {
            for dy in -1..=1 {
                if dx == 0 && dy == 0 {
                    continue;
                }

                let xx = x + dx;
                let yy = y + dy;

                if self.get_cell(xx, yy).state == CellState::Alive
                    || self.get_cell(xx, yy).state == CellState::Killed
                {
                    neighbors += 1;
                }
            }
        }

        return neighbors;
    }

    /// Update all cell states.
    fn update(&mut self) {
        // Resolve next generation
        for x in 0..self.width {
            for y in 0..self.height {
                let neighbors = self.neighbors(x, y);
                if (self.get_cell(x, y).state == CellState::Alive
                    || self.get_cell(x, y).state == CellState::Killed)
                    && (neighbors < 2 || neighbors > 3)
                {
                    self.get_cell(x, y).state = CellState::Killed;
                }
                if (self.get_cell(x, y).state == CellState::Dead
                    || self.get_cell(x, y).state == CellState::Born)
                    && neighbors == 3
                {
                    self.get_cell(x, y).state = CellState::Born;
                }
            }
        }

        // Apply next generation
        for x in 0..self.width {
            for y in 0..self.height {
                if self.get_cell(x, y).state == CellState::Killed {
                    self.get_cell(x, y).state = CellState::Dead;
                }
                if self.get_cell(x, y).state == CellState::Born {
                    self.get_cell(x, y).state = CellState::Alive;
                }
            }
        }
    }
}

/// Main
fn main() {
    let (mut rl, thread) = raylib::init()
        .size(1024, 768)
        .resizable()
        .title("Game of Life")
        .build();

    let mut grid = Grid::new(10, 10);

    let mut running = true;
    let mut interval_counter = 0.0;
    let mut update_interval = 0.1;

    while !rl.window_should_close() {
        // Keyboard controls

        if rl.is_key_pressed(KeyboardKey::KEY_EQUAL) {
            update_interval += 0.1;
        }

        if rl.is_key_pressed(KeyboardKey::KEY_MINUS) {
            update_interval -= 0.1;
        }

        if rl.is_key_pressed(KeyboardKey::KEY_SPACE) {
            running = !running;
        }

        if rl.is_key_pressed(KeyboardKey::KEY_UP) {
            grid.resize(grid.width, grid.height - 1);
        }

        if rl.is_key_pressed(KeyboardKey::KEY_DOWN) {
            grid.resize(grid.width, grid.height + 1);
        }

        if rl.is_key_pressed(KeyboardKey::KEY_LEFT) {
            grid.resize(grid.width - 1, grid.height);
        }

        if rl.is_key_pressed(KeyboardKey::KEY_RIGHT) {
            grid.resize(grid.width + 1, grid.height);
        }

        // Mouse controls

        let screen = Vector2 {
            x: rl.get_screen_width() as f32,
            y: rl.get_screen_height() as f32,
        };
        let cell_unit = Vector2 {
            x: screen.x / grid.width as f32,
            y: screen.y / grid.height as f32,
        };
        let selected_x = (rl.get_mouse_position().x / cell_unit.x) as i32;
        let selected_y = (rl.get_mouse_position().y / cell_unit.y) as i32;

        if rl.is_mouse_button_down(MouseButton::MOUSE_BUTTON_LEFT) {
            grid.get_cell(selected_x, selected_y).state = CellState::Alive;
        }

        if rl.is_mouse_button_down(MouseButton::MOUSE_BUTTON_RIGHT) {
            grid.get_cell(selected_x, selected_y).state = CellState::Dead;
        }

        // Update

        if running {
            interval_counter += rl.get_frame_time();
            if interval_counter >= update_interval {
                interval_counter = 0.0;
                grid.update();
            }
        }

        // Draw

        let mut d = rl.begin_drawing(&thread);

        d.clear_background(Color {
            r: 32,
            g: 32,
            b: 32,
            a: 255,
        });

        for x in 0..grid.width {
            for y in 0..grid.height {
                if grid.get_cell(x, y).state == CellState::Alive {
                    d.draw_rectangle_rec(
                        Rectangle {
                            x: cell_unit.x * x as f32,
                            y: cell_unit.y * y as f32,
                            width: cell_unit.x,
                            height: cell_unit.y,
                        },
                        Color {
                            r: 64,
                            g: 64,
                            b: 64,
                            a: 255,
                        },
                    );
                }
            }
        }
    }
}
