/// @file
///
/// @author    Anstro Pleuton
/// @copyright Copyright (c) 2025 Anstro Pleuton
///
/// Crystal GUI - A GUI framework for raylib.
///
/// This example file collects many components for a grand display.
///
/// This project is licensed under the terms of MIT license.

#include <string.h>

#include "crystalgui/crystalgui.h"
#include "raylib.h"
#include "raymath.h"

bool isDarkTheme         = true;
bool isHighContrastTheme = false;

CguiTheme *darkTheme              = NULL;
CguiTheme *lightTheme             = NULL;
CguiTheme *darkHighContrastTheme  = NULL;
CguiTheme *lightHighContrastTheme = NULL;

bool drawDebugBounds = false;

void SetTheme(bool light, bool high)
{
    isDarkTheme         = !light;
    isHighContrastTheme = high;
    if (isHighContrastTheme)
    {
        if (isDarkTheme)
            CguiSetActiveTheme(darkHighContrastTheme);
        else
            CguiSetActiveTheme(lightHighContrastTheme);
    }
    else
    {
        if (isDarkTheme)
            CguiSetActiveTheme(darkTheme);
        else
            CguiSetActiveTheme(lightTheme);
    }
}

void SetDarkLightTheme(CguiNode *node, bool light)
{
    SetTheme(light, isHighContrastTheme);
}

void SetHighLowTheme(CguiNode *node, bool high)
{
    SetTheme(!isDarkTheme, high);
}

void SetDrawDebugBounds(CguiNode *node, bool debug)
{
    drawDebugBounds = debug;
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(1600, 800, "Control Test Suite");

    CguiInit();

    darkTheme              = CguiCreateCrystallineThemeDark();
    lightTheme             = CguiCreateCrystallineThemeLight();
    darkHighContrastTheme  = CguiCreateCrystallineThemeDarkHighContrast();
    lightHighContrastTheme = CguiCreateCrystallineThemeLightHighContrast();

    CguiSetActiveTheme(darkTheme);

    // clang-format off
    CguiNode *themeButtons = CG_LINEAR(CguiTFillParent(), CGUI_LAYOUT_DIRECTION_X, CGUI_LAYOUT_JUSTIFY_END, 20.0f,
        CG_LINEAR_ITEM(0.0f, 200.0f, 200.0f,
            CG_LINEAR_X_BEGIN(CguiTFillParent(), 10.0f,
                CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                    CG_SWITCH(CguiTFillParent(), false, SetDarkLightTheme, false, NULL)
                ),
                CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                    CG_LABEL_BODY(CguiTFillParent(), "Light Theme", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_CENTER, NULL)
                )
            )
        ),
        CG_LINEAR_ITEM(0.0f, 200.0f, 200.0f,
            CG_LINEAR_X_BEGIN(CguiTFillParent(), 10.0f,
                CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                    CG_SWITCH(CguiTFillParent(), false, SetHighLowTheme, false, NULL)
                ),
                CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                    CG_LABEL_BODY(CguiTFillParent(), "High Contrast", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_CENTER, NULL)
                )
            )
        ),
        CG_LINEAR_ITEM(0.0f, 200.0f, 200.0f,
            CG_LINEAR_X_BEGIN(CguiTFillParent(), 10.0f,
                CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                    CG_SWITCH(CguiTFillParent(), false, SetDrawDebugBounds, false, NULL)
                ),
                CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                    CG_LABEL_BODY(CguiTFillParent(), "Draw Debug", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_CENTER, NULL)
                )
            )
        )
    );

    CguiNode *topbar = CG_EMPTY(CguiTFillParent(),
        CG_EMPTY(CguiTMargin(5.0f),
            CG_EMPTY(CguiTFillParent(),
                CG_EMPTY(CguiTMargin(5.0f),
                    themeButtons
                )
            )
        )
    );

    CguiNode *labelSection = CG_LINEAR(CguiTMargin(5.0f), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_BEGIN, 10.0f,
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LABEL_HEADING(CguiTFillParent(), "Heading: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_BEGIN, NULL)
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LABEL_BODY(CguiTFillParent(), "Body: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_BEGIN, NULL)
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LABEL_CAPTION(CguiTFillParent(), "Caption: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_BEGIN, NULL)
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LABEL_HEADING(CguiTFillParent(), "Disabled Heading: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", true, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_BEGIN, NULL)
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LABEL_BODY(CguiTFillParent(), "Disabled Body: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", true, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_BEGIN, NULL)
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LABEL_CAPTION(CguiTFillParent(), "Disabled Caption: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", true, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_BEGIN, NULL)
        )
    );

    CguiNode *buttonSection = CG_LINEAR(CguiTMargin(5.0f), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_BEGIN, 10.0f,
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_NORMAL(CguiTFillParent(), NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Normal Button", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_FLAT(CguiTFillParent(), NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Flat Button", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_ACCENT(CguiTFillParent(), NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Accent Button", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_DANGEROUS(CguiTFillParent(), NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Dangerous Button", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_WARNING(CguiTFillParent(), NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Warning Button", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_SUCCESS(CguiTFillParent(), NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Success Button", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_INFO(CguiTFillParent(), NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Info Button", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_NORMAL(CguiTFillParent(), NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Normal Button", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_FLAT(CguiTFillParent(), NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Flat Button", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_ACCENT(CguiTFillParent(), NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Accent Button", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_DANGEROUS(CguiTFillParent(), NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Dangerous Button", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_WARNING(CguiTFillParent(), NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Warning Button", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_SUCCESS(CguiTFillParent(), NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Success Button", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_BUTTON_INFO(CguiTFillParent(), NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Info Button", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        )
    );

    CguiNode *otherSection = CG_LINEAR(CguiTMargin(5.0f), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_BEGIN, 10.0f,
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_TOGGLE(CguiTFillParent(), false, NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Toggle", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_TOGGLE(CguiTFillParent(), true, NULL, false,
                CG_LABEL_BODY(CguiTFillParent(), "Toggle", false, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_TOGGLE(CguiTFillParent(), false, NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Toggle", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_TOGGLE(CguiTFillParent(), true, NULL, true,
                CG_LABEL_BODY(CguiTFillParent(), "Disabled Toggle", true, CGUI_TEXT_JUSTIFY_CENTER, CGUI_TEXT_JUSTIFY_CENTER, NULL)
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LINEAR_X_BEGIN(CguiTFillParent(), 10.0f,
                CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                    CG_SWITCH(CguiTFillParent(), false, NULL, false, NULL)
                ),
                CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                    CG_LABEL_BODY(CguiTFillParent(), "Switch", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_CENTER, NULL)
                )
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LINEAR_X_BEGIN(CguiTFillParent(), 10.0f,
                CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                    CG_SWITCH(CguiTFillParent(), false, NULL, true, NULL)
                ),
                CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                    CG_LABEL_BODY(CguiTFillParent(), "Disabled Switch", true, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_CENTER, NULL)
                )
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LINEAR_X_BEGIN(CguiTFillParent(), 10.0f,
                CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                    CG_CHECK(CguiTFillParent(), false, NULL, false, NULL)
                ),
                CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                    CG_LABEL_BODY(CguiTFillParent(), "Check", false, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_CENTER, NULL)
                )
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            CG_LINEAR_X_BEGIN(CguiTFillParent(), 10.0f,
                CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                    CG_CHECK(CguiTFillParent(), false, NULL, true, NULL)
                ),
                CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                    CG_LABEL_BODY(CguiTFillParent(), "Disabled Check", true, CGUI_TEXT_JUSTIFY_BEGIN, CGUI_TEXT_JUSTIFY_CENTER, NULL)
                )
            )
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            NULL
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            NULL
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            NULL
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            NULL
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            NULL
        ),
        CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
            NULL
        )
    );

    CguiNode *mainContent = CG_FRONTLAYER(CguiTFillParent(),
        CG_LINEAR(CguiTMargin(5.0f), CGUI_LAYOUT_DIRECTION_X, CGUI_LAYOUT_JUSTIFY_BEGIN, 10.0f,
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    labelSection
                )
            ),
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    buttonSection
                )
            ),
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    otherSection
                )
            ),
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    NULL
                )
            ),
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    NULL
                )
            ),
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    NULL
                )
            ),
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    NULL
                )
            ),
            CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                CG_EMPTY(CguiTFillParent(),
                    NULL
                )
            )
        )
    );

    CguiNode *root = CG_ROOT(
        CG_EMPTY(CguiTMargin(5.0f),
            CG_EMPTY(CguiTMargin(5.0f),
                CG_LINEAR(CguiTFillParent(), CGUI_LAYOUT_DIRECTION_Y, CGUI_LAYOUT_JUSTIFY_BEGIN, 10.0f,
                    CG_LINEAR_ITEM(0.0f, 50.0f, 50.0f,
                        topbar
                    ),
                    CG_LINEAR_ITEM(1.0f, 0.0f, INFINITY,
                        mainContent
                    )
                )
            )
        )
    );
    // clang-format on

    while (!WindowShouldClose())
    {
        CguiUpdate(root);

        BeginDrawing();
        ClearBackground(BLANK);

        CguiDraw(root, drawDebugBounds);

        EndDrawing();
    }

    CguiDeleteNode(root);

    CguiDeleteTheme(darkTheme);
    CguiDeleteTheme(lightTheme);
    CguiDeleteTheme(darkHighContrastTheme);
    CguiDeleteTheme(lightHighContrastTheme);

    CguiClose();

    CloseWindow();
}
