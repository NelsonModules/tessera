//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "tessera.hpp"
#include <cmath>
#include <limits>
#include <vector>
//=============================================================================
int
tessera_turtle(const char* commands, int length, double angleDeg, double step, double x0, double y0,
    double headingDeg, double* xs, double* ys, int maxPoints)
{
    if (maxPoints <= 0) {
        return 0;
    }
    const double deg = 3.141592653589793 / 180.0;
    const double angle = angleDeg * deg;
    const double nan = std::numeric_limits<double>::quiet_NaN();

    double x = x0;
    double y = y0;
    double heading = headingDeg * deg;
    std::vector<double> sx;
    std::vector<double> sy;
    std::vector<double> sh;

    int count = 0;
    auto emit = [&](double px, double py) {
        if (count < maxPoints) {
            xs[count] = px;
            ys[count] = py;
            ++count;
        }
    };
    emit(x, y);

    for (int i = 0; i < length && count < maxPoints; ++i) {
        char c = commands[i];
        switch (c) {
        case 'F':
        case 'G':
            x += step * std::cos(heading);
            y += step * std::sin(heading);
            emit(x, y);
            break;
        case 'f':
            x += step * std::cos(heading);
            y += step * std::sin(heading);
            emit(nan, nan);
            emit(x, y);
            break;
        case '+':
            heading += angle;
            break;
        case '-':
            heading -= angle;
            break;
        case '[':
            sx.push_back(x);
            sy.push_back(y);
            sh.push_back(heading);
            break;
        case ']':
            if (!sx.empty()) {
                x = sx.back();
                y = sy.back();
                heading = sh.back();
                sx.pop_back();
                sy.pop_back();
                sh.pop_back();
                emit(nan, nan);
                emit(x, y);
            }
            break;
        default:
            break;
        }
    }
    return count;
}
//=============================================================================
