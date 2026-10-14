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
#include <vector>
//=============================================================================
// Gray-Scott reaction-diffusion with a 9-point Laplacian (Karl Sims weights)
// on a toroidal grid. Returns the normalized V concentration in [0, 1].
void
tessera_reaction_diffusion(
    int rows, int cols, int steps, double feed, double kill, unsigned int seed, double* out)
{
    const size_t n = static_cast<size_t>(rows) * cols;
    std::vector<double> U(n, 1.0);
    std::vector<double> V(n, 0.0);
    std::vector<double> U2(n, 0.0);
    std::vector<double> V2(n, 0.0);
    auto idx = [&](int r, int c) -> size_t { return static_cast<size_t>(r) + static_cast<size_t>(c) * rows; };

    // Seed a few random square patches of V.
    unsigned int state = seed ? seed : 1u;
    auto nextu = [&]() -> double {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return static_cast<double>(state & 0xFFFFFFu) / static_cast<double>(0x1000000u);
    };
    int patches = 10;
    for (int p = 0; p < patches; ++p) {
        int pr = static_cast<int>(nextu() * rows);
        int pc = static_cast<int>(nextu() * cols);
        for (int dr = -3; dr <= 3; ++dr) {
            for (int dc = -3; dc <= 3; ++dc) {
                int rr = (pr + dr + rows) % rows;
                int cc = (pc + dc + cols) % cols;
                V[idx(rr, cc)] = 1.0;
                U[idx(rr, cc)] = 0.0;
            }
        }
    }

    const double Du = 1.0;
    const double Dv = 0.5;
    const double dt = 1.0;
    if (steps < 0) {
        steps = 0;
    }
    // Hot loop: column-major pointer walks and branch-based wrap avoid the
    // per-cell modulo and index recomputation of the naive form.
    const double killFeed = kill + feed;
    for (int s = 0; s < steps; ++s) {
        for (int c = 0; c < cols; ++c) {
            int cm = (c == 0) ? cols - 1 : c - 1;
            int cp = (c == cols - 1) ? 0 : c + 1;
            const double* Uc = &U[static_cast<size_t>(c) * rows];
            const double* Ucm = &U[static_cast<size_t>(cm) * rows];
            const double* Ucp = &U[static_cast<size_t>(cp) * rows];
            const double* Vc = &V[static_cast<size_t>(c) * rows];
            const double* Vcm = &V[static_cast<size_t>(cm) * rows];
            const double* Vcp = &V[static_cast<size_t>(cp) * rows];
            double* U2c = &U2[static_cast<size_t>(c) * rows];
            double* V2c = &V2[static_cast<size_t>(c) * rows];
            for (int r = 0; r < rows; ++r) {
                int rm = (r == 0) ? rows - 1 : r - 1;
                int rp = (r == rows - 1) ? 0 : r + 1;
                double u = Uc[r];
                double v = Vc[r];
                double lapU = 0.2 * (Uc[rm] + Uc[rp] + Ucm[r] + Ucp[r])
                    + 0.05 * (Ucm[rm] + Ucm[rp] + Ucp[rm] + Ucp[rp]) - u;
                double lapV = 0.2 * (Vc[rm] + Vc[rp] + Vcm[r] + Vcp[r])
                    + 0.05 * (Vcm[rm] + Vcm[rp] + Vcp[rm] + Vcp[rp]) - v;
                double uvv = u * v * v;
                U2c[r] = u + (Du * lapU - uvv + feed * (1.0 - u)) * dt;
                V2c[r] = v + (Dv * lapV + uvv - killFeed * v) * dt;
            }
        }
        U.swap(U2);
        V.swap(V2);
    }

    double lo = V[0];
    double hi = V[0];
    for (size_t i = 0; i < n; ++i) {
        if (V[i] < lo) {
            lo = V[i];
        }
        if (V[i] > hi) {
            hi = V[i];
        }
    }
    double span = (hi > lo) ? (hi - lo) : 1.0;
    for (size_t i = 0; i < n; ++i) {
        out[i] = (V[i] - lo) / span;
    }
}
//=============================================================================
