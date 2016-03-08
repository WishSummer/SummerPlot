#pragma once

#include "main.options.h"

#pragma region Translate Functions

double toScreenW(double w)
{
    return w / (opt_xMax - opt_xMin) * canvasWidth;
}

double toScreenH(double h)
{
    return h / (opt_yMax - opt_yMin) * canvasHeight;
}

double toScreenX(double x)
{
    return (x - opt_xMin) * canvasWidth / (opt_xMax - opt_xMin);
}

double toScreenY(double y)
{
    return canvasHeight - ((y - opt_yMin) * canvasHeight / (opt_yMax - opt_yMin));
}

double toMathX(double x)
{
    return x * (opt_xMax - opt_xMin) / canvasWidth + opt_xMin;
}

double toMathY(double y)
{
    return (canvasHeight - y) * (opt_yMax - opt_yMin) / canvasHeight + opt_yMin;
}

double toMathW(double w)
{
    return (w / canvasWidth) * (opt_xMax - opt_xMin);
}

double toMathH(double h)
{
    return (h / canvasHeight) * (opt_yMax - opt_yMin);
}
#pragma endregion
