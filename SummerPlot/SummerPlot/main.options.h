#pragma once

#include "Expression.h"

#include <vector>

using namespace std;

#pragma region Options
double opt_yMax = 5;
double opt_yMin = -5;
double opt_xMax = 20;
double opt_xMin = -20;
bool opt_gridEnabled = true;
int opt_gridSpacing = 1;
bool opt_axisEnabled = true;
int opt_tickSpacing = 1;
DWORD opt_backgroundColor = 0xFFFFFF;
vector<Expression *> opt_expressions;
int opt_selectedExp = -1;
#pragma endregion
