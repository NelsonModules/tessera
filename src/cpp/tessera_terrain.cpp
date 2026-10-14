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
#include <vector>
//=============================================================================
void
tessera_diamond_square(int n, double roughness, unsigned int seed, double* out)
{
    if (n < 1) {
        n = 1;
    }
    const int size = (1 << n) + 1;
    const int wrap = size - 1;
    std::vector<double> h(static_cast<size_t>(size) * size, 0.0);
    auto at = [&](int x, int y) -> double& { return h[static_cast<size_t>(y) * size + x]; };

    unsigned int state = seed ? seed : 1u;
    auto nextRandom = [&]() -> double {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return (static_cast<double>(state) / 4294967295.0) - 0.5;
    };

    double corner = nextRandom();
    at(0, 0) = corner;
    at(wrap, 0) = corner;
    at(0, wrap) = corner;
    at(wrap, wrap) = corner;

    double scale = 1.0;
    for (int side = wrap; side >= 2; side /= 2) {
        int half = side / 2;
        for (int x = 0; x + side < size; x += side) {
            for (int y = 0; y + side < size; y += side) {
                double avg = (at(x, y) + at(x + side, y) + at(x, y + side) + at(x + side, y + side))
                    * 0.25;
                at(x + half, y + half) = avg + nextRandom() * scale;
            }
        }
        for (int x = 0; x < size; x += half) {
            for (int y = (x + half) % side; y < size; y += side) {
                double avg = (at((x - half + wrap) % wrap, y) + at((x + half) % wrap, y)
                                 + at(x, (y + half) % wrap) + at(x, (y - half + wrap) % wrap))
                    * 0.25;
                double value = avg + nextRandom() * scale;
                at(x, y) = value;
                if (x == 0) {
                    at(wrap, y) = value;
                }
                if (y == 0) {
                    at(x, wrap) = value;
                }
            }
        }
        scale *= std::pow(2.0, -roughness);
    }

    double lo = h[0];
    double hi = h[0];
    for (double v : h) {
        if (v < lo) {
            lo = v;
        }
        if (v > hi) {
            hi = v;
        }
    }
    double span = (hi > lo) ? (hi - lo) : 1.0;
    for (int c = 0; c < size; ++c) {
        for (int r = 0; r < size; ++r) {
            out[r + c * size] = (at(c, r) - lo) / span;
        }
    }
}
//=============================================================================
