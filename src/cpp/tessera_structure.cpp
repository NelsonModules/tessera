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
void
tessera_cave(int rows, int cols, double fill, int steps, unsigned int seed, double* out)
{
    const size_t n = static_cast<size_t>(rows) * cols;
    std::vector<unsigned char> cur(n, 0);
    std::vector<unsigned char> nxt(n, 0);
    auto idx = [&](int r, int c) -> size_t { return static_cast<size_t>(r) + static_cast<size_t>(c) * rows; };

    unsigned int state = seed ? seed : 1u;
    auto nextu = [&]() -> double {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return static_cast<double>(state & 0xFFFFFFu) / static_cast<double>(0x1000000u);
    };
    // Random fill; borders are walls.
    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            bool border = (r == 0 || c == 0 || r == rows - 1 || c == cols - 1);
            cur[idx(r, c)] = (border || nextu() < fill) ? 1 : 0;
        }
    }
    if (steps < 0) {
        steps = 0;
    }
    for (int s = 0; s < steps; ++s) {
        for (int c = 0; c < cols; ++c) {
            for (int r = 0; r < rows; ++r) {
                int walls = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        if (dr == 0 && dc == 0) {
                            continue;
                        }
                        int rr = r + dr;
                        int cc = c + dc;
                        if (rr < 0 || cc < 0 || rr >= rows || cc >= cols) {
                            walls++; // out of bounds counts as wall
                        } else {
                            walls += cur[idx(rr, cc)];
                        }
                    }
                }
                // Classic cave rule: a cell is a wall when surrounded by >= 5 walls.
                nxt[idx(r, c)] = (walls >= 5) ? 1 : 0;
            }
        }
        cur.swap(nxt);
    }
    for (size_t i = 0; i < n; ++i) {
        out[i] = static_cast<double>(cur[i]);
    }
}
//=============================================================================
void
tessera_maze(int rows, int cols, unsigned int seed, double* out)
{
    const int H = 2 * rows + 1;
    const int W = 2 * cols + 1;
    auto oidx = [&](int r, int c) -> size_t { return static_cast<size_t>(r) + static_cast<size_t>(c) * H; };
    for (int i = 0; i < H * W; ++i) {
        out[i] = 1.0; // everything is wall initially
    }

    unsigned int state = seed ? seed : 1u;
    auto nextu = [&]() -> unsigned int {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    };

    std::vector<unsigned char> visited(static_cast<size_t>(rows) * cols, 0);
    std::vector<int> stackR;
    std::vector<int> stackC;
    stackR.push_back(0);
    stackC.push_back(0);
    visited[0] = 1;
    out[oidx(1, 1)] = 0.0; // carve first cell

    const int dR[4] = { -1, 1, 0, 0 };
    const int dC[4] = { 0, 0, -1, 1 };
    while (!stackR.empty()) {
        int cr = stackR.back();
        int cc = stackC.back();
        int order[4] = { 0, 1, 2, 3 };
        for (int i = 3; i > 0; --i) {
            int j = static_cast<int>(nextu() % static_cast<unsigned int>(i + 1));
            int t = order[i];
            order[i] = order[j];
            order[j] = t;
        }
        bool moved = false;
        for (int k = 0; k < 4; ++k) {
            int dir = order[k];
            int nr = cr + dR[dir];
            int nc = cc + dC[dir];
            if (nr < 0 || nc < 0 || nr >= rows || nc >= cols) {
                continue;
            }
            if (visited[static_cast<size_t>(nr) + static_cast<size_t>(nc) * rows]) {
                continue;
            }
            visited[static_cast<size_t>(nr) + static_cast<size_t>(nc) * rows] = 1;
            // carve the destination cell and the wall between.
            out[oidx(2 * nr + 1, 2 * nc + 1)] = 0.0;
            out[oidx(cr + nr + 1, cc + nc + 1)] = 0.0;
            stackR.push_back(nr);
            stackC.push_back(nc);
            moved = true;
            break;
        }
        if (!moved) {
            stackR.pop_back();
            stackC.pop_back();
        }
    }
}
//=============================================================================
