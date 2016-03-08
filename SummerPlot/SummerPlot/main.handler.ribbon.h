#pragma once

#include <UIRibbon.h>
#include <UIRibbonPropertyHelpers.h>

#include <string>
#include <algorithm>

#include "ribbon.h"
#include "main.controls.h"
#include "main.util.drawing.h"
#include "main.util.dialog.h"
#include "main.util.export.h"
#include "main.util.doublebuffer.h"
#include "main.util.colors.h"

using namespace std;

std::wstring GetFileExtension(const std::wstring & FileName)
{
    if (FileName.find_last_of(L".") != std::wstring::npos)
        return FileName.substr(FileName.find_last_of(L".") + 1);
    return L"";
}

HRESULT ribbonOnUpdateProperty(UINT nCmdID, REFPROPERTYKEY key, const PROPVARIANT* pCurrent, PROPVARIANT* pNew)
{
    switch (nCmdID)
    {
    case IDC_CMD_EDIT_EXPRESSION:
    {
        if (key == UI_PKEY_Enabled)
        {
            UIInitPropertyFromBoolean(key, (opt_selectedExp != -1), pNew);
            return S_OK;
        }
        break;
    }
    case IDC_CMD_REMOVE_EXPRESSION:
    {
        if (key == UI_PKEY_Enabled)
        {
            UIInitPropertyFromBoolean(key, (opt_selectedExp != -1), pNew);
            return S_OK;
        }
        break;
    }
    case IDC_CMD_SET_EXPRESSION_COLOR:
    {
        if (key == UI_PKEY_Enabled)
        {
            UIInitPropertyFromBoolean(key, (opt_selectedExp != -1), pNew);
            return S_OK;
        }
        else if (key == UI_PKEY_ColorType)
        {
            if (opt_selectedExp >= 0 && opt_selectedExp < (int)opt_expressions.size())
            {
                UIInitPropertyFromUInt32(key, UI_SWATCHCOLORTYPE_RGB, pNew);
                return S_OK;
            }
            else
            {
                UIInitPropertyFromUInt32(key, UI_SWATCHCOLORTYPE_NOCOLOR, pNew);
                return S_OK;
            }
        }
        else if (key == UI_PKEY_Color)
        {
            if (opt_selectedExp >= 0 && opt_selectedExp < (int)opt_expressions.size())
            {
                UIInitPropertyFromUInt32(key, opt_expressions[opt_selectedExp]->color, pNew);
                return S_OK;
            }
        }
        break;
    }
    case IDC_CMD_SHOW_GRID:
    {
        if (key == UI_PKEY_BooleanValue)
        {
            UIInitPropertyFromBoolean(key, opt_gridEnabled, pNew);
            return S_OK;
        }
        break;
    }
    case IDC_CMD_GRID_SPACING:
    {
        if (key == UI_PKEY_DecimalValue)
        {
            DECIMAL dec;
            VarDecFromI4(opt_gridSpacing, &dec);
            UIInitPropertyFromDecimal(key, dec, pNew);
            return S_OK;
        }
        else if (key == UI_PKEY_DecimalPlaces)
        {
            UIInitPropertyFromUInt32(key, 0, pNew);
            return S_OK;
        }
        else if (key == UI_PKEY_MinValue)
        {
            DECIMAL dec;
            VarDecFromI4(1, &dec);
            UIInitPropertyFromDecimal(key, dec, pNew);
            return S_OK;
        }
        break;
    }
    case IDC_CMD_SHOW_AXIS:
    {
        if (key == UI_PKEY_BooleanValue)
        {
            UIInitPropertyFromBoolean(key, opt_axisEnabled, pNew);
            return S_OK;
        }
        break;
    }
    case IDC_CMD_TICK_SPACING:
    {
        if (key == UI_PKEY_DecimalValue)
        {
            DECIMAL dec;
            VarDecFromI4(opt_tickSpacing, &dec);
            UIInitPropertyFromDecimal(key, dec, pNew);
            return S_OK;
        }
        else if (key == UI_PKEY_DecimalPlaces)
        {
            UIInitPropertyFromUInt32(key, 0, pNew);
            return S_OK;
        }
        else if (key == UI_PKEY_MinValue)
        {
            DECIMAL dec;
            VarDecFromI4(1, &dec);
            UIInitPropertyFromDecimal(key, dec, pNew);
            return S_OK;
        }
        break;
    }
    case IDC_CMD_SET_BACKGROUND_COLOR:
    {
        if (key == UI_PKEY_ColorType)
        {
            UIInitPropertyFromUInt32(key, UI_SWATCHCOLORTYPE_RGB, pNew);
            return S_OK;
        }
        else if (key == UI_PKEY_Color)
        {
            UIInitPropertyFromUInt32(key, opt_backgroundColor, pNew);
            return S_OK;
        }
        break;
    }
    }
    return E_NOTIMPL;
}

void ribbonOnExecute(UINT nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* pValue, IUISimplePropertySet* pCommandExecutionProperties)
{
    switch (nCmdID)
    {
    case IDC_CMD_ADD_EXPRESSION:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            LVITEM item = { 0 };
            item.mask = LVIF_TEXT;
            item.pszText = L"";
            item.iItem = opt_expressions.size();

            int index = ListView_InsertItem(hListview, &item);

            DWORD color = random_colors[rand() % random_colors.size()];
            opt_expressions.push_back(new Expression(L"", color));

            SetFocus(hListview);
            ListView_SetItemState(hListview, -1, 0, LVIS_SELECTED);
            ListView_EditLabel(hListview, index);
            return;
        }
        break;
    }
    case IDC_CMD_EDIT_EXPRESSION:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            if (opt_selectedExp >= 0 && opt_selectedExp < (int)opt_expressions.size())
            {
                SetFocus(hListview);
                ListView_EditLabel(hListview, opt_selectedExp);
                return;
            }
        }
        break;
    }
    case IDC_CMD_REMOVE_EXPRESSION:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            if (opt_selectedExp >= 0 && opt_selectedExp < (int)opt_expressions.size())
            {
                opt_expressions.erase(opt_expressions.begin() + opt_selectedExp);
                ListView_DeleteItem(hListview, opt_selectedExp);
                opt_selectedExp = -1;
                drawPlot();
                return;
            }
        }
        break;
    }
    case IDC_CMD_EXPORT:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            wstring filePath = showSaveDialog(L"PNG\0*.png\0JPG\0*.jpg\0BMP\0*.bmp\0All\0*.*\0", L"png");
            if (filePath.size() > 0)
            {
                wstring extension = GetFileExtension(filePath);
                transform(extension.begin(), extension.end(), extension.begin(), tolower);

                wstring format;
                if (extension == L"png")
                {
                    format = L"image/png";
                }
                else if (extension == L"jpg")
                {
                    format = L"image/jpeg";
                }
                else if (extension == L"bmp")
                {
                    format = L"image/bmp";
                }
                else
                {
                    format = L"image/png";
                }

                exportHBITMAP(hBitmapMemory, format.c_str(), filePath.c_str());

                MessageBoxW(hWindow, L"Image saved!", L"SummerPlot", MB_ICONINFORMATION);
            }
            return;
        }
        break;
    }
    case IDC_CMD_EXIT:
    {
        PostQuitMessage(0);
        break;
    }
    case IDC_CMD_SHOW_GRID:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            PROPVARIANT var;
            HRESULT hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_BooleanValue, &var);
            if (!FAILED(hr))
            {
                BOOL val;
                hr = PropVariantToBoolean(var, &val);
                opt_gridEnabled = (val == TRUE);
                drawPlot();
                return;
            }
        }
        break;
    }
    case IDC_CMD_SHOW_AXIS:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            PROPVARIANT var;
            HRESULT hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_BooleanValue, &var);
            if (!FAILED(hr))
            {
                BOOL val;
                hr = PropVariantToBoolean(var, &val);
                opt_axisEnabled = (val == TRUE);
                drawPlot();
                return;
            }
        }
        break;
    }
    case IDC_CMD_SET_EXPRESSION_COLOR:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            if (opt_selectedExp >= 0 && opt_selectedExp < (int)opt_expressions.size())
            {
                PROPVARIANT var;
                HRESULT hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_ColorType, &var);
                if (!FAILED(hr) && var.uintVal == UI_SWATCHCOLORTYPE_RGB)
                {
                    hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_Color, &var);
                    if (!FAILED(hr))
                    {
                        opt_expressions[opt_selectedExp]->setColor(var.uintVal);
                        drawPlot();
                        return;
                    }
                }
            }
        }
        break;
    }
    case IDC_CMD_SET_BACKGROUND_COLOR:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            PROPVARIANT var;
            HRESULT hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_ColorType, &var);
            if (!FAILED(hr) && var.uintVal == UI_SWATCHCOLORTYPE_RGB)
            {
                hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_Color, &var);
                if (!FAILED(hr))
                {
                    opt_backgroundColor = var.uintVal;
                    drawPlot();
                    return;
                }
            }
        }
        break;
    }
    case IDC_CMD_GRID_SPACING:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            PROPVARIANT var;
            HRESULT hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_DecimalValue, &var);
            if (!FAILED(hr))
            {
                LONG val;
                hr = VarI4FromDec(&var.decVal, &val);
                if (!FAILED(hr))
                {
                    opt_gridSpacing = val;
                    drawPlot();
                    return;
                }
            }
        }
        break;
    }
    case IDC_CMD_TICK_SPACING:
    {
        if (verb == UI_EXECUTIONVERB_EXECUTE)
        {
            PROPVARIANT var;
            HRESULT hr = ribbonFramework->GetUICommandProperty(nCmdID, UI_PKEY_DecimalValue, &var);
            if (!FAILED(hr))
            {
                LONG val;
                hr = VarI4FromDec(&var.decVal, &val);
                if (!FAILED(hr))
                {
                    opt_tickSpacing = val;
                    drawPlot();
                    return;
                }
            }
        }
        break;
    }
    }
}