/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * MIT License                                                                     *
 *                                                                                 *
 * Copyright (c) 2022 GamerZ-0216                                                  *
 *                                                                                 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy    *
 * of this software and associated documentation files (the "Software"), to deal   *
 * in the Software without restriction, including without limitation the rights    *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 * copies of the Software, and to permit persons to whom the Software is           *
 * furnished to do so, subject to the following conditions:                        *
 *                                                                                 *
 * The above copyright notice and this permission notice shall be included in all  *
 * copies or substantial portions of the Software.                                 *
 *                                                                                 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
 * SOFTWARE.                                                                       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * BPLE Header file for World Control and management                               *
 * Program made by GamerZ-0216 ( @GamerZ-0216#7661 in Discord )                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Here is the official Discord server for BPLE                                    *
 * https://discord.gg/RD8vPwahPd                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Be sure to support me by subscribing to my YouTube channel                      *
 * https://www.youtube.com/channel/UCph0ej9zcBMCfosQLE1E7Jg                        *
 * Also be sure to join my discord server                                          *
 * https://discord.gg/xhXcpghqNb                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


 // Include main header
#include <GamerZ-BPLE.h>

// Actual data

std::vector< std::vector< bool > > GZBPLE::PartObjectFrame
{
	{ false },
	{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
	{ false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ true, true, true, true, true, true, true, true, true, true, false },
	{ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, true, true },
	{ false, false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false },
	{ false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false, false },
	{ false, false },
	{ false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
};

std::vector< std::vector< bool > > GZBPLE::PartObjectGripped
{
	{ false },
	{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
	{ false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false },
	{ true, true, true, true, true },
	{ true, true, true, true, true },
	{ true, true, true, true },
	{ true, true, true, true },
	{ false, false, false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false },
	{ true, true, true, true, true },
	{ false, false, false, false },
	{ true, true, true, true },
	{ false, false, false, false },
	{ false, false, false, false, false },
	{ false, false, false, false },
	{ true, true, true, true, true, true, true, false, true, true, true },
	{ false, false },
	{ false, false, false, false, false },
	{ false, false, false, false, false, false },
	{ false, false, false, false },
	{ false, false, false, false, false, false, false },
	{ true, true, true, true },
	{ true, true, false, false, false, false },
	{ false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
};

// Incomplete
std::vector< std::vector< int > > GZBPLE::PartObjectSizeX
{
	{ 0 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100 },
	{ false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, false },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, false, false, false },
	{ 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 0, 100, 100, 100 },
	{ 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 }
};

// Incomplete
std::vector< std::vector< int > > GZBPLE::PartObjectSizeY
{
	{ 0 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100 },
	{ false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, false },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, false, false, false },
	{ 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 0, 100, 100, 100 },
	{ 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 }
};

// Incomplete
std::vector< std::vector< int > > GZBPLE::PartObjectCenterX
{
	{ 0 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100 },
	{ false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, false },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, false, false, false },
	{ 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 0, 100, 100, 100 },
	{ 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 }
};

// Incomplete
std::vector< std::vector< int > > GZBPLE::PartObjectCenterY
{
	{ 0 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100 },
	{ false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, false },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false, false, false, false, false, false, false },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, false, false, false },
	{ 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 0, 100, 100, 100 },
	{ 100, 100 },
	{ 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100 },
	{ false, false, false, false },
	{ 100, 100, 100, 100, 100, 100, 100 },
	{ 100, 100, 100, 100 },
	{ false, false, 100, 100, 100, 100 },
	{ 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 }
};

std::vector< int > GZBPLE::SkinsPerPart
{
	0, 20, 1, 7, 6, 11, 134, 11, 7, 8, 8, 8, 6, 10, 10, 5, 5, 4, 4, 10, 7, 7, 7, 4, 4, 6, 8, 8, 8, 8, 6, 4, 4, 5, 4, 4, 4, 5, 4, 10, 2, 5, 6, 4, 7, 4, 6, 48
};

// Actual Functions

int GZBPLE::ConvertToInteger( std::string Input )
{
	std::stringstream Convert;
	int Output = 0;
	bool Negative = false;
	for( int Index = 0; Index < Input.length(); Index++ )
	{
		if( Input[ Index ] >= '0' && Input[ Index ] <= '9' ) Convert << Input[ Index ];
		if( Input[ Index ] == '-' ) Negative = true;
	}
	Convert >> Output;
	if( Negative ) Output = Output * -1;
	return Output;
}

// Default Constructor
GZBPLE::PartObject::PartObject( int PartType, int PartSkin, int PartPosX, int PartPosY, int PartProp, int PartFlip )
{
	Type = PartType;
	Skin = PartSkin;
	PosX = PartPosX;
	PosY = PartPosY;
	Prop = PartProp;
	Flip = PartFlip;
	Frame = PartObjectFrame.at( Type ).at( Skin );
	Gripped = PartObjectGripped.at( Type ).at( Skin );
	PropType = PartObjectPropType.at( Type ).at( Skin );
	SizeX = PartObjectSizeX.at( Type ).at( Skin );
	SizeY = PartObjectSizeY.at( Type ).at( Skin );
	CenterX = PartObjectCenterX.at( Type ).at( Skin );
	CenterY = PartObjectCenterY.at( Type ).at( Skin );
}

// World Functions

void GZBPLE::World::Load_Level( std::string File )
{
	std::ifstream Ifile;
	std::string LineData;
	std::stringstream Convert;
	Part.clear();
	Ifile.open( File.c_str() );
	if( Ifile.fail() )
	{
		Ifile.close();
		return;
	}
	for( int Lines = 0; std::getline( Ifile, LineData ); Lines++ )
	{
		int Sector = 0;
		LineData = LineData + ",";
		int Type, Skin, PosX, PosY, Prop, Flip;
		for( int Index = 0; Index < LineData.length(); Index++ )
		{
			if( LineData[ Index ] == ',' )
			{
				switch( Sector )
				{
					case 0: Type = ConvertToInteger( Convert.str() ); break;
					case 1: Skin = ConvertToInteger( Convert.str() ); break;
					case 2: PosX = ConvertToInteger( Convert.str() ); break;
					case 3: PosY = ConvertToInteger( Convert.str() ); break;
					case 4: Prop = ConvertToInteger( Convert.str() ); break;
					case 5: Flip = ConvertToInteger( Convert.str() ); break;
				}
				Convert.str( "" );
				Convert.clear();
				Sector++;
			}
			else Convert << LineData[ Index ];
		}
		Part.push_back( PartObject( Type, Skin, PosX, PosY, Prop, Flip ) );
		Convert.str( "" );
		Convert.clear();
	}
	Ifile.close();
}

void GZBPLE::World::Save_Level( std::string File )
{
	std::ofstream Ofile;
	Ofile.open( File.c_str() );
	if( Ofile.fail() )
	{
		Ofile.close();
		return;
	}
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		Ofile << Part.at( Index ).Type << ','
			<< Part.at( Index ).Skin << ','
			<< Part.at( Index ).PosX << ','
			<< Part.at( Index ).PosY << ','
			<< Part.at( Index ).Prop << ','
			<< Part.at( Index ).Flip << '\n';
	}
	Ofile.close();
}

void GZBPLE::World::Select_Part( int Index, int Mode = 0 )
{
	if( Index >= 0 && Index < Part.size() )
	{
		switch( Mode )
		{
			case 0: Part.at( Index ).Selected = !Part.at( Index ).Selected; break;
			case 1: Part.at( Index ).Selected = true; break;
			case 2: Part.at( Index ).Selected = false; break;
			default: return;
		}
	}
}

void GZBPLE::World::Select_Rectangle( int StartX, int StartY, int EndX, int EndY, int Method = 0, int Mode = 0 )
{
	bool Confirm = false;
	std::vector< int > PendingParts;
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		if( Part.at( Index ).PosX >= StartX &&
			Part.at( Index ).PosY >= StartY &&
			Part.at( Index ).PosX <= EndX &&
			Part.at( Index ).PosY <= EndY )
		{
			switch( Method )
			{
				case 0:
				{
					switch( Mode )
					{
						case 0: Part.at( Index ).Selected = !Part.at( Index ).Selected; break;
						case 1: Part.at( Index ).Selected = true; break;
						case 2: Part.at( Index ).Selected = false; break;
					}
				} break;
				case 1:
				{
					if( Part.at( Index ).Frame )
					{
						switch( Mode )
						{
							case 0: Part.at( Index ).Selected = !Part.at( Index ).Selected; break;
							case 1: Part.at( Index ).Selected = true; break;
							case 2: Part.at( Index ).Selected = false; break;
						}
					}
					else PendingParts.push_back( Index );
				} break;
				case 2:
				{
					if( !Part.at( Index ).Frame )
					{
						switch( Mode )
						{
							case 0: Part.at( Index ).Selected = !Part.at( Index ).Selected; break;
							case 1: Part.at( Index ).Selected = true; break;
							case 2: Part.at( Index ).Selected = false; break;
						}
					}
					else PendingParts.push_back( Index );
				} break;
				default: return;
			}
		}
	}
	if( PendingParts.size() != 0 )
	{
		for( int Alpha = 0; Alpha < Part.size(); Alpha++ )
		{
			for( int Beta = 0; Beta < PendingParts.size(); Beta++ )
			{
				if( Part.at( Alpha ).PosX == Part.at( PendingParts.at( Beta ) ).PosX &&
					Part.at( Alpha ).PosY == Part.at( PendingParts.at( Beta ) ).PosY )
				{
					if( Mode == 1 )
					{
						Part.at( PendingParts.at( Beta ) ).Selected = true;
					}
					if( Mode == 2 )
					{
						Part.at( PendingParts.at( Beta ) ).Selected = false;
					}
				}
			}
		}
	}
}

void GZBPLE::World::Move_Part( int OffsetX, int OffsetY, int Mode = 0 ){
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		switch( Mode )
		{
			case 0:
			{
				Part.at( Index ).PosX += OffsetX;
				Part.at( Index ).PosY += OffsetY;
			} break;
			case 1:
			{
				if( Part.at( Index ).Selected )
				{
					Part.at( Index ).PosX += OffsetX;
					Part.at( Index ).PosY += OffsetY;
				}
			} break;
			case 2:
			{
				if( !Part.at( Index ).Selected )
				{
					Part.at( Index ).PosX += OffsetX;
					Part.at( Index ).PosY += OffsetY;
				}
			} break;
			default: return;
		}
	}
}

void GZBPLE::World::Insert_Part( int PartType, int PartSkin, int PartPosX, int PartPosY, int PartProp, int PartFlip )
{
	bool TriedPlace = false;
	if( PartObjectFrame.at( PartType ).at( PartSkin ) )
	{
		for( int Index = 0; Index < Part.size(); Index++ )
		{
			if( Part.at( Index ).PosX == PartPosX &&
				Part.at( Index ).PosY == PartPosY )
			{
				TriedPlace = true;
				if( !Part.at( Index ).Frame )
				{
					Part.push_back( PartObject( PartType, PartSkin, PartPosX, PartPosY, PartProp, PartFlip ) );
					return;
				}
			}
		}
	}
	else{
		for( int Index = 0; Index < Part.size(); Index++ )
		{
			if( Part.at( Index ).PosX == PartPosX &&
				Part.at( Index ).PosY == PartPosY )
			{
				TriedPlace = true;
				if( PartObjectFrame.at( Part.at( Index ).Type ).at( Part.at( Index ).Skin ) )
				{
					Part.push_back( PartObject( PartType, PartSkin, PartPosX, PartPosY, PartProp, PartFlip ) );
					return;
				}
			}
		}
	}
	if( !TriedPlace ) Part.push_back( PartObject( PartType, PartSkin, PartPosX, PartPosY, PartProp, PartFlip ) );
}

void GZBPLE::World::Delete_Index( int Index )
{
	if( Index >= 0 && Index < Part.size() )
	{
		Part.erase( Part.begin() + Index );
	}
}

void GZBPLE::World::Delete_Type( int PartType, int Method = 0, int Mode = 0 )
{
	bool Confirm = false;
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		switch( Mode )
		{
			case 0: Confirm = true; break;
			case 1: Confirm = Part.at( Index ).Selected;
			case 2: Confirm = !Part.at( Index ).Selected;
		}
		if( Part.at( Index ).Type == PartType && Confirm )
		{
			switch( Method )
			{
				case 0:
				{
					Part.erase( Part.begin() + Index );
				} break;
				case 1:
				{
					if( Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				case 2:
				{
					if( !Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				default: return;
			}
		}
	}
}

void GZBPLE::World::Delete_Type_Skin( int PartType, int PartSkin, int Method = 0, int Mode = 0 )
{
	bool Confirm = false;
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		switch( Mode )
		{
			case 0: Confirm = true; break;
			case 1: Confirm = Part.at( Index ).Selected;
			case 2: Confirm = !Part.at( Index ).Selected;
		}
		if( Part.at( Index ).Type == PartType && Part.at( Index ).Skin == PartSkin && Confirm )
		{
			switch( Method )
			{
				case 0:
				{
					Part.erase( Part.begin() + Index );
				} break;
				case 1:
				{
					if( Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				case 2:
				{
					if( !Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				default: return;
			}
		}
	}
}

void GZBPLE::World::Delete_Type_Skin_Prop( int PartType, int PartSkin, int PartProp, int Method = 0, int Mode = 0 )
{
	bool Confirm = false;
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		switch( Mode )
		{
			case 0: Confirm = true; break;
			case 1: Confirm = Part.at( Index ).Selected;
			case 2: Confirm = !Part.at( Index ).Selected;
		}
		if( Part.at( Index ).Type == PartType && Part.at( Index ).Skin == PartSkin && Part.at( Index ).Prop == PartProp && Confirm )
		{
			switch( Method )
			{
				case 0:
				{
					Part.erase( Part.begin() + Index );
				} break;
				case 1:
				{
					if( Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				case 2:
				{
					if( !Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				default: return;
			}
		}
	}
}

void GZBPLE::World::Delete_Type_Skin_Flip( int PartType, int PartSkin, int PartFlip, int Method = 0, int Mode = 0 )
{
	bool Confirm = false;
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		switch( Mode )
		{
			case 0: Confirm = true; break;
			case 1: Confirm = Part.at( Index ).Selected;
			case 2: Confirm = !Part.at( Index ).Selected;
		}
		if( Part.at( Index ).Type == PartType && Part.at( Index ).Skin == PartSkin && Part.at( Index ).Flip == PartFlip && Confirm )
		{
			switch( Method )
			{
				case 0:
				{
					Part.erase( Part.begin() + Index );
				} break;
				case 1:
				{
					if( Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				case 2:
				{
					if( !Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				default: return;
			}
		}
	}
}

void GZBPLE::World::Delete_Type_Skin_Prop_Flip( int PartType, int PartSkin, int PartProp, int PartFlip, int Method = 0, int Mode = 0 )
{
	bool Confirm = false;
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		switch( Mode )
		{
			case 0: Confirm = true; break;
			case 1: Confirm = Part.at( Index ).Selected;
			case 2: Confirm = !Part.at( Index ).Selected;
		}
		if( Part.at( Index ).Type == PartType && Part.at( Index ).Skin == PartSkin && Part.at( Index ).Prop == PartProp && Part.at( Index ).Flip == PartFlip && Confirm )
		{
			switch( Method )
			{
				case 0:
				{
					Part.erase( Part.begin() + Index );
				} break;
				case 1:
				{
					if( Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				case 2:
				{
					if( !Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				default: return;
			}
		}
	}
}

void GZBPLE::World::Delete_Pos( int PartPosX, int PartPosY, int Method = 0, int Mode = 0 )
{
	bool Confirm = false;
	int TriedDisplaceIndex = -1;
	for( int Index = 0; Index < Part.size(); Index++ )
	{
		switch( Mode )
		{
			case 0: Confirm = true; break;
			case 1: Confirm = Part.at( Index ).Selected;
			case 2: Confirm = !Part.at( Index ).Selected;
		}
		if( Part.at( Index ).PosX == PartPosX && Part.at( Index ).PosY == PartPosY && Confirm )
		{
			switch( Method )
			{
				case 0:
				{
					Part.erase( Part.begin() + Index );
				} break;
				case 1:
				{
					if( Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				case 2:
				{
					if( !Part.at( Index ).Frame ) Part.erase( Part.begin() + Index );
				} break;
				case 3:
				{
					if( Part.at( Index ).Frame )
					{
						Part.erase( Part.begin() + Index );
						return;
					}
					else TriedDisplaceIndex = Index;
				} break;
				case 4:
				{
					if( !Part.at( Index ).Frame )
					{
						Part.erase( Part.begin() + Index );
						return;
					}
					else TriedDisplaceIndex = Index;
				} break;
				default: return;
			}
		}
	}
	if( TriedDisplaceIndex != -1 && ( Method == 3 || Method == 4 ) )
	{
		Part.erase( Part.begin() + TriedDisplaceIndex );
	}
}

// End World Functions