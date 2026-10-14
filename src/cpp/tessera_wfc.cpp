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
#include <vector>
//=============================================================================
// Wave function collapse, overlapping model. N x N patterns are learned from a
// toroidally-wrapped integer sample, their pairwise overlap compatibility is
// precomputed, then an output grid is collapsed cell by cell (lowest-entropy
// heuristic, weighted choice, constraint propagation), restarting on
// contradiction.
//=============================================================================
namespace {
struct Rng {
    unsigned int state;
    double next01()
    {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return static_cast<double>(state & 0xFFFFFFu) / static_cast<double>(0x1000000u);
    }
};
} // namespace
//=============================================================================
int
tessera_wfc(const double* sample, int srows, int scols, int n, int outRows, int outCols,
    unsigned int seed, double* out, int* retries, int* contradictions, double* maxEntropy,
    double* trace, int maxTrace, int* traceCount)
{
    if (retries != nullptr) {
        *retries = 0;
    }
    if (contradictions != nullptr) {
        *contradictions = 0;
    }
    if (maxEntropy != nullptr) {
        *maxEntropy = 0.0;
    }
    if (traceCount != nullptr) {
        *traceCount = 0;
    }
    if (n < 1 || srows < n || scols < n || outRows < 1 || outCols < 1) {
        return 0;
    }
    const int area = n * n;

    // Extract and deduplicate N x N patterns (row-major within a pattern).
    std::vector<std::vector<double>> patterns;
    std::vector<double> weights;
    for (int pr = 0; pr < srows; ++pr) {
        for (int pc = 0; pc < scols; ++pc) {
            std::vector<double> pat(area);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    int sr = (pr + i) % srows;
                    int sc = (pc + j) % scols;
                    pat[i * n + j] = sample[sr + static_cast<size_t>(sc) * srows];
                }
            }
            int found = -1;
            for (size_t k = 0; k < patterns.size(); ++k) {
                if (patterns[k] == pat) {
                    found = static_cast<int>(k);
                    break;
                }
            }
            if (found < 0) {
                patterns.push_back(pat);
                weights.push_back(1.0);
            } else {
                weights[found] += 1.0;
            }
        }
    }
    const int P = static_cast<int>(patterns.size());
    if (P == 0) {
        return 0;
    }

    // Overlap compatibility: compat[d*P*P + a*P + b] is 1 when pattern b may sit
    // in direction d of pattern a (overlapping cells agree).
    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };
    std::vector<unsigned char> compat(static_cast<size_t>(4) * P * P, 0);
    for (int d = 0; d < 4; ++d) {
        for (int a = 0; a < P; ++a) {
            for (int b = 0; b < P; ++b) {
                bool ok = true;
                for (int i = 0; i < n && ok; ++i) {
                    for (int j = 0; j < n; ++j) {
                        int i2 = i - dr[d];
                        int j2 = j - dc[d];
                        if (i2 >= 0 && i2 < n && j2 >= 0 && j2 < n) {
                            if (patterns[a][i * n + j] != patterns[b][i2 * n + j2]) {
                                ok = false;
                                break;
                            }
                        }
                    }
                }
                compat[(static_cast<size_t>(d) * P + a) * P + b] = ok ? 1 : 0;
            }
        }
    }

    const int cells = outRows * outCols;
    std::vector<unsigned char> possible(static_cast<size_t>(cells) * P);
    std::vector<int> popcount(cells);
    Rng rng { seed ? seed : 1u };

    const int maxRetries = 40;
    for (int attempt = 0; attempt < maxRetries; ++attempt) {
        std::fill(possible.begin(), possible.end(), 1);
        for (int c = 0; c < cells; ++c) {
            popcount[c] = P;
        }
        bool contradiction = false;
        std::vector<int> stack;

        auto propagate = [&]() -> bool {
            while (!stack.empty()) {
                int cell = stack.back();
                stack.pop_back();
                int cr = cell / outCols;
                int cc = cell % outCols;
                unsigned char* curr = &possible[static_cast<size_t>(cell) * P];
                for (int d = 0; d < 4; ++d) {
                    int nr = cr + dr[d];
                    int nc = cc + dc[d];
                    if (nr < 0 || nc < 0 || nr >= outRows || nc >= outCols) {
                        continue;
                    }
                    int ncell = nr * outCols + nc;
                    unsigned char* nb = &possible[static_cast<size_t>(ncell) * P];
                    bool changed = false;
                    for (int b = 0; b < P; ++b) {
                        if (!nb[b]) {
                            continue;
                        }
                        bool supported = false;
                        for (int a = 0; a < P; ++a) {
                            if (curr[a] && compat[(static_cast<size_t>(d) * P + a) * P + b]) {
                                supported = true;
                                break;
                            }
                        }
                        if (!supported) {
                            nb[b] = 0;
                            popcount[ncell]--;
                            changed = true;
                        }
                    }
                    if (popcount[ncell] == 0) {
                        return false;
                    }
                    if (changed) {
                        stack.push_back(ncell);
                    }
                }
            }
            return true;
        };

        while (true) {
            // Observe: lowest-entropy (fewest remaining) undecided cell.
            int best = -1;
            int bestCount = P + 1;
            int tie = 0;
            for (int c = 0; c < cells; ++c) {
                if (popcount[c] <= 1) {
                    continue;
                }
                if (popcount[c] < bestCount) {
                    bestCount = popcount[c];
                    best = c;
                    tie = 1;
                } else if (popcount[c] == bestCount) {
                    tie++;
                    if (rng.next01() < 1.0 / tie) {
                        best = c;
                    }
                }
            }
            if (best < 0) {
                break; // fully collapsed
            }
            if (maxEntropy != nullptr && bestCount > *maxEntropy) {
                *maxEntropy = static_cast<double>(bestCount);
            }
            if (trace != nullptr && traceCount != nullptr && *traceCount < maxTrace) {
                int traceIndex = *traceCount;
                int traceRow = best / outCols;
                int traceCol = best % outCols;
                trace[traceIndex] = static_cast<double>(traceRow + 1);
                trace[traceIndex + maxTrace] = static_cast<double>(traceCol + 1);
                trace[traceIndex + 2 * maxTrace] = static_cast<double>(bestCount);
                (*traceCount)++;
            }
            // Collapse: weighted random choice among the remaining patterns.
            unsigned char* curr = &possible[static_cast<size_t>(best) * P];
            double total = 0.0;
            for (int a = 0; a < P; ++a) {
                if (curr[a]) {
                    total += weights[a];
                }
            }
            double pick = rng.next01() * total;
            int chosen = -1;
            for (int a = 0; a < P; ++a) {
                if (curr[a]) {
                    pick -= weights[a];
                    if (pick <= 0.0) {
                        chosen = a;
                        break;
                    }
                }
            }
            if (chosen < 0) {
                for (int a = P - 1; a >= 0; --a) {
                    if (curr[a]) {
                        chosen = a;
                        break;
                    }
                }
            }
            for (int a = 0; a < P; ++a) {
                curr[a] = (a == chosen) ? 1 : 0;
            }
            popcount[best] = 1;
            stack.push_back(best);
            if (!propagate()) {
                contradiction = true;
                if (contradictions != nullptr) {
                    (*contradictions)++;
                }
                break;
            }
        }

        if (!contradiction) {
            if (retries != nullptr) {
                *retries = attempt;
            }
            for (int c = 0; c < cells; ++c) {
                int cr = c / outCols;
                int cc = c % outCols;
                unsigned char* curr = &possible[static_cast<size_t>(c) * P];
                double value = patterns[0][0];
                for (int a = 0; a < P; ++a) {
                    if (curr[a]) {
                        value = patterns[a][0];
                        break;
                    }
                }
                out[cr + static_cast<size_t>(cc) * outRows] = value;
            }
            return 1;
        }
    }

    // Every retry hit a contradiction: fall back to the most common pattern.
    for (int i = 0; i < cells; ++i) {
        out[i] = patterns[0][0];
    }
    if (retries != nullptr) {
        *retries = maxRetries;
    }
    return 0;
}
//=============================================================================
