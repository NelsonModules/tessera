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
#include <vector>
//=============================================================================
namespace {
//=============================================================================
struct Point {
    double x;
    double y;
};
//=============================================================================
inline unsigned int
nextState(unsigned int& state)
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}
//=============================================================================
inline double
u01(unsigned int& state)
{
    return static_cast<double>(nextState(state) & 0xFFFFFFu) / static_cast<double>(0x1000000u);
}
//=============================================================================
inline size_t
idx(int row, int col, int rows)
{
    return static_cast<size_t>(row) + static_cast<size_t>(col) * rows;
}
//=============================================================================
} // namespace
//=============================================================================
void
tessera_lloyd_voronoi(int rows, int cols, int cells, int iterations, unsigned int seed, double* out,
    double* outSites)
{
    if (cells < 1) {
        cells = 1;
    }
    if (iterations < 0) {
        iterations = 0;
    }
    unsigned int state = seed ? seed : 1u;
    std::vector<Point> points(static_cast<size_t>(cells));
    for (int i = 0; i < cells; ++i) {
        points[i] = { u01(state) * cols, u01(state) * rows };
    }
    std::vector<int> count(static_cast<size_t>(cells));
    std::vector<double> sumX(static_cast<size_t>(cells));
    std::vector<double> sumY(static_cast<size_t>(cells));
    for (int it = 0; it < iterations; ++it) {
        std::fill(count.begin(), count.end(), 0);
        std::fill(sumX.begin(), sumX.end(), 0.0);
        std::fill(sumY.begin(), sumY.end(), 0.0);
        for (int c = 0; c < cols; ++c) {
            for (int r = 0; r < rows; ++r) {
                int best = 0;
                double bestD = 1.0e300;
                for (int i = 0; i < cells; ++i) {
                    double dx = points[i].x - c;
                    double dy = points[i].y - r;
                    double d = dx * dx + dy * dy;
                    if (d < bestD) {
                        bestD = d;
                        best = i;
                    }
                }
                count[best]++;
                sumX[best] += c;
                sumY[best] += r;
            }
        }
        for (int i = 0; i < cells; ++i) {
            if (count[i] > 0) {
                points[i].x = sumX[i] / count[i];
                points[i].y = sumY[i] / count[i];
            }
        }
    }
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            int best = 0;
            double bestD = 1.0e300;
            for (int i = 0; i < cells; ++i) {
                double dx = points[i].x - c;
                double dy = points[i].y - r;
                double d = dx * dx + dy * dy;
                if (d < bestD) {
                    bestD = d;
                    best = i;
                }
            }
            out[idx(r, c, rows)] = static_cast<double>(best + 1);
        }
    }
    if (outSites != nullptr) {
        for (int i = 0; i < cells; ++i) {
            outSites[i] = points[i].x + 1.0;
            outSites[i + cells] = points[i].y + 1.0;
        }
    }
}
//=============================================================================
void
tessera_dla(int rows, int cols, int particles, unsigned int seed, const double* obstacles,
    const double* seeds, int seedCount, double directionY, double directionX, int periodic,
    double* out)
{
    if (particles < 1) {
        particles = 1;
    }
    for (int i = 0; i < rows * cols; ++i) {
        out[i] = obstacles != nullptr && obstacles[i] > 0.0 ? 1.0 : 0.0;
    }
    int cr = rows / 2;
    int cc = cols / 2;
    int placed = 0;
    if (seeds != nullptr && seedCount > 0) {
        for (int i = 0; i < seedCount; ++i) {
            int sr = static_cast<int>(std::round(seeds[i])) - 1;
            int sc = static_cast<int>(std::round(seeds[i + seedCount])) - 1;
            if (sr >= 0 && sr < rows && sc >= 0 && sc < cols && out[idx(sr, sc, rows)] == 0.0) {
                out[idx(sr, sc, rows)] = 1.0;
                placed++;
            }
        }
    }
    if (placed == 0) {
        out[idx(cr, cc, rows)] = 1.0;
        placed = 1;
    }
    unsigned int state = seed ? seed : 1u;
    int radius = std::max(4, std::min(rows, cols) / 3);
    const int maxSteps = std::max(rows, cols) * std::max(rows, cols);
    const double directionLength = std::sqrt(directionY * directionY + directionX * directionX);
    if (directionLength > 0.0) {
        directionY /= directionLength;
        directionX /= directionLength;
    }
    auto wrap = [](int value, int limit) {
        value %= limit;
        return value < 0 ? value + limit : value;
    };
    while (placed < particles && placed < rows * cols) {
        double angle = u01(state) * 6.283185307179586;
        int r = cr + static_cast<int>(std::round(std::sin(angle) * radius));
        int c = cc + static_cast<int>(std::round(std::cos(angle) * radius));
        if (periodic != 0) {
            r = wrap(r, rows);
            c = wrap(c, cols);
        } else {
            r = std::max(1, std::min(rows - 2, r));
            c = std::max(1, std::min(cols - 2, c));
        }
        for (int step = 0; step < maxSteps; ++step) {
            if (directionLength > 0.0 && u01(state) < 0.35) {
                r += directionY > 0.15 ? 1 : (directionY < -0.15 ? -1 : 0);
                c += directionX > 0.15 ? 1 : (directionX < -0.15 ? -1 : 0);
            } else {
                int dir = static_cast<int>(nextState(state) % 4u);
                if (dir == 0) {
                    r--;
                } else if (dir == 1) {
                    r++;
                } else if (dir == 2) {
                    c--;
                } else {
                    c++;
                }
            }
            if (periodic != 0) {
                r = wrap(r, rows);
                c = wrap(c, cols);
            } else if (r <= 0 || c <= 0 || r >= rows - 1 || c >= cols - 1) {
                break;
            }
            int rm = periodic != 0 ? wrap(r - 1, rows) : r - 1;
            int rp = periodic != 0 ? wrap(r + 1, rows) : r + 1;
            int cm = periodic != 0 ? wrap(c - 1, cols) : c - 1;
            int cp = periodic != 0 ? wrap(c + 1, cols) : c + 1;
            bool touching = out[idx(rm, c, rows)] > 0.0 || out[idx(rp, c, rows)] > 0.0
                || out[idx(r, cm, rows)] > 0.0 || out[idx(r, cp, rows)] > 0.0;
            if (touching) {
                if (out[idx(r, c, rows)] == 0.0) {
                    out[idx(r, c, rows)] = 1.0;
                    placed++;
                }
                break;
            }
        }
    }
}
//=============================================================================
