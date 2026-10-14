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
//=============================================================================
namespace {
//=============================================================================
inline double
fade(double t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}
//=============================================================================
inline double
lerp(double a, double b, double t)
{
    return a + t * (b - a);
}
//=============================================================================
// Seeded 512-entry permutation table (Fisher-Yates shuffle driven by an LCG).
void
buildPermutation(unsigned int seed, int perm[512])
{
    int base[256];
    for (int i = 0; i < 256; ++i) {
        base[i] = i;
    }
    unsigned int state = seed * 747796405u + 2891336453u;
    for (int i = 255; i > 0; --i) {
        state = state * 747796405u + 2891336453u;
        int j = static_cast<int>((state >> 16) % static_cast<unsigned int>(i + 1));
        int tmp = base[i];
        base[i] = base[j];
        base[j] = tmp;
    }
    for (int i = 0; i < 512; ++i) {
        perm[i] = base[i & 255];
    }
}
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
inline int
positiveMod(int value, int period)
{
    int r = value % period;
    return (r < 0) ? r + period : r;
}
//=============================================================================
inline int
noisePeriod(double frequency)
{
    int period = static_cast<int>(std::lround(std::fabs(frequency)));
    return (period < 1) ? 1 : period;
}
//=============================================================================
inline double
grad2(int hash, double x, double y)
{
    switch (hash & 7) {
    case 0:
        return x + y;
    case 1:
        return -x + y;
    case 2:
        return x - y;
    case 3:
        return -x - y;
    case 4:
        return x;
    case 5:
        return -x;
    case 6:
        return y;
    default:
        return -y;
    }
}
//=============================================================================
// Classic Perlin gradient noise, raw output in about [-1, 1].
double
perlinRaw(double x, double y, const int perm[512])
{
    double fx = std::floor(x);
    double fy = std::floor(y);
    int xi = static_cast<int>(fx) & 255;
    int yi = static_cast<int>(fy) & 255;
    double xf = x - fx;
    double yf = y - fy;
    double u = fade(xf);
    double v = fade(yf);
    int aa = perm[perm[xi] + yi];
    int ab = perm[perm[xi] + yi + 1];
    int ba = perm[perm[xi + 1] + yi];
    int bb = perm[perm[xi + 1] + yi + 1];
    double x1 = lerp(grad2(aa, xf, yf), grad2(ba, xf - 1.0, yf), u);
    double x2 = lerp(grad2(ab, xf, yf - 1.0), grad2(bb, xf - 1.0, yf - 1.0), u);
    return lerp(x1, x2, v);
}
//=============================================================================
double
perlinRawPeriodic(double x, double y, const int perm[512], int periodX, int periodY)
{
    double fx = std::floor(x);
    double fy = std::floor(y);
    int x0 = positiveMod(static_cast<int>(fx), periodX) & 255;
    int y0 = positiveMod(static_cast<int>(fy), periodY) & 255;
    int x1 = positiveMod(static_cast<int>(fx) + 1, periodX) & 255;
    int y1 = positiveMod(static_cast<int>(fy) + 1, periodY) & 255;
    double xf = x - fx;
    double yf = y - fy;
    double u = fade(xf);
    double v = fade(yf);
    int aa = perm[perm[x0] + y0];
    int ab = perm[perm[x0] + y1];
    int ba = perm[perm[x1] + y0];
    int bb = perm[perm[x1] + y1];
    double xBlend0 = lerp(grad2(aa, xf, yf), grad2(ba, xf - 1.0, yf), u);
    double xBlend1 = lerp(grad2(ab, xf, yf - 1.0), grad2(bb, xf - 1.0, yf - 1.0), u);
    return lerp(xBlend0, xBlend1, v);
}
//=============================================================================
// Value noise, raw output in [0, 1].
double
valueRaw(double x, double y, unsigned int seed)
{
    double fx = std::floor(x);
    double fy = std::floor(y);
    int xi = static_cast<int>(fx);
    int yi = static_cast<int>(fy);
    double xf = x - fx;
    double yf = y - fy;
    double u = fade(xf);
    double v = fade(yf);
    double v00 = rand01(hash2(xi, yi, seed));
    double v10 = rand01(hash2(xi + 1, yi, seed));
    double v01 = rand01(hash2(xi, yi + 1, seed));
    double v11 = rand01(hash2(xi + 1, yi + 1, seed));
    return lerp(lerp(v00, v10, u), lerp(v01, v11, u), v);
}
//=============================================================================
// Simplex noise (2d), raw output in about [-1, 1].
const int simplexGrad[12][2]
    = { { 1, 1 }, { -1, 1 }, { 1, -1 }, { -1, -1 }, { 1, 0 }, { -1, 0 }, { 1, 0 }, { -1, 0 },
          { 0, 1 }, { 0, -1 }, { 0, 1 }, { 0, -1 } };
//=============================================================================
double
simplexRaw(double xin, double yin, const int perm[512])
{
    const double F2 = 0.3660254037844386; // 0.5 * (sqrt(3) - 1)
    const double G2 = 0.21132486540518713; // (3 - sqrt(3)) / 6
    double s = (xin + yin) * F2;
    int i = static_cast<int>(std::floor(xin + s));
    int j = static_cast<int>(std::floor(yin + s));
    double t = (i + j) * G2;
    double x0 = xin - (i - t);
    double y0 = yin - (j - t);
    int i1 = (x0 > y0) ? 1 : 0;
    int j1 = (x0 > y0) ? 0 : 1;
    double x1 = x0 - i1 + G2;
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0 + 2.0 * G2;
    double y2 = y0 - 1.0 + 2.0 * G2;
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = perm[ii + perm[jj]] % 12;
    int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
    int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;
    double n0 = 0.0, n1 = 0.0, n2 = 0.0;
    double t0 = 0.5 - x0 * x0 - y0 * y0;
    if (t0 > 0.0) {
        t0 *= t0;
        n0 = t0 * t0 * (simplexGrad[gi0][0] * x0 + simplexGrad[gi0][1] * y0);
    }
    double t1 = 0.5 - x1 * x1 - y1 * y1;
    if (t1 > 0.0) {
        t1 *= t1;
        n1 = t1 * t1 * (simplexGrad[gi1][0] * x1 + simplexGrad[gi1][1] * y1);
    }
    double t2 = 0.5 - x2 * x2 - y2 * y2;
    if (t2 > 0.0) {
        t2 *= t2;
        n2 = t2 * t2 * (simplexGrad[gi2][0] * x2 + simplexGrad[gi2][1] * y2);
    }
    return 70.0 * (n0 + n1 + n2);
}
//=============================================================================
double
simplexRawPeriodic(double x, double y, const int perm[512], double period)
{
    double u = (period > 0.0) ? x / period : 0.0;
    double v = (period > 0.0) ? y / period : 0.0;
    double x0 = x - period;
    double y0 = y - period;
    double n00 = simplexRaw(x, y, perm);
    double n10 = simplexRaw(x0, y, perm);
    double n01 = simplexRaw(x, y0, perm);
    double n11 = simplexRaw(x0, y0, perm);
    return lerp(lerp(n00, n10, u), lerp(n01, n11, u), v);
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
tessera_perlin(int rows, int cols, double frequency, unsigned int seed, int seamless, double* out)
{
    int perm[512];
    buildPermutation(seed, perm);
    int period = noisePeriod(frequency);
    for (int c = 0; c < cols; ++c) {
        double x = seamless ? (static_cast<double>(c) / (cols > 1 ? cols - 1 : 1)) * period
                            : (static_cast<double>(c) / cols) * frequency;
        for (int r = 0; r < rows; ++r) {
            double y = seamless ? (static_cast<double>(r) / (rows > 1 ? rows - 1 : 1)) * period
                                : (static_cast<double>(r) / rows) * frequency;
            double value = seamless ? perlinRawPeriodic(x, y, perm, period, period) : perlinRaw(x, y, perm);
            out[r + c * rows] = clamp01(value * 0.5 + 0.5);
        }
    }
}
//=============================================================================
void
tessera_simplex(
    int rows, int cols, double frequency, unsigned int seed, int seamless, double* out)
{
    int perm[512];
    buildPermutation(seed, perm);
    int period = noisePeriod(frequency);
    for (int c = 0; c < cols; ++c) {
        double x = seamless ? (static_cast<double>(c) / (cols > 1 ? cols - 1 : 1)) * period
                            : (static_cast<double>(c) / cols) * frequency;
        for (int r = 0; r < rows; ++r) {
            double y = seamless ? (static_cast<double>(r) / (rows > 1 ? rows - 1 : 1)) * period
                                : (static_cast<double>(r) / rows) * frequency;
            double value = seamless ? simplexRawPeriodic(x, y, perm, period) : simplexRaw(x, y, perm);
            out[r + c * rows] = clamp01(value * 0.5 + 0.5);
        }
    }
}
//=============================================================================
void
tessera_value_noise(int rows, int cols, double frequency, unsigned int seed, double* out)
{
    for (int c = 0; c < cols; ++c) {
        double x = (static_cast<double>(c) / cols) * frequency;
        for (int r = 0; r < rows; ++r) {
            double y = (static_cast<double>(r) / rows) * frequency;
            out[r + c * rows] = clamp01(valueRaw(x, y, seed));
        }
    }
}
//=============================================================================
void
tessera_worley(int rows, int cols, int cells, unsigned int seed, double* out)
{
    if (cells < 1) {
        cells = 1;
    }
    for (int c = 0; c < cols; ++c) {
        double fx = (static_cast<double>(c) / cols) * cells;
        int cx = static_cast<int>(std::floor(fx));
        for (int r = 0; r < rows; ++r) {
            double fy = (static_cast<double>(r) / rows) * cells;
            int cy = static_cast<int>(std::floor(fy));
            double best = 1.0e300;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int gx = cx + dx;
                    int gy = cy + dy;
                    unsigned int h = hash2(gx, gy, seed);
                    double px = gx + rand01(h);
                    double py = gy + rand01(h * 2246822519u + 1u);
                    double ex = px - fx;
                    double ey = py - fy;
                    double d = ex * ex + ey * ey;
                    if (d < best) {
                        best = d;
                    }
                }
            }
            out[r + c * rows] = clamp01(std::sqrt(best));
        }
    }
}
//=============================================================================
void
tessera_fbm(int rows, int cols, int octaves, double frequency, double lacunarity, double gain,
    unsigned int seed, int seamless, double* out)
{
    if (octaves < 1) {
        octaves = 1;
    }
    int perm[512];
    buildPermutation(seed, perm);
    for (int c = 0; c < cols; ++c) {
        double bx = static_cast<double>(c) / (seamless && cols > 1 ? cols - 1 : cols);
        for (int r = 0; r < rows; ++r) {
            double by = static_cast<double>(r) / (seamless && rows > 1 ? rows - 1 : rows);
            double amplitude = 1.0;
            double freq = frequency;
            double sum = 0.0;
            double norm = 0.0;
            for (int o = 0; o < octaves; ++o) {
                int period = noisePeriod(freq);
                double sampleX = bx * (seamless ? period : freq);
                double sampleY = by * (seamless ? period : freq);
                double sample = seamless ? perlinRawPeriodic(sampleX, sampleY, perm, period, period)
                                         : perlinRaw(sampleX, sampleY, perm);
                sum += amplitude * sample;
                norm += amplitude;
                amplitude *= gain;
                freq *= lacunarity;
            }
            double value = (norm > 0.0) ? (sum / norm) : 0.0;
            out[r + c * rows] = clamp01(value * 0.5 + 0.5);
        }
    }
}
//=============================================================================
namespace {
// Normalized fBm sample in about [-1, 1] at a point (shared by the warp).
double
fbmSample(double x, double y, const int perm[512], int octaves)
{
    double amplitude = 1.0;
    double freq = 1.0;
    double sum = 0.0;
    double norm = 0.0;
    for (int o = 0; o < octaves; ++o) {
        sum += amplitude * perlinRaw(x * freq, y * freq, perm);
        norm += amplitude;
        amplitude *= 0.5;
        freq *= 2.0;
    }
    return (norm > 0.0) ? (sum / norm) : 0.0;
}
} // namespace
//=============================================================================
void
tessera_ridged(int rows, int cols, int octaves, double frequency, unsigned int seed, double* out)
{
    if (octaves < 1) {
        octaves = 1;
    }
    int perm[512];
    buildPermutation(seed, perm);
    for (int c = 0; c < cols; ++c) {
        double bx = static_cast<double>(c) / cols;
        for (int r = 0; r < rows; ++r) {
            double by = static_cast<double>(r) / rows;
            double amplitude = 0.5;
            double freq = frequency;
            double sum = 0.0;
            double norm = 0.0;
            for (int o = 0; o < octaves; ++o) {
                double s = 1.0 - std::fabs(perlinRaw(bx * freq, by * freq, perm));
                s *= s;
                sum += s * amplitude;
                norm += amplitude;
                amplitude *= 0.5;
                freq *= 2.0;
            }
            out[r + c * rows] = clamp01((norm > 0.0) ? (sum / norm) : 0.0);
        }
    }
}
//=============================================================================
void
tessera_billow(int rows, int cols, int octaves, double frequency, unsigned int seed, double* out)
{
    if (octaves < 1) {
        octaves = 1;
    }
    int perm[512];
    buildPermutation(seed, perm);
    for (int c = 0; c < cols; ++c) {
        double bx = static_cast<double>(c) / cols;
        for (int r = 0; r < rows; ++r) {
            double by = static_cast<double>(r) / rows;
            double amplitude = 1.0;
            double freq = frequency;
            double sum = 0.0;
            double norm = 0.0;
            for (int o = 0; o < octaves; ++o) {
                double s = 2.0 * std::fabs(perlinRaw(bx * freq, by * freq, perm)) - 1.0;
                sum += s * amplitude;
                norm += amplitude;
                amplitude *= 0.5;
                freq *= 2.0;
            }
            double value = (norm > 0.0) ? (sum / norm) : 0.0;
            out[r + c * rows] = clamp01(value * 0.5 + 0.5);
        }
    }
}
//=============================================================================
void
tessera_turbulence(
    int rows, int cols, int octaves, double frequency, unsigned int seed, double* out)
{
    if (octaves < 1) {
        octaves = 1;
    }
    int perm[512];
    buildPermutation(seed, perm);
    for (int c = 0; c < cols; ++c) {
        double bx = static_cast<double>(c) / cols;
        for (int r = 0; r < rows; ++r) {
            double by = static_cast<double>(r) / rows;
            double amplitude = 1.0;
            double freq = frequency;
            double sum = 0.0;
            double norm = 0.0;
            for (int o = 0; o < octaves; ++o) {
                sum += std::fabs(perlinRaw(bx * freq, by * freq, perm)) * amplitude;
                norm += amplitude;
                amplitude *= 0.5;
                freq *= 2.0;
            }
            out[r + c * rows] = clamp01((norm > 0.0) ? (sum / norm) : 0.0);
        }
    }
}
//=============================================================================
void
tessera_domain_warp(int rows, int cols, int octaves, double frequency, double strength,
    unsigned int seed, double* out)
{
    if (octaves < 1) {
        octaves = 1;
    }
    int perm[512];
    buildPermutation(seed, perm);
    for (int c = 0; c < cols; ++c) {
        double bx = (static_cast<double>(c) / cols) * frequency;
        for (int r = 0; r < rows; ++r) {
            double by = (static_cast<double>(r) / rows) * frequency;
            double q1 = fbmSample(bx, by, perm, octaves);
            double q2 = fbmSample(bx + 5.2, by + 1.3, perm, octaves);
            double r1 = fbmSample(bx + strength * q1 + 1.7, by + strength * q2 + 9.2, perm, octaves);
            double r2 = fbmSample(bx + strength * q1 + 8.3, by + strength * q2 + 2.8, perm, octaves);
            double v = fbmSample(bx + strength * r1, by + strength * r2, perm, octaves);
            out[r + c * rows] = clamp01(v * 0.5 + 0.5);
        }
    }
}
//=============================================================================
void
tessera_voronoi(int rows, int cols, int cells, unsigned int seed, double* out)
{
    if (cells < 1) {
        cells = 1;
    }
    for (int c = 0; c < cols; ++c) {
        double fx = (static_cast<double>(c) / cols) * cells;
        int cx = static_cast<int>(std::floor(fx));
        for (int r = 0; r < rows; ++r) {
            double fy = (static_cast<double>(r) / rows) * cells;
            int cy = static_cast<int>(std::floor(fy));
            double best = 1.0e300;
            int bestId = 1;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    int gx = cx + dx;
                    int gy = cy + dy;
                    unsigned int h = hash2(gx, gy, seed);
                    double px = gx + rand01(h);
                    double py = gy + rand01(h * 2246822519u + 1u);
                    double ex = px - fx;
                    double ey = py - fy;
                    double d = ex * ex + ey * ey;
                    if (d < best) {
                        best = d;
                        int wx = ((gx % cells) + cells) % cells;
                        int wy = ((gy % cells) + cells) % cells;
                        bestId = wy * cells + wx + 1;
                    }
                }
            }
            out[r + c * rows] = static_cast<double>(bestId);
        }
    }
}
//=============================================================================
void
tessera_curl(int rows, int cols, double frequency, unsigned int seed, double* outU, double* outV)
{
    int perm[512];
    buildPermutation(seed, perm);
    const double eps = 1.0e-3;
    for (int c = 0; c < cols; ++c) {
        double x = (static_cast<double>(c) / cols) * frequency;
        for (int r = 0; r < rows; ++r) {
            double y = (static_cast<double>(r) / rows) * frequency;
            double dPdy = (perlinRaw(x, y + eps, perm) - perlinRaw(x, y - eps, perm)) / (2.0 * eps);
            double dPdx = (perlinRaw(x + eps, y, perm) - perlinRaw(x - eps, y, perm)) / (2.0 * eps);
            outU[r + c * rows] = dPdy;
            outV[r + c * rows] = -dPdx;
        }
    }
}
//=============================================================================
