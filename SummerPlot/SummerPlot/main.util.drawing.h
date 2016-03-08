#pragma once

#include <windows.h>
#include <string>
#include <sstream>
#include <iomanip>

#include "main.status.h"
#include "main.options.h"
#include "main.util.translate.h"
#include "main.util.doublebuffer.h"

using namespace std;

HFONT hFontLabel;
HBRUSH hBrushLabel;
HPEN hPenAxis;
HPEN hPenGrid;

int _ticksSize;
int _ticks[2000];
double _tickLabels[2000];

int _gridSize;
int _grid[2000];

void _getTicks(
    _Out_ int * length,
    _Out_ int * ticks,
    _Out_ double * tickLabels,
    _In_ int tickSpacing,
    _In_ int minDistance,
    _In_ bool vertical,
    _In_ bool autoResize = true
    )
{
    int rangeMin, rangeMax;
    int len = 0;
    double tickSpacingPx;

    if (vertical)
    {
        rangeMin = int(floor(opt_yMin));
        rangeMax = int(ceil(opt_yMax));
        tickSpacingPx = toScreenH(tickSpacing);
    }
    else
    {
        rangeMin = int(floor(opt_xMin));
        rangeMax = int(ceil(opt_xMax));
        tickSpacingPx = toScreenW(tickSpacing);
    }

    if (tickSpacingPx == 0)
    {
        *length = 0;
        return;
    }

    int tickCount = ((rangeMax - rangeMin + 1) / tickSpacing) + 1;

    if (!autoResize && tickSpacingPx < minDistance)
    {
        *length = 0;
        return;
    }

    int scale = (int)max(1, double(minDistance) / tickSpacingPx);
    scale = (int)pow(4, ceil(log(scale) / log(4)));
    tickSpacing *= scale;

    for (int p = rangeMin; p < rangeMax; ++p)
    {
        if (p % int(tickSpacing) == 0)
        {
            int pos = int(vertical ? toScreenY(p) : toScreenX(p));
            if (tickLabels) tickLabels[len] = double(p);
            ticks[len++] = pos;
        }
    }

    *length = len;
}

void createAxisFont()
{
    LOGFONT font = { 0 };
    font.lfHeight = -MulDiv(10, GetDeviceCaps(hDCMemory, LOGPIXELSY), 72);
    font.lfWeight = FW_REGULAR;
    font.lfQuality = CLEARTYPE_QUALITY;
    font.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
    lstrcpyW(font.lfFaceName, L"Verdana");
    hFontLabel = CreateFontIndirect(&font);
}

void _drawClear()
{
    SetBkColor(hDCMemory, opt_backgroundColor);
    RECT rect = { 0, 0, canvasWidth, canvasHeight };
    ExtTextOutW(hDCMemory, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

void _drawAxis(bool vertical)
{
    int linePos = vertical ? int(toScreenX(0)) : int(toScreenY(0));

    _getTicks(&_ticksSize, _ticks, _tickLabels, opt_tickSpacing, 50, vertical, true);
    if (_ticksSize == 0) return;
    if (_ticksSize > 2000) _ticksSize = 2000;

    HGDIOBJ oldPen = SelectObject(hDCMemory, hPenAxis);
    HGDIOBJ oldFont = SelectObject(hDCMemory, hFontLabel);

    if (vertical)
    {
        MoveToEx(hDCMemory, linePos, 0, NULL);
        LineTo(hDCMemory, linePos, canvasHeight);
    }
    else
    {
        MoveToEx(hDCMemory, 0, linePos, NULL);
        LineTo(hDCMemory, canvasWidth, linePos);
    }

    int tickLowerBound = linePos - 10;
    int tickUpperBound = linePos + 10;

    for (int i = 0; i < _ticksSize; ++i)
    {
        wstringstream ss;
        ss << setprecision(6) << _tickLabels[i];

        wstring label = ss.str();

        int tickPosition = _ticks[i];

        if (vertical)
        {
            MoveToEx(hDCMemory, tickLowerBound, tickPosition, NULL);
            LineTo(hDCMemory, tickUpperBound, tickPosition);
            TextOutW(hDCMemory, linePos + 2, tickPosition + 2, label.data(), label.size());
        }
        else
        {
            MoveToEx(hDCMemory, tickPosition, tickLowerBound, NULL);
            LineTo(hDCMemory, tickPosition, tickUpperBound);
            if (abs(_tickLabels[i]) > 0.0000001) TextOutW(hDCMemory, tickPosition + 2, linePos + 2, label.data(), label.size());
        }
    }

    SelectObject(hDCMemory, oldPen);
    SelectObject(hDCMemory, oldFont);
}

void _drawGrid(bool vertical)
{
    _getTicks(&_gridSize, _grid, NULL, opt_gridSpacing, 20, vertical, true);
    if (_gridSize == 0) return;
    if (_gridSize > 2000) _gridSize = 2000;

    HGDIOBJ oldPen = SelectObject(hDCMemory, hPenGrid);

    for (int i = 0; i < _gridSize; ++i)
    {
        int tickPosition = _grid[i];

        if (vertical)
        {
            MoveToEx(hDCMemory, 0, tickPosition, NULL);
            LineTo(hDCMemory, canvasWidth, tickPosition);
        }
        else
        {
            MoveToEx(hDCMemory, tickPosition, 0, NULL);
            LineTo(hDCMemory, tickPosition, canvasHeight);
        }
    }

    SelectObject(hDCMemory, oldPen);
}

void _drawExpression()
{
    POINT * points = new POINT[canvasWidth];

    for (auto & ex : opt_expressions)
    {
        if (!ex->ok) continue;

        HGDIOBJ oldPen = SelectObject(hDCMemory, ex->hPen);

        for (int i = 0; i < canvasWidth; i++)
        {
            points[i].x = i;
            points[i].y = static_cast<LONG>(toScreenY(ex->execute(toMathX(i))));
        }

        Polyline(hDCMemory, &points[0], canvasWidth);

        SelectObject(hDCMemory, oldPen);
    }

    delete[] points;
}

void drawPlot()
{
    if (!hFontLabel)
    {
        createAxisFont();
        hBrushLabel = CreateSolidBrush(0x444444);
        hPenAxis = CreatePen(PS_SOLID, 2, 0x444444);
        hPenGrid = CreatePen(PS_SOLID, 2, 0xDDDDDD);
    }

    _drawClear();

    if (opt_gridEnabled)
    {
        _drawGrid(true);
        _drawGrid(false);
    }
    if (opt_axisEnabled)
    {
        _drawAxis(true);
        _drawAxis(false);
    }

    _drawExpression();
    refresh();
}