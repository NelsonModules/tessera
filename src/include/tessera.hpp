//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#pragma once
//=============================================================================
// Pure, dependency-free procedural generation kernels.
//
// Every 2d field is written to a caller-owned buffer in Nelson's column-major
// layout: element (row r, column c) of a rows x cols field is at out[r + c * rows].
// Noise fields are normalized to [0, 1]. Escape-time fractal fields hold the
// smooth (fractional) iteration count, with 0 meaning the point did not escape.
//=============================================================================
extern "C"
{
    //-------------------------------------------------------------------------
    // Coherent noise (single octave), normalized to [0, 1].
    //-------------------------------------------------------------------------
    void
    tessera_perlin(
        int rows, int cols, double frequency, unsigned int seed, int seamless, double* out);

    void
    tessera_simplex(
        int rows, int cols, double frequency, unsigned int seed, int seamless, double* out);

    void
    tessera_perlin3(
        int rows, int cols, int planes, double frequency, unsigned int seed, int seamless, double* out);

    void
    tessera_simplex3(
        int rows, int cols, int planes, double frequency, unsigned int seed, int seamless, double* out);

    void
    tessera_fbm3(int rows, int cols, int planes, int octaves, double frequency,
        double lacunarity, double gain, unsigned int seed, int seamless, double* out);

    void
    tessera_value_noise(int rows, int cols, double frequency, unsigned int seed, double* out);

    // Worley / cellular noise: normalized distance to the nearest of `cells`
    // feature points per axis.
    void
    tessera_worley(int rows, int cols, int cells, unsigned int seed, double* out);

    // Extended Worley output. feature: 1=F1, 2=F2, 3=F2-F1.
    // metric: 0=euclidean, 1=manhattan, 2=chebyshev. seamless != 0 wraps cells.
    void
    tessera_worley_options(int rows, int cols, int cells, unsigned int seed, int feature,
        int metric, int seamless, double* out);

    // Curl noise: a divergence-free 2d flow field (outU, outV) obtained as the
    // curl of a Perlin potential. Ideal for particle advection and flow fields.
    void
    tessera_curl(int rows, int cols, double frequency, unsigned int seed, double* outU,
        double* outV);

    //-------------------------------------------------------------------------
    // Fractal composition: fractional Brownian motion over Perlin noise.
    //-------------------------------------------------------------------------
    void
    tessera_fbm(int rows, int cols, int octaves, double frequency, double lacunarity, double gain,
        unsigned int seed, int seamless, double* out);

    //-------------------------------------------------------------------------
    // Escape-time fractals (smooth iteration count, 0 = inside the set).
    //-------------------------------------------------------------------------
    void
    tessera_mandelbrot(int rows, int cols, double xmin, double xmax, double ymin, double ymax,
        int maxIter, double* out);

    void
    tessera_julia(int rows, int cols, double cre, double cim, double xmin, double xmax, double ymin,
        double ymax, int maxIter, double* out);

    void
    tessera_burningship(int rows, int cols, double xmin, double xmax, double ymin, double ymax,
        int maxIter, double* out);

    // Newton fractal for p(z) = z^3 - 1. `outRoot` holds the converged root index
    // (1..3, or 0 when none) and `outIter` the fractional iteration count.
    void
    tessera_newton(int rows, int cols, double xmin, double xmax, double ymin, double ymax,
        int maxIter, double* outRoot, double* outIter);

    //-------------------------------------------------------------------------
    // Terrain: diamond-square midpoint displacement.
    // The output is a square field of side ((1 << n) + 1), normalized to [0, 1].
    //-------------------------------------------------------------------------
    void
    tessera_diamond_square(int n, double roughness, unsigned int seed, double* out);

    //-------------------------------------------------------------------------
    // Cellular automata.
    //-------------------------------------------------------------------------
    // Conway's game of life, toroidal, `generations` steps. in/out are rows x cols.
    void
    tessera_gameoflife(const double* in, int rows, int cols, int generations, double* out);

    // Wolfram elementary cellular automaton. Output is generations x width.
    // When hasInit is non-zero, `init` provides the first row (length width);
    // otherwise a single active cell is placed at the center.
    void
    tessera_automaton1d(int rule, int width, int generations, const double* init, int hasInit,
        unsigned int seed, double* out);

    //-------------------------------------------------------------------------
    // Multifractal composition over Perlin noise, normalized to [0, 1].
    //-------------------------------------------------------------------------
    // Ridged multifractal: sharp ridges, ideal for mountain ranges.
    void
    tessera_ridged(int rows, int cols, int octaves, double frequency, unsigned int seed,
        double* out);

    // Billow noise: puffy, cloud-like lobes.
    void
    tessera_billow(int rows, int cols, int octaves, double frequency, unsigned int seed,
        double* out);

    // Turbulence: absolute-value fBm, marble and flame-like detail.
    void
    tessera_turbulence(int rows, int cols, int octaves, double frequency, unsigned int seed,
        double* out);

    // Domain-warped fBm (Quilez): fBm whose coordinates are displaced by fBm.
    void
    tessera_domain_warp(int rows, int cols, int octaves, double frequency, double strength,
        unsigned int seed, double* out);

    //-------------------------------------------------------------------------
    // Voronoi cell-id map: index (1-based) of the nearest feature point of a
    // `cells` x `cells` jittered grid.
    //-------------------------------------------------------------------------
    void
    tessera_voronoi(int rows, int cols, int cells, unsigned int seed, double* out);

    void
    tessera_lloyd_voronoi(int rows, int cols, int cells, int iterations, unsigned int seed,
        double* out, double* outSites);

    //-------------------------------------------------------------------------
    // Gray-Scott reaction-diffusion: organic spots and stripes in [0, 1].
    //-------------------------------------------------------------------------
    void
    tessera_reaction_diffusion(int rows, int cols, int steps, double feed, double kill,
        unsigned int seed, double* out);

    //-------------------------------------------------------------------------
    // Structure generation.
    //-------------------------------------------------------------------------
    // Cellular-automaton cave: smoothed random fill, 1 = wall, 0 = floor.
    void
    tessera_cave(int rows, int cols, double fill, int steps, unsigned int seed, double* out);

    // Recursive-backtracker maze on a rows x cols cell grid. The output is a
    // (2*rows+1) x (2*cols+1) field, 1 = wall, 0 = passage.
    void
    tessera_maze(int rows, int cols, unsigned int seed, double* out);

    void
    tessera_dla(int rows, int cols, int particles, unsigned int seed, const double* obstacles,
        const double* seeds, int seedCount, double directionY, double directionX, int periodic,
        double* out);

    //-------------------------------------------------------------------------
    // Iterated function systems (chaos game). kind: 0 = Barnsley fern,
    // 1 = Sierpinski triangle. Writes n points (x, y) into xs/ys.
    //-------------------------------------------------------------------------
    void
    tessera_chaos_game(int kind, int n, unsigned int seed, double* xs, double* ys);

    //-------------------------------------------------------------------------
    // Halton low-discrepancy sequence: n points in [0, 1)^dims, column-major.
    //-------------------------------------------------------------------------
    void
    tessera_halton(int n, int dims, double* out);

    //-------------------------------------------------------------------------
    // Turtle interpreter for L-system strings. Commands: F/G draw forward,
    // f moves without drawing, + turns left, - turns right, [ pushes state,
    // ] pops state; other characters are ignored. Angle/heading are in degrees.
    // Writes up to maxPoints (x, y) vertices (NaN rows break the polyline) and
    // returns the vertex count.
    //-------------------------------------------------------------------------
    int
    tessera_turtle(const char* commands, int length, double angleDeg, double step, double x0,
        double y0, double headingDeg, double* xs, double* ys, int maxPoints);

    //-------------------------------------------------------------------------
    // Hydraulic erosion: droplet simulation carving an existing heightfield.
    // in/out are rows x cols; out is normalized to [0, 1].
    //-------------------------------------------------------------------------
    void
    tessera_erode(const double* in, int rows, int cols, int droplets, unsigned int seed,
        double* out);

    //-------------------------------------------------------------------------
    // Wave function collapse (overlapping model). Learns N x N patterns from the
    // srows x scols integer `sample` and synthesizes an outRows x outCols field.
    // Returns 1 on success, 0 if a contradiction survived every retry.
    //-------------------------------------------------------------------------
    int
    tessera_wfc(const double* sample, int srows, int scols, int n, int outRows, int outCols,
        unsigned int seed, double* out, int* retries, int* contradictions, double* maxEntropy,
        double* trace, int maxTrace, int* traceCount);

    //-------------------------------------------------------------------------
    // Blue-noise sampling: Bridson's Poisson-disk algorithm.
    // Writes up to maxPoints samples into xs/ys and returns the sample count.
    //-------------------------------------------------------------------------
    int
    tessera_poisson_disk(double width, double height, double radius, int k, unsigned int seed,
        double* xs, double* ys, int maxPoints);
}
//=============================================================================
