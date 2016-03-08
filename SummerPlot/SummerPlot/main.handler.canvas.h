#pragma once

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#include "main.controls.h"
#include "main.status.h"
#include "main.util.translate.h"
#include "main.util.doublebuffer.h"
#include "main.util.drawing.h"

int lastX, lastY;
bool isMouseDown = false;

LRESULT CALLBACK CanvasProc(
    HWND hWnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam,
    UINT_PTR uIdSubclass,
    DWORD_PTR dwRefData
    )
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        onCanvasPaint();
        break;
    }
    case WM_LBUTTONDOWN:
    {
        SetCapture(hCanvas);
        lastX = GET_X_LPARAM(lParam);
        lastY = GET_Y_LPARAM(lParam);
        isMouseDown = true;
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (isMouseDown)
        {
            int currentX = GET_X_LPARAM(lParam);
            int currentY = GET_Y_LPARAM(lParam);
            double xMove = toMathX(lastX) - toMathX(currentX);
            double yMove = toMathY(lastY) - toMathY(currentY);
            opt_xMin = opt_xMin + xMove;
            opt_xMax = opt_xMax + xMove;
            opt_yMin = opt_yMin + yMove;
            opt_yMax = opt_yMax + yMove;
            lastX = currentX;
            lastY = currentY;
            drawPlot();
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        isMouseDown = false;
        ReleaseCapture();
        break;
    }
    case WM_NCHITTEST:
    {
        return HTCLIENT;
    }
    case WM_MOUSEWHEEL:
    {
        int delta = -GET_WHEEL_DELTA_WPARAM(wParam);
        POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hWnd, &pt);

        double x = toMathX(pt.x);
        double y = toMathY(pt.y);

        double scale = 1.0;
        if (delta > 0)
        {
            scale = 1.0 + ((double)delta / 240.0);
        }
        else if (delta < 0)
        {
            scale = 1.0 / (1 - (double)delta / 240.0);
        }

        double w = opt_xMax - opt_xMin;
        double h = opt_yMax - opt_yMin;
        double ofsx = (x - opt_xMin) / w;
        double ofsy = (y - opt_yMin) / h;
        opt_xMin = opt_xMin + w * ofsx * (1 - scale);
        opt_xMax = opt_xMin + w * scale;
        opt_yMin = opt_yMin + h * ofsy * (1 - scale);
        opt_yMax = opt_yMin + h * scale;
        drawPlot();
        break;
    }
    }
    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}