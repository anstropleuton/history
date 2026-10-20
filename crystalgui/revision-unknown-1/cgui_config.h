/*******************************************************************************
 *
 *  Crystal GUI - Crystal-Clear Modern-Looking GUI Library for raylib!
 *
 *  Copyright (c) 2023 Anstro Pleuton (@anstropleuton)
 *
 *  License: MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sub-license, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 *
 ******************************************************************************/

#ifndef CGUI_CONFIG_H
#define CGUI_CONFIG_H

// NOTE: No do-while used because the __cguiAfterLog can be a break statement
#define CGUI_LOG_CONDITION(__cguiCondition, __cguiAfterLog, __cguiType, ...) \
    if (__cguiCondition)                                                     \
    {                                                                        \
        CGUI_LOG(__cguiType, __VA_ARGS__);                                   \
        __cguiAfterLog;                                                      \
    }

// NOTE: Everything else has do-while because they do not break (pun intended)
#define CGUI_CHECK_VALID_LOG(__cguiModule, __cguiValue, __cguiErrorMessage, __cguiBeforeReturn, __cguiReturnValue) \
    do                                                                                                             \
    {                                                                                                              \
        CGUI_LOG_CONDITION(!__cguiValue, __cguiBeforeReturn; return __cguiReturnValue, LOG_WARNING,                \
                                                                    "CGUI: "__cguiModule                           \
                                                                    ": "__cguiErrorMessage);                       \
    } while (0)

#define CGUI_CHECK_VALID(__cguiValue, __cguiBeforeReturn, __cguiReturnValue) \
    do                                                                       \
    {                                                                        \
        if (!__cguiValue)                                                    \
        {                                                                    \
            __cguiBeforeReturn;                                              \
            return __cguiReturnValue;                                        \
        }                                                                    \
    } while (0)

#define CGUI_LOG_ID(__cguiModule, __cguiId, __cguiSuccessMessage) \
    do                                                            \
    {                                                             \
        CGUI_LOG(LOG_INFO,                                        \
                 "CGUI: "__cguiModule                             \
                 ": [ID %i] "__cguiSuccessMessage,                \
                 __cguiId);                                       \
    } while (0)

#define CGUI_LOG_NOID(__cguiModule, __cguiSuccessMessage) \
    do                                                    \
    {                                                     \
        CGUI_LOG(LOG_INFO,                                \
                 "CGUI: "__cguiModule                     \
                 ": "__cguiSuccessMessage);               \
    } while (0)

// raylib macro
#define SMOOTH_CIRCLE_ERROR_RATE 0.5f

// Memory allocated to text has a minimum of and is multiple of this unit size
#define CGUI_TEXT_EDITOR_TEXT_UNIT_LENGTH 256

#define CGUI_TEXT_EDITOR_UNDO_HISTORY_LIMIT 10000

#endif // CGUI_CONFIG_H
