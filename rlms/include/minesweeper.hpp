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
 *  This project implements the game Minesweeper along with additional features
 *  to enhance the game.
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

#pragma once

#include <random>
#include <stdexcept>
#include <vector>

/**
 *  @brief  All Minesweeper's contents in this namespace.
 */
namespace minesweeper {

/**
 *  @brief  Minesweeper board generation configuration.
 */
struct config {

    /**
     *  @brief  Board width.
     */
    int width;

    /**
     *  @brief  Board height.
     */
    int height;

    /**
     *  @brief  Number of mines per board.
     */
    int mines;

    /**
     *  @brief  Maximum number of logically solvable board generation attempts
     *          before giving up on logically solvable board generation.
     *
     *  In case the maximum attempts was reached, good luck.
     */
    int max_generation_attempts;

    /**
     *  @brief  Seed for RNG.
     */
    int seed;

    /**
     *  @brief  Configure.  Use seed -1 for random seed.
     */
    inline config(
        int width, int height, int mines,
        int max_generation_attempts = 10000, int seed = -1
    ) : width(width), height(height), mines(mines),
        max_generation_attempts(max_generation_attempts), seed(seed)
    {
        if (this->seed == -1)
        {
            std::random_device rd;
            this->seed = rd();
        }
    }

    /**
     *  @brief  Returns true if the configuration is valid.
     */
    [[nodiscard]] inline constexpr auto validate() const
    {
        if (width < 1 || height < 1)
        {
            return false;
        }

        if (mines > width * height - 9)
        {
            return false;
        }

        if (max_generation_attempts < 1)
        {
            return false;
        }

        return true;
    }
};

/**
 *  @brief  Minesweeper cell state.
 *  @note   To check if a cell is hidden, perform comparisons on the `revealed`
 *          state, not on `hidden` state.  If it is not `revealed`, it implies
 *          hidden, flagged or question-marked.  Performing comparisons on
 *          `hidden` does not mean that it is hidden or revealed, it can also
 *          imply that the cell is flagged or question marked.
 */
enum class c_state {
    /**
     *  @brief  Unspecified.
     */
    unknown = -1,
    /**
     *  @brief  Cell is revealed.
     */
    revealed, // Perform comparisons on this
    /**
     *  @brief  Cell is hidden.
     */
    hidden,
    /**
     *  @brief  Cell is flagged.
     */
    flagged,
    /**
     *  @brief  Cell is question-marked.
     */
    q_marked,
    /**
     *  @brief  Max.
     */
    max
};

/**
 *  @brief  Convert @c c_state to string.
 */
[[nodiscard]] inline auto to_string(c_state state)
{
    using namespace std::string_literals;

    switch (state)
    {
        case c_state::unknown: return "unknown"s;
        case c_state::revealed: return "revealed"s;
        case c_state::hidden: return "hidden"s;
        case c_state::flagged: return "flagged"s;
        case c_state::q_marked: return "q_marked"s;
        case c_state::max: return "max"s;
    }
    return ""s;
}

/**
 *  @brief  Minesweeper cell in the board.
 */
struct cell {

    /**
     *  @brief  True if the cell is a mine.
     */
    bool is_mine;

    /**
     *  @brief  The number of neighboring cells that are mines.
     */
    int n_mines;

    /**
     *  @brief  Cell state.
     */
    c_state state;
};

/**
 *  @brief  Minesweeper game state.
 */
enum class g_state {
    /**
     *  @brief  Unspecified.
     */
    unknown = -1,
    /**
     *  @brief  Game has not been started.
     */
    not_started,
    /**
     *  @brief  Player is playing the game.
     */
    playing,
    /**
     *  @brief  Game over and the player won.
     */
    won,
    /**
     *  @brief  Game over and the player lost.
     */
    lost,
    /**
     *  @brief  Max.
     */
    max
};

/**
 *  @brief  Convert @c g_state to string.
 */
[[nodiscard]] inline auto to_string(g_state state)
{
    using namespace std::string_literals;

    switch (state)
    {
        case g_state::unknown: return "unknown"s;
        case g_state::not_started: return "not_started"s;
        case g_state::playing: return "playing"s;
        case g_state::won: return "won"s;
        case g_state::lost: return "lost"s;
        case g_state::max: return "max"s;
    }
    return ""s;
}

/**
 *  @brief  Minesweeper.
 */
struct minesweeper {

    /**
     *  @brief  Configuration.
     */
    const config cfg;

    /**
     *  @brief  Board.
     */
    std::vector<std::vector<cell>> board;

    /**
     *  @brief  Game state.
     */
    g_state state;

    /**
     *  @brief  Initialize minesweeper.
     *
     *  @param  cfg  Configuration.
     */
    inline minesweeper(config cfg)
        : cfg(cfg), state(g_state::unknown)
    {
        if (!cfg.validate())
        {
            throw std::invalid_argument("Invalid configuration");
        }

        // Assumption: Board is ALWAYS the correct size and it never changes
        // at any given point in time.  You should construct a new minesweeper
        // object for different size.
        board.resize(cfg.width);
        for (auto &board : board)
        {
            board.resize(cfg.height);
        }

        reset();
    }

    /**
     *  @brief  Reset everything.
     */
    auto reset() -> void;

    /**
     *  @brief  Reset board completely.
     */
    auto reset_board() -> void;

    /**
     *  @brief  Reset board state.
     */
    auto reset_board_state() -> void;

    /**
     *  @brief   Generate the board.
     *
     *  Generate the board in a logically solvable manner.  Attempts to generate
     *  the board in a logically solvable manner @c cfg.max_generation_attempts
     *  times.  In case of failure, keeps the logically unsolvable board,
     *  meaning, the player must suffer >:)
     *
     *  Mines are not placed at cell coordinates @c start_x and @c start_y and
     *  its neighbors.
     *
     *  @param   start_x  Starting cell coordinates X.
     *  @param   start_y  Starting cell coordinates Y.
     *  @return  True if the board is logically solvable.
     */
    auto generate_board(int start_x, int start_y) -> bool;

    /**
     *  @brief  Generate mines in the board.
     *
     *  @param  gen   Random number generator.
     *  @param  forb  Forbidden coordinates (don't place mines there).
     */
    auto generate_mines(
        std::mt19937                    &gen,
        std::vector<std::pair<int, int>> forb
    ) -> void;

    /**
     *  @brief  Compute the number of mines in the neighbor cells.
     */
    auto compute_neighbor_mines() -> void;

    /**
     *  @brief  Helper to get the coordinates of neighboring cells.
     *
     *  @param  x  Cell coordinates X.
     *  @param  y  Cell coordinates Y.
     */
    [[nodiscard]] auto n_cells(int x, int y)
    -> std::vector<std::pair<int, int>>;

    /**
     *  @brief  Check win condition (all non-mine cells must be revealed).
     */
    [[nodiscard]] auto check_win() -> bool;

    /**
     *  @brief  Give a vague hint on number of mines left to be revealed.
     *          It does not spoil the result because it just returns
     *          number of total mines minus number of flagged cells.
     *  @note   Be sure to handle negative numbers.
     */
    [[nodiscard]] auto mines_left() -> int;

    /**
     *  @brief  On primary click.
     *
     *  @param  x  Primary click on cell coordinates X.
     *  @param  y  Primary click on cell coordinates Y.
     */
    auto on_primary_click(int x, int y) -> void;

    /**
     *  @brief  On secondary click.
     *
     *  @param  x  Secondary click on cell coordinates X.
     *  @param  y  Secondary click on cell coordinates Y.
     */
    auto on_secondary_click(int x, int y) -> void;

    /**
     *  @brief  Reveal cell and flood-fill.
     *
     *  @param  x  Cell coordinates X.
     *  @param  y  Cell coordinates Y.
     */
    auto reveal(int x, int y) -> void;

    /**
     *  @brief  Speed reveal neighbor cell.
     *
     *  @param  x  Cell coordinates X.
     *  @param  y  Cell coordinates Y.
     */
    auto speed_reveal(int x, int y) -> void;

    /**
     *  @brief  Toggle cell state (hidden -> flagged -> q_marked -> hidden).
     *
     *  @param  x  Cell coordinates X.
     *  @param  y  Cell coordinates Y.
     */
    auto toggle(int x, int y) -> void;

    /**
     *  @brief  Speed toggle neighbor cell.
     *
     *  @param  x  Cell coordinates X.
     *  @param  y  Cell coordinates Y.
     */
    auto speed_toggle(int x, int y) -> void;

    /**
     *  @brief  Simple solver to deduce logical solvability, starting from
     *          starting cell coordinates.
     *
     *  @param  start_x  Starting cell coordinates X.
     *  @param  start_y  Starting cell coordinates Y.
     */
    auto logically_solvable(int start_x, int start_y) -> bool;
};

} // namespace ms
