/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Minesweeper implementation.
 *
 *  @copyright  Copyright (c) 2024 Anstro Pleuton
 *
 *   __  __ _
 *  |  \/  (_)_ __   ___  _____      _____  ___ _ __   ___ _ __
 *  | |\/| | | '_ \ / _ \/ __\ \ /\ / / _ \/ _ \ '_ \ / _ \ '__|
 *  | |  | | | | | |  __/\__ \\ V  V /  __/  __/ |_) |  __/ |
 *  |_|  |_|_|_| |_|\___||___/ \_/\_/ \___|\___| .__/ \___|_|
 *                                             |_|
 *
 *  Minesweeper game implementation with a few enhancement features.
 *
 *  The features include:
 *  - Core Minesweeper game.
 *  - Generate mines after the first click.
 *  - Always generate completely logically solvable board. This means that the
 *    player need have to guess or take risks.
 *  - Flag: Flag prevents accidental left click to reveal the cell when the cell
 *    is flagged.
 *  - Question Mark: Question marking achieves the effect of flag, but does not
 *    count in the Mines Count display.
 *  - Speed reveal: Left clicking a revealed cell with mine number > 0, if the
 *    number of adjacent hidden cells without a flag equal to the number on the
 *    clicked cell, reveals all the adjacent hidden cells without a flag.
 *  - Speed flag: Right clicking a revealed cell with mine number > 0, if the
 *    number of adjacent hidden cells equal to the number on the clicked cell,
 *    flags all the adjacent hidden cells.
 *
 *  This software is licensed under the terms of MIT License.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 *  Credits where credit's due:
 *  - ASCII Art generated using https://www.patorjk.com/software/taag with font
 *    "Standard".
 */

#include "minesweeper.hpp"

#include <algorithm>
#include <queue>
#include <random>
#include <utility>
#include <vector>

/**
 *  @brief  Reset everything.
 */
auto minesweeper::minesweeper::reset() -> void
{
    state = g_state::not_started;
    reset_board();
}

/**
 *  @brief  Reset board completely.
 */
auto minesweeper::minesweeper::reset_board() -> void
{
    for (int x = 0; x < cfg.width; x++)
    {
        for (int y = 0; y < cfg.height; y++)
        {
            board[x][y] = cell {};
        }
    }

    reset_board_state();
}

/**
 *  @brief  Reset board state.
 */
auto minesweeper::minesweeper::reset_board_state() -> void
{
    for (int x = 0; x < cfg.width; x++)
    {
        for (int y = 0; y < cfg.height; y++)
        {
            board[x][y].state = c_state::hidden;
        }
    }
}

/**
 *  @brief  Generate mines in the board.  Does not place mine at starting
 *          cell coordinates, or its neighbors.
 *
 *  @param  start_x  Starting cell coordinates X.
 *  @param  start_y  Starting cell coordinates Y.
 */
auto minesweeper::minesweeper::generate_board(int start_x, int start_y) -> bool
{
    // Forbidden cells
    std::vector<std::pair<int, int>> forb = { { start_x, start_y } };
    for (auto [nx, ny] : n_cells(start_x, start_y))
    {
        forb.emplace_back(nx, ny);
    }

    // Generator is here instead of generate_mines to allow different generation
    // for each generation attempt
    std::mt19937 gen(cfg.seed);
    bool         result = false;

    for (int i = 0; i < cfg.max_generation_attempts; i++)
    {
        // Reset board completely
        reset_board();

        // Generate mines
        generate_mines(gen, forb);

        // Compute neighbor mines count
        compute_neighbor_mines();

        // Check logically solvability
        if ((result = logically_solvable(start_x, start_y)))
        {
            break;
        }
    }

    return result;
}

/**
 *  @brief  Generate mines in the board.
 *
 *  @param  gen   Random number generator.
 *  @param  forb  Forbidden coordinates (don't place mines there).
 */
auto minesweeper::minesweeper::generate_mines(
    std::mt19937                    &gen,
    std::vector<std::pair<int, int>> forb
) -> void
{
    std::uniform_int_distribution x_dist(0, cfg.width - 1);
    std::uniform_int_distribution y_dist(0, cfg.height - 1);

    for (int j = 0; j < cfg.mines; j++)
    {
        int x, y;
        do
        {
            x = x_dist(gen);
            y = y_dist(gen);
        }
        while (std::find(forb.begin(), forb.end(), std::make_pair(x, y))
               != forb.end());

        board[x][y].is_mine = true;
    }
}

/**
 *  @brief  Compute the number of mines in the neighbor cells.
 */
auto minesweeper::minesweeper::compute_neighbor_mines() -> void
{
    for (int x = 0; x < cfg.width; x++)
    {
        for (int y = 0; y < cfg.height; y++)
        {
            int count = 0;

            for (auto [nx, ny] : n_cells(x, y))
            {
                if (board[nx][ny].is_mine)
                {
                    count++;
                }
            }

            board[x][y].n_mines = count;
        }
    }
}

/**
 *  @brief  Get the coordinates of neighboring cells.
 *
 *  @param  x  Cell coordinates X.
 *  @param  y  Cell coordinates Y.
 */
auto minesweeper::minesweeper::n_cells(int x, int y)
-> std::vector<std::pair<int, int>>
{
    std::vector<std::pair<int, int>> neighbors;

    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
            {
                continue;
            }

            int nx = dx + x;
            int ny = dy + y;

            if (nx < 0 || nx >= cfg.width
             || ny < 0 || ny >= cfg.height)
            {
                continue;
            }

            neighbors.emplace_back(nx, ny);
        }
    }

    return neighbors;
}

/**
 *  @brief  Check win condition (all non-mine cells must be revealed).
 */
[[nodiscard]] auto minesweeper::minesweeper::check_win() -> bool
{
    for (int x = 0; x < cfg.width; x++)
    {
        for (int y = 0; y < cfg.height; y++)
        {
            if (!board[x][y].is_mine && board[x][y].state != c_state::revealed)
            {
                return false;
            }
        }
    }

    return true;
}

/**
 *  @brief  Give a vague hint on number of mines left to be revealed.
 *          It does not spoil the result because it just returns
 *          number of total mines minus number of flagged cells.
 *  @note   Be sure to handle negative numbers.
 */
[[nodiscard]] auto minesweeper::minesweeper::mines_left() -> int
{
    int flagged_count = 0;
    for (int x = 0; x < cfg.width; x++)
    {
        for (int y = 0; y < cfg.height; y++)
        {
            if (board[x][y].state == c_state::flagged)
            {
                flagged_count++;
            }
        }
    }

    return cfg.mines - flagged_count;
}

/**
 *  @brief  On primary click.
 *
 *  @param  x  Primary click on cell coordinates X.
 *  @param  y  Primary click on cell coordinates Y.
 */
auto minesweeper::minesweeper::on_primary_click(int x, int y) -> void
{
    if (x < 0 || x >= cfg.width)
    {
        throw std::runtime_error("Invalid cell coordinates X");
    }

    if (y < 0 || y >= cfg.height)
    {
        throw std::runtime_error("Invalid cell coordinates Y");
    }

    // First click
    if (state == g_state::not_started)
    {
        generate_board(x, y);
        reveal(x, y);
        state = g_state::playing;
        return;
    }

    // Game not running, do nothing
    if (state != g_state::playing)
    {
        return;
    }

    // Flagged or question-marked cell, do nothing
    if (board[x][y].state == c_state::flagged
     || board[x][y].state == c_state::q_marked)
    {
        return;
    }

    // Cell is a mine, game over
    if (board[x][y].is_mine)
    {
        board[x][y].state = c_state::revealed;
        state = g_state::lost;
        return;
    }

    if (board[x][y].state != c_state::revealed)
    {
        // Reveal with flood-fill
        reveal(x, y);
    }
    else if (board[x][y].n_mines > 0)
    {
        // Speed reveal neighboring cells
        speed_reveal(x, y);
    }

    // All non-mine cells revealed?
    if (check_win())
    {
        state = g_state::won;
    }
}

/**
 *  @brief  On secondary click.
 *
 *  @param  x  Secondary click on cell coordinates X.
 *  @param  y  Secondary click on cell coordinates Y.
 */
auto minesweeper::minesweeper::on_secondary_click(int x, int y) -> void
{
    if (x < 0 || x >= cfg.width)
    {
        throw std::runtime_error("Invalid cell coordinates X");
    }

    if (y < 0 || y >= cfg.height)
    {
        throw std::runtime_error("Invalid cell coordinates Y");
    }

    if (board[x][y].state != c_state::revealed)
    {
        // Toggle flag state
        toggle(x, y);
    }
    else
    {
        // Speed toggle neighboring cells
        speed_toggle(x, y);
    }
}

/**
 *  @brief  Reveal cell and flood-fill.
 *
 *  @param  x  Cell coordinates X.
 *  @param  y  Cell coordinates Y.
 */
auto minesweeper::minesweeper::reveal(int x, int y) -> void
{
    if (x < 0 || x >= cfg.width)
    {
        throw std::runtime_error("Invalid cell coordinates X");
    }

    if (y < 0 || y >= cfg.height)
    {
        throw std::runtime_error("Invalid cell coordinates Y");
    }

    std::queue<std::pair<int, int>> q;
    q.emplace(x, y);

    while (!q.empty())
    {
        auto [cx, cy] = q.front();
        q.pop();

        // Cell already revealed, or is flagged/question-marked
        if (board[cx][cy].state != c_state::hidden)
        {
            continue;
        }

        board[cx][cy].state = c_state::revealed;

        // Stop expanding if cell has neighboring mines
        if (board[cx][cy].n_mines != 0)
        {
            continue;
        }

        // Add hidden neighbors to queue
        for (auto [nx, ny] : n_cells(cx, cy))
        {
            if (board[nx][ny].state == c_state::hidden)
            {
                q.emplace(nx, ny);
            }
        }
    }
}

/**
 *  @brief  Speed reveal cell.
 *
 *  @param  x  Cell coordinates X.
 *  @param  y  Cell coordinates Y.
 */
auto minesweeper::minesweeper::speed_reveal(int x, int y) -> void
{
    if (x < 0 || x >= cfg.width)
    {
        throw std::runtime_error("Invalid cell coordinates X");
    }

    if (y < 0 || y >= cfg.height)
    {
        throw std::runtime_error("Invalid cell coordinates Y");
    }

    // Coords of neighboring cells
    std::vector<std::pair<int, int>> flagged;
    std::vector<std::pair<int, int>> hidden;

    for (auto [nx, ny] : n_cells(x, y))
    {
        if (board[nx][ny].state == c_state::flagged)
        {
            flagged.emplace_back(nx, ny);
        }
        if (board[nx][ny].state != c_state::revealed)
        {
            hidden.emplace_back(nx, ny);
        }
    }

    if (flagged.size() == board[x][y].n_mines)
    {
        for (auto [rx, ry] : hidden)
        {
            reveal(rx, ry);
        }
    }
}

/**
 *  @brief  Toggle cell state (hidden -> flagged -> q_marked -> hidden).
 *
 *  @param  x  Cell coordinates X.
 *  @param  y  Cell coordinates Y.
 */
auto minesweeper::minesweeper::toggle(int x, int y) -> void
{
    if (x < 0 || x >= cfg.width)
    {
        throw std::runtime_error("Invalid cell coordinates X");
    }

    if (y < 0 || y >= cfg.height)
    {
        throw std::runtime_error("Invalid cell coordinates Y");
    }

    if (board[x][y].state == c_state::hidden)
    {
        board[x][y].state = c_state::flagged;
    }
    else if (board[x][y].state == c_state::flagged)
    {
        board[x][y].state = c_state::q_marked;
    }
    else if (board[x][y].state == c_state::q_marked)
    {
        board[x][y].state = c_state::hidden;
    }
}

/**
 *  @brief  Speed toggle neighbor cell.
 *
 *  @param  x  Cell coordinates X.
 *  @param  y  Cell coordinates Y.
 */
auto minesweeper::minesweeper::speed_toggle(int x, int y) -> void
{
    if (x < 0 || x >= cfg.width)
    {
        throw std::runtime_error("Invalid cell coordinates X");
    }

    if (y < 0 || y >= cfg.height)
    {
        throw std::runtime_error("Invalid cell coordinates Y");
    }
}

/**
 *  @brief  Simple solver to deduce logical solvability, starting from
 *          starting cell coordinates.
 *
 *  @param  start_x  Starting cell coordinates X.
 *  @param  start_y  Starting cell coordinates Y.
 */
auto minesweeper::minesweeper::logically_solvable(
    int start_x,
    int start_y
) -> bool
{
    // This does not make a copy of the board, to reduce duplicate or hack-y
    // code for reveal logic.

    // Initial reveal
    reveal(start_x, start_y);

    // Deduction loop
    // Beware, never nesters
    bool progress = true;
    while (progress)
    {
        progress = false;

        for (int x = 0; x < cfg.width; x++)
        {
            for (int y = 0; y < cfg.height; y++)
            {
                if (board[x][y].state != c_state::revealed)
                {
                    continue;
                }

                // Coords of neighboring cells
                std::vector<std::pair<int, int>> flagged;
                std::vector<std::pair<int, int>> hidden;

                for (auto [nx, ny] : n_cells(x, y))
                {
                    if (board[nx][ny].state == c_state::flagged)
                    {
                        flagged.emplace_back(nx, ny);
                    }
                    if (board[nx][ny].state != c_state::revealed)
                    {
                        hidden.emplace_back(nx, ny);
                    }
                }

                // Rule 1: If the number of neighboring flagged cells equals the
                // number of neighboring mine cells, then all hidden cells are
                // safe to be revealed
                if (flagged.size() == board[x][y].n_mines)
                {
                    for (auto [hx, hy] : hidden)
                    {
                        reveal(hx, hy);
                        progress = true;
                    }
                }

                // Rule 2: If the number of neighboring flagged cells plus the
                // number of neighboring hidden cells equals the number of
                // neighboring cells that are mine, then all hidden cells are
                // mines.
                if (flagged.size() + hidden.size() == board[x][y].n_mines)
                {
                    for (auto [hx, hy] : hidden)
                    {
                        toggle(hx, hy);
                        progress = true;
                    }
                }
            }
        }
    }

    // The board is logically solvable if the algorithm won the game
    bool solved = check_win();
    reset_board_state();
    return solved;
}
