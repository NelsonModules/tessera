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
//=============================================================================
// Chaos game: kind 0 = Barnsley fern, 1 = Sierpinski triangle.
void
tessera_chaos_game(int kind, int n, unsigned int seed, double* xs, double* ys)
{
    unsigned int state = seed ? seed : 1u;
    auto u01 = [&]() -> double {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return static_cast<double>(state & 0xFFFFFFu) / static_cast<double>(0x1000000u);
    };

    double x = 0.0;
    double y = 0.0;
    const double vx[3] = { 0.0, 1.0, 0.5 };
    const double vy[3] = { 0.0, 0.0, 0.8660254037844386 };

    auto step = [&]() {
        if (kind == 1) {
            int v = static_cast<int>(u01() * 3.0);
            if (v > 2) {
                v = 2;
            }
            x = (x + vx[v]) * 0.5;
            y = (y + vy[v]) * 0.5;
        } else {
            double p = u01();
            double nx, ny;
            if (p < 0.01) {
                nx = 0.0;
                ny = 0.16 * y;
            } else if (p < 0.86) {
                nx = 0.85 * x + 0.04 * y;
                ny = -0.04 * x + 0.85 * y + 1.6;
            } else if (p < 0.93) {
                nx = 0.2 * x - 0.26 * y;
                ny = 0.23 * x + 0.22 * y + 1.6;
            } else {
                nx = -0.15 * x + 0.28 * y;
                ny = 0.26 * x + 0.24 * y + 0.44;
            }
            x = nx;
            y = ny;
        }
    };

    for (int i = 0; i < 20; ++i) {
        step();
    }
    for (int i = 0; i < n; ++i) {
        step();
        xs[i] = x;
        ys[i] = y;
    }
}
//=============================================================================
