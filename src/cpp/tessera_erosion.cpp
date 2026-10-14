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
// Droplet-based hydraulic erosion (Hans/Lague style): water droplets descend
// the gradient, eroding steep terrain and depositing sediment where they slow.
void
tessera_erode(const double* in, int rows, int cols, int droplets, unsigned int seed, double* out)
{
    const size_t n = static_cast<size_t>(rows) * cols;
    std::vector<double> H(in, in + n);
    auto at = [&](int r, int c) -> double& { return H[static_cast<size_t>(r) + static_cast<size_t>(c) * rows]; };

    unsigned int state = seed ? seed : 1u;
    auto u01 = [&]() -> double {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return static_cast<double>(state & 0xFFFFFFu) / static_cast<double>(0x1000000u);
    };

    const double inertia = 0.05;
    const double capacityFactor = 4.0;
    const double minSlope = 0.01;
    const double deposition = 0.3;
    const double erosion = 0.3;
    const double evaporation = 0.01;
    const double gravity = 4.0;
    const int maxLifetime = 30;

    if (droplets < 0) {
        droplets = 0;
    }
    for (int d = 0; d < droplets; ++d) {
        double x = u01() * (cols - 1);
        double y = u01() * (rows - 1);
        double dx = 0.0, dy = 0.0;
        double speed = 1.0, water = 1.0, sediment = 0.0;

        for (int life = 0; life < maxLifetime; ++life) {
            int nx = static_cast<int>(x);
            int ny = static_cast<int>(y);
            if (nx < 0 || ny < 0 || nx >= cols - 1 || ny >= rows - 1) {
                break;
            }
            double u = x - nx;
            double v = y - ny;
            double h00 = at(ny, nx);
            double h10 = at(ny, nx + 1);
            double h01 = at(ny + 1, nx);
            double h11 = at(ny + 1, nx + 1);
            double gx = (h10 - h00) * (1.0 - v) + (h11 - h01) * v;
            double gy = (h01 - h00) * (1.0 - u) + (h11 - h10) * u;
            double oldHeight
                = h00 * (1 - u) * (1 - v) + h10 * u * (1 - v) + h01 * (1 - u) * v + h11 * u * v;

            dx = dx * inertia - gx * (1.0 - inertia);
            dy = dy * inertia - gy * (1.0 - inertia);
            double len = std::sqrt(dx * dx + dy * dy);
            if (len < 1.0e-8) {
                break;
            }
            dx /= len;
            dy /= len;
            x += dx;
            y += dy;
            if (x < 0.0 || y < 0.0 || x >= cols - 1 || y >= rows - 1) {
                break;
            }

            int mx = static_cast<int>(x);
            int my = static_cast<int>(y);
            double mu = x - mx;
            double mv = y - my;
            double newHeight = at(my, mx) * (1 - mu) * (1 - mv) + at(my, mx + 1) * mu * (1 - mv)
                + at(my + 1, mx) * (1 - mu) * mv + at(my + 1, mx + 1) * mu * mv;
            double deltaH = newHeight - oldHeight;

            double capacity = std::fmax(-deltaH, minSlope) * speed * water * capacityFactor;
            if (sediment > capacity || deltaH > 0.0) {
                double drop = (deltaH > 0.0) ? std::fmin(deltaH, sediment)
                                             : (sediment - capacity) * deposition;
                sediment -= drop;
                at(ny, nx) += drop * (1 - u) * (1 - v);
                at(ny, nx + 1) += drop * u * (1 - v);
                at(ny + 1, nx) += drop * (1 - u) * v;
                at(ny + 1, nx + 1) += drop * u * v;
            } else {
                double take = std::fmin((capacity - sediment) * erosion, -deltaH);
                sediment += take;
                at(ny, nx) -= take * (1 - u) * (1 - v);
                at(ny, nx + 1) -= take * u * (1 - v);
                at(ny + 1, nx) -= take * (1 - u) * v;
                at(ny + 1, nx + 1) -= take * u * v;
            }

            speed = std::sqrt(std::fmax(speed * speed + deltaH * gravity, 0.0));
            water *= (1.0 - evaporation);
        }
    }

    double lo = H[0];
    double hi = H[0];
    for (size_t i = 0; i < n; ++i) {
        if (H[i] < lo) {
            lo = H[i];
        }
        if (H[i] > hi) {
            hi = H[i];
        }
    }
    double span = (hi > lo) ? (hi - lo) : 1.0;
    for (size_t i = 0; i < n; ++i) {
        out[i] = (H[i] - lo) / span;
    }
}
//=============================================================================
