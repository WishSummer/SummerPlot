#pragma once

#include <windows.h>
#include <UIRibbon.h>

#pragma region Controls
HWND hWindow = 0;
HWND hListview = 0;
HWND hCanvas = 0;
HDC hDCCanvas = 0;
#pragma endregion

#pragma region Ribbon
IUIFramework     * ribbonFramework;
IUIApplication   * ribbonApp;
IUIRibbon        * ribbon;
#pragma endregion
