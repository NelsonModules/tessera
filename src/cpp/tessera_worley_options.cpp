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
#include <algorithm>
#include <cmath>
//=============================================================================
namespace {
//=============================================================================
inline unsigned int
hash2(int x, int y, unsigned int seed)
{
    unsigned int h = static_cast<unsigned int>(x) * 374761393u
        + static_cast<unsigned int>(y) * 668265263u + seed * 362437u;
    h = (h ^ (h >> 13)) * 1274126177u;
    return h ^ (h >> 16);
}
//=============================================================================
inline double
rand01(unsigned int h)
{
    return static_cast<double>(h & 0xFFFFFFu) / static_cast<double>(0x1000000u);
}
//=============================================================================
inline double
metricDistance(double dx, double dy, int metric)
{
    if (metric == 1) {
        return std::fabs(dx) + std::fabs(dy);
    }
    if (metric == 2) {
        return std::max(std::fabs(dx), std::fabs(dy));
    }
    return std::sqrt(dx * dx + dy * dy);
}
//=============================================================================
inline double
clamp01(double v)
{
    if (v < 0.0) {
        return 0.0;
    }
    if (v > 1.0) {
        return 1.0;
    }
    return v;
}
//=============================================================================
} // namespace
//=============================================================================
void
tessera_worley_options(int rows, int cols, int cells, unsigned int seed, int feature, int metric,
    int seamless, double* out)
{
    if (cells < 1) {
        cells = 1;
    }
    if (feature < 1 || feature > 3) {
        feature = 1;
    }
    if (metric < 0 || metric > 2) {
        metric = 0;
    }
    for (int c = 0; c < cols; ++c) {
        double fx = (static_cast<double>(c) / (seamless && cols > 1 ? cols - 1 : cols)) * cells;
        int cx = static_cast<int>(std::floor(fx));
        for (int r = 0; r < rows; ++r) {
            double fy = (static_cast<double>(r) / (seamless && rows > 1 ? rows - 1 : rows)) * cells;
            int cy = static_cast<int>(std::floor(fy));
            double best1 = 1.0e300;
            double best2 = 1.0e300;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int gx = cx + dx;
                    int gy = cy + dy;
                    int hx = gx;
                    int hy = gy;
                    if (seamless) {
                        hx = ((gx % cells) + cells) % cells;
                        hy = ((gy % cells) + cells) % cells;
                    }
                    unsigned int h = hash2(hx, hy, seed);
                    double px = gx + rand01(h);
                    double py = gy + rand01(h * 2246822519u + 1u);
                    double ex = px - fx;
                    double ey = py - fy;
                    if (seamless) {
                        if (ex > cells * 0.5) {
                            ex -= cells;
                        } else if (ex < -cells * 0.5) {
                            ex += cells;
                        }
                        if (ey > cells * 0.5) {
                            ey -= cells;
                        } else if (ey < -cells * 0.5) {
                            ey += cells;
                        }
                    }
                    double d = metricDistance(ex, ey, metric);
                    if (d < best1) {
                        best2 = best1;
                        best1 = d;
                    } else if (d < best2) {
                        best2 = d;
                    }
                }
            }
            double value = best1;
            if (feature == 2) {
                value = best2;
            } else if (feature == 3) {
                value = best2 - best1;
            }
            out[r + c * rows] = clamp01(value);
        }
    }
}
//=============================================================================
