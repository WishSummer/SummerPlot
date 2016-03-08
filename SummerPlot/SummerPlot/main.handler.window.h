#pragma once

#include <windows.h>
#include <Uxtheme.h>
#include <CommCtrl.h>

#include "main.controls.h"
#include "main.status.h"
#include "main.util.ribbon.h"
#include "main.util.doublebuffer.h"
#include "main.handler.canvas.h"
#include "main.options.h"

void onWindowResize()
{
    RECT rect;
    GetClientRect(hWindow, &rect);
    OffsetRect(&rect, -rect.left, -rect.top);

    if (rect.right == 0 || rect.bottom == 0) return;

    UINT32 ribbonHeight;
    ribbon->GetHeight(&ribbonHeight);
    rect.top += ribbonHeight;

    canvasWidth = rect.right - 200;
    canvasHeight = rect.bottom - rect.top;

    HDWP dwp = BeginDeferWindowPos(2);
    DeferWindowPos(dwp, hListview, NULL, 0, rect.top, 200, rect.bottom - rect.top, NULL);
    DeferWindowPos(dwp, hCanvas, NULL, 200, rect.top, canvasWidth, rect.bottom - rect.top, NULL);
    EndDeferWindowPos(dwp);

    resetDoubleBuffer();
    drawPlot();
}

void onWindowInitialized()
{
    hListview = GetDlgItem(hWindow, IDC_LIST_EXPRESSIONS);
    hCanvas = GetDlgItem(hWindow, IDC_DRAW);
    hDCCanvas = GetDC(hCanvas);

    SetWindowSubclass(hCanvas, CanvasProc, 0, NULL);

    DWORD lvStyles = LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT;
    ListView_SetExtendedListViewStyleEx(hListview, lvStyles, lvStyles);
    SetWindowTheme(hListview, L"Explorer", NULL);

    LVCOLUMN col = { 0 };
    col.mask = LVCF_WIDTH | LVCF_TEXT;
    col.cx = 195;
    col.pszText = L"Expressions";
    ListView_InsertColumn(hListview, 0, &col);

    onWindowResize();
}

INT_PTR CALLBACK DialogProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        ribbonInit(hWnd);
        hWindow = hWnd;
        onWindowInitialized();
        return TRUE;
    }
    case WM_ERASEBKGND:
    {
        return TRUE;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc;
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_RIBBON_RESIZE:
    case WM_SIZE:
    case WM_SIZING:
    {
        onWindowResize();
        return FALSE;
    }
    case WM_CLOSE:
        DestroyWindow(hWindow);
        return TRUE;

    case WM_DESTROY:
        ribbonDestroy();
        PostQuitMessage(0);
        return TRUE;

    case WM_NOTIFY:
    {
        switch (((LPNMHDR)lParam)->code)
        {
        case LVN_ENDLABELEDIT:
        {
            NMLVDISPINFO * pdi = (NMLVDISPINFO*)lParam;
            if (!pdi->item.pszText) break;

            if (pdi->item.iItem >= 0 && pdi->item.iItem < (int)opt_expressions.size())
            {
                DWORD color = opt_expressions[pdi->item.iItem]->color;
                delete opt_expressions[pdi->item.iItem];
                opt_expressions[pdi->item.iItem] = new Expression(
                    wstring(pdi->item.pszText),
                    color
                    );
                drawPlot();
                SetWindowLongW(hWindow, DWL_MSGRESULT, TRUE);
                return TRUE;
            }

            break;
        }
        case LVN_ITEMCHANGED:
        {
            int selected = ListView_GetNextItem(hListview, -1, LVNI_SELECTED);
            if (selected >= (int)opt_expressions.size()) selected = -1;
            opt_selectedExp = selected;

            ribbonFramework->InvalidateUICommand(IDC_CMD_EDIT_EXPRESSION, UI_INVALIDATIONS_STATE, nullptr);
            ribbonFramework->InvalidateUICommand(IDC_CMD_REMOVE_EXPRESSION, UI_INVALIDATIONS_STATE, nullptr);
            ribbonFramework->InvalidateUICommand(IDC_CMD_SET_EXPRESSION_COLOR, UI_INVALIDATIONS_STATE, nullptr);

            break;
        }
        }
        break;
    }
    }

    return FALSE;
}
