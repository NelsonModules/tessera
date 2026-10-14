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
namespace {
double
haltonRadicalInverse(int index, int base)
{
    double f = 1.0;
    double result = 0.0;
    while (index > 0) {
        f /= base;
        result += f * (index % base);
        index /= base;
    }
    return result;
}
} // namespace
//=============================================================================
void
tessera_halton(int n, int dims, double* out)
{
    static const int primes[16]
        = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53 };
    for (int d = 0; d < dims; ++d) {
        int base = primes[d];
        for (int i = 0; i < n; ++i) {
            out[i + static_cast<size_t>(d) * n] = haltonRadicalInverse(i + 1, base);
        }
    }
}
//=============================================================================
int
tessera_poisson_disk(double width, double height, double radius, int k, unsigned int seed,
    double* xs, double* ys, int maxPoints)
{
    if (width <= 0.0 || height <= 0.0 || radius <= 0.0 || maxPoints <= 0) {
        return 0;
    }
    if (k < 1) {
        k = 1;
    }
    const double cell = radius / std::sqrt(2.0);
    const int gw = static_cast<int>(std::ceil(width / cell));
    const int gh = static_cast<int>(std::ceil(height / cell));
    std::vector<int> grid(static_cast<size_t>(gw) * gh, -1);
    std::vector<double> px;
    std::vector<double> py;
    std::vector<int> active;

    unsigned int state = seed ? seed : 1u;
    auto u01 = [&]() -> double {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return static_cast<double>(state & 0xFFFFFFu) / static_cast<double>(0x1000000u);
    };

    auto gridIndex = [&](double x, double y) -> int {
        int gx = static_cast<int>(x / cell);
        int gy = static_cast<int>(y / cell);
        if (gx < 0) {
            gx = 0;
        }
        if (gy < 0) {
            gy = 0;
        }
        if (gx >= gw) {
            gx = gw - 1;
        }
        if (gy >= gh) {
            gy = gh - 1;
        }
        return gy * gw + gx;
    };

    auto fits = [&](double x, double y) -> bool {
        int gx = static_cast<int>(x / cell);
        int gy = static_cast<int>(y / cell);
        for (int iy = gy - 2; iy <= gy + 2; ++iy) {
            for (int ix = gx - 2; ix <= gx + 2; ++ix) {
                if (ix < 0 || iy < 0 || ix >= gw || iy >= gh) {
                    continue;
                }
                int s = grid[static_cast<size_t>(iy) * gw + ix];
                if (s >= 0) {
                    double dx = px[s] - x;
                    double dy = py[s] - y;
                    if (dx * dx + dy * dy < radius * radius) {
                        return false;
                    }
                }
            }
        }
        return true;
    };

    double x0 = u01() * width;
    double y0 = u01() * height;
    px.push_back(x0);
    py.push_back(y0);
    grid[gridIndex(x0, y0)] = 0;
    active.push_back(0);

    while (!active.empty() && static_cast<int>(px.size()) < maxPoints) {
        int aIndex = static_cast<int>(u01() * active.size());
        if (aIndex >= static_cast<int>(active.size())) {
            aIndex = static_cast<int>(active.size()) - 1;
        }
        int sample = active[aIndex];
        bool placed = false;
        for (int attempt = 0; attempt < k; ++attempt) {
            double angle = u01() * 6.283185307179586;
            double dist = radius * (1.0 + u01());
            double nx = px[sample] + std::cos(angle) * dist;
            double ny = py[sample] + std::sin(angle) * dist;
            if (nx < 0.0 || ny < 0.0 || nx >= width || ny >= height) {
                continue;
            }
            if (fits(nx, ny)) {
                int id = static_cast<int>(px.size());
                px.push_back(nx);
                py.push_back(ny);
                grid[gridIndex(nx, ny)] = id;
                active.push_back(id);
                placed = true;
                break;
            }
        }
        if (!placed) {
            active[aIndex] = active.back();
            active.pop_back();
        }
    }

    int count = static_cast<int>(px.size());
    if (count > maxPoints) {
        count = maxPoints;
    }
    for (int i = 0; i < count; ++i) {
        xs[i] = px[i];
        ys[i] = py[i];
    }
    return count;
}
//=============================================================================
