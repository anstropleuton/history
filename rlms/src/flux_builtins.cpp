/**
 *  @author  Anstro Pleuton (https://github.com/anstropleuton)
 *  @brief   Implementation for builtin functions and constants from
 *           @c flux.hpp .
 *
 *  @copyright  Copyright (c) 2024 Anstro Pleuton
 *
 *   _____ _
 *  |  ___| |_   ___  __
 *  | |_  | | | | \ \/ /
 *  |  _| | | |_| |>  <
 *  |_|   |_|\__,_/_/\_\
 *
 *  Flux is an expression-storing data type that evaluates the expression as a
 *  float when required its value.
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


#include <algorithm>
#include <cfenv>
#include <cmath>
#include <numbers>
#include <numeric>
#include <stdexcept>

#include "flux.hpp"

/**
 *  @brief  Append some built-in functions to @c functions .
 */
auto flux::add_builtin_functions() -> void
{
    functions["abs"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for abs");
        }
        // (x)
        return std::abs(params[0]);
    };

    functions["acos"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for acos");
        }
        // (x)
        return std::acos(params[0]);
    };

    functions["acosh"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for acosh");
        }
        // (x)
        return std::acosh(params[0]);
    };

    ///  @todo  @c add_sat has not been added yet.  Uncomment this when C++26
    // releases and GCC implements it.
    /*
     *   functions["add_sat"] = [&](std::vector<float> params)
     *   {
     *    if (params.size() != 2)
     *    {
     *        throw std::runtime_error("Invalid parameters for add_sat");
     *    }
     *    // (x, y)
     *    return std::add_sat(params[0], params[1]);
     *   };
     */

    functions["assoc_laguerre"] = [&](std::vector<float> params)
    {
        if (params.size() != 3)
        {
            throw std::runtime_error("Invalid parameters for assoc_laguerre");
        }
        // (x, n, alpha)
        return std::assoc_laguerre(params[0], (int)std::round(params[1]),
            (int)std::round(params[2]));
    };

    functions["assoc_legendre"] = [&](std::vector<float> params)
    {
        if (params.size() != 3)
        {
            throw std::runtime_error("Invalid parameters for assoc_legendre");
        }
        // (l, m, x)
        return std::assoc_legendre((int)std::round(params[0]),
            (int)std::round(params[1]), params[2]);
    };

    functions["asin"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for asin");
        }
        // (x)
        return std::asin(params[0]);
    };

    functions["asinh"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for asinh");
        }
        // (x)
        return std::asinh(params[0]);
    };

    functions["atan"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for atan");
        }
        // (x)
        return std::atan(params[0]);
    };

    functions["atan2"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for atan2");
        }
        // (y, x)
        return std::atan2(params[0], params[1]);
    };

    functions["atanh"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for atanh");
        }
        // (x)
        return std::atanh(params[0]);
    };

    functions["beta"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for beta");
        }
        // (x, y)
        return std::beta(params[0], params[1]);
    };

    functions["ceil"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for ceil");
        }
        // (x)
        return std::ceil(params[0]);
    };

    functions["cbrt"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for cbrt");
        }
        // (x)
        return std::cbrt(params[0]);
    };

    functions["comp_ellint_1"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for comp_ellint_1");
        }
        // (x)
        return std::comp_ellint_1(params[0]);
    };

    functions["comp_ellint_2"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for comp_ellint_2");
        }
        // (x)
        return std::comp_ellint_2(params[0]);
    };

    functions["comp_ellint_3"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for comp_ellint_3");
        }
        // (x, y)
        return std::comp_ellint_3(params[0], params[1]);
    };

    functions["cos"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for cos");
        }
        // (x)
        return std::cos(params[0]);
    };

    functions["cosh"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for cosh");
        }
        // (x)
        return std::cosh(params[0]);
    };

    functions["cyl_bessel_i"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for cyl_bessel_i");
        }
        // (x, y)
        return std::cyl_bessel_i(params[0], params[1]);
    };

    functions["cyl_bessel_j"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for cyl_bessel_j");
        }
        // (x, y)
        return std::cyl_bessel_j(params[0], params[1]);
    };

    functions["cyl_bessel_k"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for cyl_bessel_k");
        }
        // (x, y)
        return std::cyl_bessel_k(params[0], params[1]);
    };

    functions["cyl_neumann"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for cyl_neumann");
        }
        // (x, y)
        return std::cyl_neumann(params[0], params[1]);
    };

    functions["dim"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for dim");
        }
        // (x, y)
        return std::fdim(params[0], params[1]);
    };

    ///  @todo  @c div_sat has not been added yet.  Uncomment this when C++26
    // releases and GCC implements it.
    /*
     *   functions["div_sat"] = [&](std::vector<float> params)
     *   {
     *    if (params.size() != 2)
     *    {
     *        throw std::runtime_error("Invalid parameters for div_sat");
     *    }
     *    // (x, y)
     *    return std::div_sat(params[0], params[1]);
     *   };
     */

    functions["ellint_1"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for ellint_1");
        }
        // (phi, k)
        return std::ellint_1(params[0], params[1]);
    };

    functions["ellint_2"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for ellint_2");
        }
        // (phi, k)
        return std::ellint_2(params[0], params[1]);
    };

    functions["ellint_3"] = [&](std::vector<float> params)
    {
        if (params.size() != 3)
        {
            throw std::runtime_error("Invalid parameters for ellint_3");
        }
        // (phi, k, n)
        return std::ellint_3(params[0], params[1], params[2]);
    };

    functions["erf"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for erf");
        }
        // (x)
        return std::erf(params[0]);
    };

    functions["erfc"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for erfc");
        }
        // (x)
        return std::erfc(params[0]);
    };

    functions["exp"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for exp");
        }
        // (x)
        return std::exp(params[0]);
    };

    functions["exp2"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for exp2");
        }
        // (x)
        return std::exp2(params[0]);
    };

    functions["expint"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for expint");
        }
        // (x)
        return std::expint(params[0]);
    };

    functions["expm1"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for expm1");
        }
        // (x)
        return std::expm1(params[0]);
    };

    functions["fegetround"] = [&](std::vector<float> params)
    {
        if (params.size() != 0)
        {
            throw std::runtime_error("Invalid parameters for fegetround");
        }
        // ()
        return (float)std::fegetround();
    };

    functions["fesetround"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for fesetround");
        }
        // (mode) where mode:
        // 1 => FE_DOWNWARD,
        // 2 => FE_TONEAREST,
        // 3 => FE_TOWARDZERO,
        // 4 => FE_UPWARD
        int mode = (int)std::round(params[0]);
        int result;

        switch (mode)
        {
            case 1: result = std::fesetround(FE_DOWNWARD); break;
            case 2: result = std::fesetround(FE_TONEAREST); break;
            case 3: result = std::fesetround(FE_TOWARDZERO); break;
            case 4: result = std::fesetround(FE_UPWARD); break;
            default:
            {
                throw std::runtime_error("Invalid rounding mode");
            }
        }
        return (float)result;
    };

    functions["fma"] = [&](std::vector<float> params)
    {
        if (params.size() != 3)
        {
            throw std::runtime_error("Invalid parameters for fma");
        }
        // (x, y, z)
        return std::fma(params[0], params[1], params[2]);
    };

    functions["floor"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for floor");
        }
        // (x)
        return std::floor(params[0]);
    };

    functions["gcd"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for gcd");
        }
        // (x, y)
        return (float)std::gcd((int)std::round(params[0]),
            (int)std::round(params[1]));
    };

    functions["hermite"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for hermite");
        }
        // (n, x)
        return std::hermite((int)std::round(params[0]), params[1]);
    };

    functions["hypot"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for hypot");
        }
        // (x, y)
        return std::hypot(params[0], params[1]);
    };

    functions["laguerre"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for laguerre");
        }
        // (n, x)
        return std::laguerre((int)std::round(params[0]), params[1]);
    };

    functions["legendre"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for legendre");
        }
        // (l, x)
        return std::legendre((int)std::round(params[0]), params[1]);
    };

    functions["lgamma"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for lgamma");
        }
        // (x)
        return std::lgamma(params[0]);
    };

    functions["lcm"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for lcm");
        }
        // (x, y)
        return (float)std::lcm((int)std::round(params[0]),
            (int)std::round(params[1]));
    };

    functions["lerp"] = [&](std::vector<float> params)
    {
        if (params.size() != 3)
        {
            throw std::runtime_error("Invalid parameters for lerp");
        }
        // (start, end, t)
        return std::lerp(params[0], params[1], params[2]);
    };

    functions["log"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for log");
        }
        // (x)
        return std::log(params[0]);
    };

    functions["log1p"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for log1p");
        }
        // (x)
        return std::log1p(params[0]);
    };

    functions["log10"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for log10");
        }
        // (x)
        return std::log10(params[0]);
    };

    functions["log2"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for log2");
        }
        // (x)
        return std::log2(params[0]);
    };

    functions["max"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for max");
        }
        // (x, y)
        return std::ranges::max(params);
    };

    functions["midpoint"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for midpoint");
        }
        // (x, y)
        return std::midpoint(params[0], params[1]);
    };

    functions["min"] = [&](std::vector<float> params)
    {
        // (x, y)
        return std::ranges::min(params);
    };

    functions["mod"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for mod");
        }
        // (x, y)
        return std::fmod(params[0], params[1]);
    };

    ///  @todo  @c mul_sat has not been added yet.  Uncomment this when C++26
    // releases and GCC implements it.
    /*
     *   functions["mul_sat"] = [&](std::vector<float> params)
     *   {
     *    if (params.size() != 2)
     *    {
     *        throw std::runtime_error("Invalid parameters for mul_sat");
     *    }
     *    // (x, y)
     *    return std::mul_sat(params[0], params[1]);
     *   };
     */

    functions["nan"] = [&](std::vector<float> params)
    {
        if (!params.empty())
        {
            throw std::runtime_error("Invalid parameters for nan");
        }
        // ()
        return std::nanf("");
    };

    functions["nearbyint"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for nearbyint");
        }
        // (x)
        return std::nearbyint(params[0]);
    };

    functions["pow"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for pow");
        }
        // (x, y)
        return std::pow(params[0], params[1]);
    };

    functions["remainder"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for remainder");
        }
        // (x, y)
        return std::remainder(params[0], params[1]);
    };

    functions["riemann_zeta"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for riemann_zeta");
        }
        // (x)
        return std::riemann_zeta(params[0]);
    };

    functions["rint"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for rint");
        }
        // (x)
        return std::rint(params[0]);
    };

    functions["round"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for round");
        }
        // (x)
        return std::round(params[0]);
    };

    ///  @todo  @c sub_sat has not been added yet.  Uncomment this when C++26
    // releases and GCC implements it.
    /*
     *   functions["sub_sat"] = [&](std::vector<float> params)
     *   {
     *    if (params.size() != 2)
     *    {
     *        throw std::runtime_error("Invalid parameters for sub_sat");
     *    }
     *    // (x, y)
     *    return std::sub_sat(params[0], params[1]);
     *   };
     */

    functions["sin"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for sin");
        }
        // (x)
        return std::sin(params[0]);
    };

    functions["sinh"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for sinh");
        }
        // (x)
        return std::sinh(params[0]);
    };

    functions["sph_bessel"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for sph_bessel");
        }
        // (n, x)
        return std::sph_bessel((int)std::round(params[0]), params[1]);
    };

    functions["sph_legendre"] = [&](std::vector<float> params)
    {
        if (params.size() != 3)
        {
            throw std::runtime_error("Invalid parameters for sph_legendre");
        }
        // (l, m, x)
        return std::sph_legendre((int)std::round(params[0]),
            (int)std::round(params[1]), params[2]);
    };

    functions["sph_neumann"] = [&](std::vector<float> params)
    {
        if (params.size() != 2)
        {
            throw std::runtime_error("Invalid parameters for sph_neumann");
        }
        // (n, x)
        return std::sph_neumann((int)std::round(params[0]), params[1]);
    };

    functions["sqrt"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for sqrt");
        }
        // (x)
        return std::sqrt(params[0]);
    };

    functions["tan"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for tan");
        }
        // (x)
        return std::tan(params[0]);
    };

    functions["tanh"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for tanh");
        }
        // (x)
        return std::tanh(params[0]);
    };

    functions["tgamma"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for tgamma");
        }
        // (x)
        return std::tgamma(params[0]);
    };

    functions["trunc"] = [&](std::vector<float> params)
    {
        if (params.size() != 1)
        {
            throw std::runtime_error("Invalid parameters for trunc");
        }
        // (x)
        return std::trunc(params[0]);
    };
}

/**
 *  @brief  Append some built-in constants to @c constants .
 */
auto flux::add_builtin_constants() -> void
{
    constants["e"]          = std::numbers::e;
    constants["log2e"]      = std::numbers::log2e;
    constants["log10e"]     = std::numbers::log10e;
    constants["pi"]         = std::numbers::pi;
    constants["inv_pi"]     = std::numbers::inv_pi;
    constants["inv_sqrtpi"] = std::numbers::inv_sqrtpi;
    constants["ln2"]        = std::numbers::ln2;
    constants["ln10"]       = std::numbers::ln10;
    constants["sqrt2"]      = std::numbers::sqrt2;
    constants["sqrt3"]      = std::numbers::sqrt3;
    constants["inv_sqrt3"]  = std::numbers::inv_sqrt3;
    constants["egamma"]     = std::numbers::egamma;
    constants["phi"]        = std::numbers::phi;
}
