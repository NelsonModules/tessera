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
mapAxis(int index, int count, double lo, double hi)
{
    double denom = (count > 1) ? static_cast<double>(count - 1) : 1.0;
    return lo + (hi - lo) * (static_cast<double>(index) / denom);
}
//=============================================================================
inline double
smoothCount(int iter, double mag2)
{
    // Normalized iteration count for continuous coloring.
    return static_cast<double>(iter) + 1.0 - std::log(std::log(std::sqrt(mag2))) / std::log(2.0);
}
//=============================================================================
} // namespace
//=============================================================================
void
tessera_mandelbrot(int rows, int cols, double xmin, double xmax, double ymin, double ymax,
    int maxIter, double* out)
{
    for (int c = 0; c < cols; ++c) {
        double cx = mapAxis(c, cols, xmin, xmax);
        for (int r = 0; r < rows; ++r) {
            double cy = mapAxis(r, rows, ymin, ymax);
            double zr = 0.0, zi = 0.0, mag2 = 0.0;
            int it = 0;
            while (it < maxIter) {
                double nzr = zr * zr - zi * zi + cx;
                double nzi = 2.0 * zr * zi + cy;
                zr = nzr;
                zi = nzi;
                mag2 = zr * zr + zi * zi;
                if (mag2 > 4.0) {
                    break;
                }
                ++it;
            }
            out[r + c * rows] = (it >= maxIter) ? 0.0 : smoothCount(it, mag2);
        }
    }
}
//=============================================================================
void
tessera_julia(int rows, int cols, double cre, double cim, double xmin, double xmax, double ymin,
    double ymax, int maxIter, double* out)
{
    for (int c = 0; c < cols; ++c) {
        double x0 = mapAxis(c, cols, xmin, xmax);
        for (int r = 0; r < rows; ++r) {
            double y0 = mapAxis(r, rows, ymin, ymax);
            double zr = x0, zi = y0, mag2 = 0.0;
            int it = 0;
            while (it < maxIter) {
                double nzr = zr * zr - zi * zi + cre;
                double nzi = 2.0 * zr * zi + cim;
                zr = nzr;
                zi = nzi;
                mag2 = zr * zr + zi * zi;
                if (mag2 > 4.0) {
                    break;
                }
                ++it;
            }
            out[r + c * rows] = (it >= maxIter) ? 0.0 : smoothCount(it, mag2);
        }
    }
}
//=============================================================================
void
tessera_burningship(int rows, int cols, double xmin, double xmax, double ymin, double ymax,
    int maxIter, double* out)
{
    for (int c = 0; c < cols; ++c) {
        double cx = mapAxis(c, cols, xmin, xmax);
        for (int r = 0; r < rows; ++r) {
            double cy = mapAxis(r, rows, ymin, ymax);
            double zr = 0.0, zi = 0.0, mag2 = 0.0;
            int it = 0;
            while (it < maxIter) {
                double ar = std::fabs(zr);
                double ai = std::fabs(zi);
                double nzr = ar * ar - ai * ai + cx;
                double nzi = 2.0 * ar * ai + cy;
                zr = nzr;
                zi = nzi;
                mag2 = zr * zr + zi * zi;
                if (mag2 > 4.0) {
                    break;
                }
                ++it;
            }
            out[r + c * rows] = (it >= maxIter) ? 0.0 : smoothCount(it, mag2);
        }
    }
}
//=============================================================================
void
tessera_newton(int rows, int cols, double xmin, double xmax, double ymin, double ymax, int maxIter,
    double* outRoot, double* outIter)
{
    // Roots of p(z) = z^3 - 1.
    const double rootr[3] = { 1.0, -0.5, -0.5 };
    const double rooti[3] = { 0.0, 0.8660254037844386, -0.8660254037844386 };
    const double tol = 1.0e-6;
    for (int c = 0; c < cols; ++c) {
        double x0 = mapAxis(c, cols, xmin, xmax);
        for (int r = 0; r < rows; ++r) {
            double y0 = mapAxis(r, rows, ymin, ymax);
            double zr = x0;
            double zi = y0;
            int it = 0;
            int found = 0;
            for (; it < maxIter; ++it) {
                double z2r = zr * zr - zi * zi;
                double z2i = 2.0 * zr * zi;
                double z3r = z2r * zr - z2i * zi;
                double z3i = z2r * zi + z2i * zr;
                double numr = z3r - 1.0;
                double numi = z3i;
                double denr = 3.0 * z2r;
                double deni = 3.0 * z2i;
                double den = denr * denr + deni * deni;
                if (den < 1.0e-18) {
                    break;
                }
                double qr = (numr * denr + numi * deni) / den;
                double qi = (numi * denr - numr * deni) / den;
                zr -= qr;
                zi -= qi;
                for (int k = 0; k < 3; ++k) {
                    double dr = zr - rootr[k];
                    double di = zi - rooti[k];
                    if (dr * dr + di * di < tol) {
                        found = k + 1;
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }
            outRoot[r + c * rows] = static_cast<double>(found);
            outIter[r + c * rows] = static_cast<double>(it);
        }
    }
}
//=============================================================================
