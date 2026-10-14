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
tessera_gameoflife(const double* in, int rows, int cols, int generations, double* out)
{
    std::vector<unsigned char> current(static_cast<size_t>(rows) * cols);
    std::vector<unsigned char> next(static_cast<size_t>(rows) * cols);
    for (size_t i = 0; i < current.size(); ++i) {
        current[i] = (in[i] != 0.0) ? 1 : 0;
    }
    if (generations < 0) {
        generations = 0;
    }
    for (int g = 0; g < generations; ++g) {
        for (int c = 0; c < cols; ++c) {
            for (int r = 0; r < rows; ++r) {
                int alive = 0;
                for (int dc = -1; dc <= 1; ++dc) {
                    for (int dr = -1; dr <= 1; ++dr) {
                        if (dr == 0 && dc == 0) {
                            continue;
                        }
                        int rr = (r + dr + rows) % rows;
                        int cc = (c + dc + cols) % cols;
                        alive += current[rr + static_cast<size_t>(cc) * rows];
                    }
                }
                unsigned char cell = current[r + static_cast<size_t>(c) * rows];
                unsigned char live = (cell && (alive == 2 || alive == 3)) || (!cell && alive == 3);
                next[r + static_cast<size_t>(c) * rows] = live ? 1 : 0;
            }
        }
        current.swap(next);
    }
    for (size_t i = 0; i < current.size(); ++i) {
        out[i] = static_cast<double>(current[i]);
    }
}
//=============================================================================
void
tessera_automaton1d(int rule, int width, int generations, const double* init, int hasInit,
    unsigned int seed, double* out)
{
    std::vector<unsigned char> row(static_cast<size_t>(width), 0);
    if (hasInit) {
        for (int x = 0; x < width; ++x) {
            row[x] = (init[x] != 0.0) ? 1 : 0;
        }
    } else if (seed != 0u) {
        unsigned int state = seed;
        for (int x = 0; x < width; ++x) {
            state ^= state << 13;
            state ^= state >> 17;
            state ^= state << 5;
            row[x] = (state & 1u) ? 1 : 0;
        }
    } else {
        row[width / 2] = 1;
    }
    std::vector<unsigned char> nextRow(static_cast<size_t>(width), 0);
    for (int g = 0; g < generations; ++g) {
        for (int x = 0; x < width; ++x) {
            out[g + static_cast<size_t>(x) * generations] = static_cast<double>(row[x]);
        }
        for (int x = 0; x < width; ++x) {
            int left = row[(x - 1 + width) % width];
            int center = row[x];
            int right = row[(x + 1) % width];
            int pattern = (left << 2) | (center << 1) | right;
            nextRow[x] = (rule >> pattern) & 1;
        }
        row.swap(nextRow);
    }
}
//=============================================================================
