#pragma once

#include "main.controls.h"
#include "main.status.h"

HDC hDCMemory = 0;
HBITMAP hBitmapMemory = 0;
HGDIOBJ hBitmapOld = 0;

void destroyDoubleBuffer()
{
    if (hBitmapMemory)
    {
        SelectObject(hDCMemory, hBitmapOld);
        DeleteObject(hBitmapMemory);
        DeleteDC(hDCMemory);
        hBitmapOld = 0;
        hBitmapMemory = 0;
        hDCMemory = 0;
    }
}

void resetDoubleBuffer()
{
    destroyDoubleBuffer();
    hDCMemory = CreateCompatibleDC(hDCCanvas);
    hBitmapMemory = CreateCompatibleBitmap(hDCCanvas, canvasWidth, canvasHeight);
    hBitmapOld = SelectObject(hDCMemory, hBitmapMemory);
}

void onCanvasPaint()
{
    if (hBitmapMemory)
    {
        BitBlt(hDCCanvas, 0, 0, canvasWidth, canvasHeight, hDCMemory, 0, 0, SRCCOPY);
    }
}

void refresh()
{
    InvalidateRect(hCanvas, NULL, FALSE);
}
