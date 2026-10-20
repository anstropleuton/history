/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Test all of Minesweeper implementation.
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

int main()
{}
