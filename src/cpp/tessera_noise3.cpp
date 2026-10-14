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
inline double
grad3(int hash, double x, double y, double z)
{
    int h = hash & 15;
    double u = (h < 8) ? x : y;
    double v = (h < 4) ? y : ((h == 12 || h == 14) ? x : z);
    return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
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
double
perlinRaw3(double x, double y, double z, const int perm[512])
{
    double fx = std::floor(x);
    double fy = std::floor(y);
    double fz = std::floor(z);
    int xi = static_cast<int>(fx) & 255;
    int yi = static_cast<int>(fy) & 255;
    int zi = static_cast<int>(fz) & 255;
    double xf = x - fx;
    double yf = y - fy;
    double zf = z - fz;
    double u = fade(xf);
    double v = fade(yf);
    double w = fade(zf);

    int aaa = perm[perm[perm[xi] + yi] + zi];
    int aba = perm[perm[perm[xi] + yi + 1] + zi];
    int aab = perm[perm[perm[xi] + yi] + zi + 1];
    int abb = perm[perm[perm[xi] + yi + 1] + zi + 1];
    int baa = perm[perm[perm[xi + 1] + yi] + zi];
    int bba = perm[perm[perm[xi + 1] + yi + 1] + zi];
    int bab = perm[perm[perm[xi + 1] + yi] + zi + 1];
    int bbb = perm[perm[perm[xi + 1] + yi + 1] + zi + 1];

    double x1 = lerp(grad3(aaa, xf, yf, zf), grad3(baa, xf - 1.0, yf, zf), u);
    double x2 = lerp(grad3(aba, xf, yf - 1.0, zf), grad3(bba, xf - 1.0, yf - 1.0, zf), u);
    double y1 = lerp(x1, x2, v);
    x1 = lerp(grad3(aab, xf, yf, zf - 1.0), grad3(bab, xf - 1.0, yf, zf - 1.0), u);
    x2 = lerp(grad3(abb, xf, yf - 1.0, zf - 1.0),
        grad3(bbb, xf - 1.0, yf - 1.0, zf - 1.0), u);
    return lerp(y1, lerp(x1, x2, v), w);
}
//=============================================================================
double
perlinRawPeriodic3(double x, double y, double z, const int perm[512], int periodX, int periodY,
    int periodZ)
{
    double fx = std::floor(x);
    double fy = std::floor(y);
    double fz = std::floor(z);
    int x0 = positiveMod(static_cast<int>(fx), periodX) & 255;
    int y0 = positiveMod(static_cast<int>(fy), periodY) & 255;
    int z0 = positiveMod(static_cast<int>(fz), periodZ) & 255;
    int x1 = positiveMod(static_cast<int>(fx) + 1, periodX) & 255;
    int y1 = positiveMod(static_cast<int>(fy) + 1, periodY) & 255;
    int z1 = positiveMod(static_cast<int>(fz) + 1, periodZ) & 255;
    double xf = x - fx;
    double yf = y - fy;
    double zf = z - fz;
    double u = fade(xf);
    double v = fade(yf);
    double w = fade(zf);

    int aaa = perm[perm[perm[x0] + y0] + z0];
    int aba = perm[perm[perm[x0] + y1] + z0];
    int aab = perm[perm[perm[x0] + y0] + z1];
    int abb = perm[perm[perm[x0] + y1] + z1];
    int baa = perm[perm[perm[x1] + y0] + z0];
    int bba = perm[perm[perm[x1] + y1] + z0];
    int bab = perm[perm[perm[x1] + y0] + z1];
    int bbb = perm[perm[perm[x1] + y1] + z1];

    double xBlend0 = lerp(grad3(aaa, xf, yf, zf), grad3(baa, xf - 1.0, yf, zf), u);
    double xBlend1 = lerp(grad3(aba, xf, yf - 1.0, zf), grad3(bba, xf - 1.0, yf - 1.0, zf), u);
    double yBlend0 = lerp(xBlend0, xBlend1, v);
    xBlend0 = lerp(grad3(aab, xf, yf, zf - 1.0), grad3(bab, xf - 1.0, yf, zf - 1.0), u);
    xBlend1 = lerp(grad3(abb, xf, yf - 1.0, zf - 1.0),
        grad3(bbb, xf - 1.0, yf - 1.0, zf - 1.0), u);
    return lerp(yBlend0, lerp(xBlend0, xBlend1, v), w);
}
//=============================================================================
const int simplexGrad3[12][3] = { { 1, 1, 0 }, { -1, 1, 0 }, { 1, -1, 0 }, { -1, -1, 0 },
    { 1, 0, 1 }, { -1, 0, 1 }, { 1, 0, -1 }, { -1, 0, -1 }, { 0, 1, 1 },
    { 0, -1, 1 }, { 0, 1, -1 }, { 0, -1, -1 } };
//=============================================================================
inline double
simplexCorner(double t, int gi, double x, double y, double z)
{
    if (t < 0.0) {
        return 0.0;
    }
    t *= t;
    return t * t * (simplexGrad3[gi][0] * x + simplexGrad3[gi][1] * y + simplexGrad3[gi][2] * z);
}
//=============================================================================
double
simplexRaw3(double xin, double yin, double zin, const int perm[512])
{
    const double F3 = 1.0 / 3.0;
    const double G3 = 1.0 / 6.0;
    double s = (xin + yin + zin) * F3;
    int i = static_cast<int>(std::floor(xin + s));
    int j = static_cast<int>(std::floor(yin + s));
    int k = static_cast<int>(std::floor(zin + s));
    double t = (i + j + k) * G3;
    double x0 = xin - (i - t);
    double y0 = yin - (j - t);
    double z0 = zin - (k - t);

    int i1 = 0, j1 = 0, k1 = 0;
    int i2 = 0, j2 = 0, k2 = 0;
    if (x0 >= y0) {
        if (y0 >= z0) {
            i1 = 1;
            i2 = 1;
            j2 = 1;
        } else if (x0 >= z0) {
            i1 = 1;
            i2 = 1;
            k2 = 1;
        } else {
            k1 = 1;
            i2 = 1;
            k2 = 1;
        }
    } else {
        if (y0 < z0) {
            k1 = 1;
            j2 = 1;
            k2 = 1;
        } else if (x0 < z0) {
            j1 = 1;
            j2 = 1;
            k2 = 1;
        } else {
            j1 = 1;
            i2 = 1;
            j2 = 1;
        }
    }

    double x1 = x0 - i1 + G3;
    double y1 = y0 - j1 + G3;
    double z1 = z0 - k1 + G3;
    double x2 = x0 - i2 + 2.0 * G3;
    double y2 = y0 - j2 + 2.0 * G3;
    double z2 = z0 - k2 + 2.0 * G3;
    double x3 = x0 - 1.0 + 3.0 * G3;
    double y3 = y0 - 1.0 + 3.0 * G3;
    double z3 = z0 - 1.0 + 3.0 * G3;

    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
    int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
    int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
    int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

    double n0 = simplexCorner(0.6 - x0 * x0 - y0 * y0 - z0 * z0, gi0, x0, y0, z0);
    double n1 = simplexCorner(0.6 - x1 * x1 - y1 * y1 - z1 * z1, gi1, x1, y1, z1);
    double n2 = simplexCorner(0.6 - x2 * x2 - y2 * y2 - z2 * z2, gi2, x2, y2, z2);
    double n3 = simplexCorner(0.6 - x3 * x3 - y3 * y3 - z3 * z3, gi3, x3, y3, z3);
    return 32.0 * (n0 + n1 + n2 + n3);
}
//=============================================================================
double
simplexRawPeriodic3(double x, double y, double z, const int perm[512], double period)
{
    double u = (period > 0.0) ? x / period : 0.0;
    double v = (period > 0.0) ? y / period : 0.0;
    double w = (period > 0.0) ? z / period : 0.0;
    double x0 = x - period;
    double y0 = y - period;
    double z0 = z - period;
    double n000 = simplexRaw3(x, y, z, perm);
    double n100 = simplexRaw3(x0, y, z, perm);
    double n010 = simplexRaw3(x, y0, z, perm);
    double n110 = simplexRaw3(x0, y0, z, perm);
    double n001 = simplexRaw3(x, y, z0, perm);
    double n101 = simplexRaw3(x0, y, z0, perm);
    double n011 = simplexRaw3(x, y0, z0, perm);
    double n111 = simplexRaw3(x0, y0, z0, perm);
    double xy0 = lerp(lerp(n000, n100, u), lerp(n010, n110, u), v);
    double xy1 = lerp(lerp(n001, n101, u), lerp(n011, n111, u), v);
    return lerp(xy0, xy1, w);
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
inline size_t
volumeIndex(int row, int col, int plane, int rows, int cols)
{
    return static_cast<size_t>(row) + static_cast<size_t>(col) * rows
        + static_cast<size_t>(plane) * rows * cols;
}
//=============================================================================
} // namespace
//=============================================================================
void
tessera_perlin3(
    int rows, int cols, int planes, double frequency, unsigned int seed, int seamless, double* out)
{
    int perm[512];
    buildPermutation(seed, perm);
    int period = noisePeriod(frequency);
    for (int p = 0; p < planes; ++p) {
        double z = seamless ? (static_cast<double>(p) / (planes > 1 ? planes - 1 : 1)) * period
                            : (static_cast<double>(p) / planes) * frequency;
        for (int c = 0; c < cols; ++c) {
            double x = seamless ? (static_cast<double>(c) / (cols > 1 ? cols - 1 : 1)) * period
                                : (static_cast<double>(c) / cols) * frequency;
            for (int r = 0; r < rows; ++r) {
                double y = seamless ? (static_cast<double>(r) / (rows > 1 ? rows - 1 : 1)) * period
                                    : (static_cast<double>(r) / rows) * frequency;
                double value = seamless ? perlinRawPeriodic3(x, y, z, perm, period, period, period)
                                        : perlinRaw3(x, y, z, perm);
                out[volumeIndex(r, c, p, rows, cols)] = clamp01(value * 0.5 + 0.5);
            }
        }
    }
}
//=============================================================================
void
tessera_simplex3(
    int rows, int cols, int planes, double frequency, unsigned int seed, int seamless, double* out)
{
    int perm[512];
    buildPermutation(seed + 0x9E3779B9u, perm);
    int period = noisePeriod(frequency);
    for (int p = 0; p < planes; ++p) {
        double z = seamless ? (static_cast<double>(p) / (planes > 1 ? planes - 1 : 1)) * period
                            : (static_cast<double>(p) / planes) * frequency;
        for (int c = 0; c < cols; ++c) {
            double x = seamless ? (static_cast<double>(c) / (cols > 1 ? cols - 1 : 1)) * period
                                : (static_cast<double>(c) / cols) * frequency;
            for (int r = 0; r < rows; ++r) {
                double y = seamless ? (static_cast<double>(r) / (rows > 1 ? rows - 1 : 1)) * period
                                    : (static_cast<double>(r) / rows) * frequency;
                double value = seamless ? simplexRawPeriodic3(x, y, z, perm, period)
                                        : simplexRaw3(x, y, z, perm);
                out[volumeIndex(r, c, p, rows, cols)] = clamp01(value * 0.5 + 0.5);
            }
        }
    }
}
//=============================================================================
void
tessera_fbm3(int rows, int cols, int planes, int octaves, double frequency, double lacunarity,
    double gain, unsigned int seed, int seamless, double* out)
{
    if (octaves < 1) {
        octaves = 1;
    }
    int perm[512];
    buildPermutation(seed, perm);
    for (int p = 0; p < planes; ++p) {
        double bz = static_cast<double>(p) / (seamless && planes > 1 ? planes - 1 : planes);
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
                    double sampleZ = bz * (seamless ? period : freq);
                    double sample = seamless
                        ? perlinRawPeriodic3(sampleX, sampleY, sampleZ, perm, period, period, period)
                        : perlinRaw3(sampleX, sampleY, sampleZ, perm);
                    sum += amplitude * sample;
                    norm += amplitude;
                    amplitude *= gain;
                    freq *= lacunarity;
                }
                double value = (norm > 0.0) ? (sum / norm) : 0.0;
                out[volumeIndex(r, c, p, rows, cols)] = clamp01(value * 0.5 + 0.5);
            }
        }
    }
}
//=============================================================================
