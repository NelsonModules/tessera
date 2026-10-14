//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
// The builtins are registered under hidden __tessera_<name>__ identifiers and
// re-exported to the user through the thin +tessera package wrappers in
// functions/+tessera, so the public API is the tessera.<name> namespace.
//=============================================================================
#include "NelsonGateway.hpp"
#include "perlinBuiltin.hpp"
#include "perlin3Builtin.hpp"
#include "simplexBuiltin.hpp"
#include "simplex3Builtin.hpp"
#include "value_noiseBuiltin.hpp"
#include "worleyBuiltin.hpp"
#include "worley_optionsBuiltin.hpp"
#include "curlBuiltin.hpp"
#include "fbmBuiltin.hpp"
#include "fbm3Builtin.hpp"
#include "ridgedBuiltin.hpp"
#include "billowBuiltin.hpp"
#include "turbulenceBuiltin.hpp"
#include "domain_warpBuiltin.hpp"
#include "mandelbrotBuiltin.hpp"
#include "juliaBuiltin.hpp"
#include "burningshipBuiltin.hpp"
#include "newtonfractalBuiltin.hpp"
#include "diamond_squareBuiltin.hpp"
#include "erodeBuiltin.hpp"
#include "gameoflifeBuiltin.hpp"
#include "automaton1dBuiltin.hpp"
#include "reaction_diffusionBuiltin.hpp"
#include "voronoiBuiltin.hpp"
#include "caveBuiltin.hpp"
#include "mazeBuiltin.hpp"
#include "chaos_gameBuiltin.hpp"
#include "wfcBuiltin.hpp"
#include "poisson_diskBuiltin.hpp"
#include "haltonBuiltin.hpp"
#include "turtleBuiltin.hpp"
#include "dlaBuiltin.hpp"
#include "lloydBuiltin.hpp"
//=============================================================================
using namespace Nelson;
//=============================================================================
const std::wstring gatewayName = L"tessera";
//=============================================================================
static const nlsGateway gateway[] = {
    { "__tessera_perlin__", (ptrBuiltin)Nelson::Tessera::perlinBuiltin, 1, -1 },
    { "__tessera_perlin3__", (ptrBuiltin)Nelson::Tessera::perlin3Builtin, 1, -1 },
    { "__tessera_simplex__", (ptrBuiltin)Nelson::Tessera::simplexBuiltin, 1, -1 },
    { "__tessera_simplex3__", (ptrBuiltin)Nelson::Tessera::simplex3Builtin, 1, -1 },
    { "__tessera_value_noise__", (ptrBuiltin)Nelson::Tessera::value_noiseBuiltin, 1, -1 },
    { "__tessera_worley__", (ptrBuiltin)Nelson::Tessera::worleyBuiltin, 1, -1 },
    { "__tessera_worley_options__", (ptrBuiltin)Nelson::Tessera::worley_optionsBuiltin, 1, -1 },
    { "__tessera_fbm__", (ptrBuiltin)Nelson::Tessera::fbmBuiltin, 1, -1 },
    { "__tessera_fbm3__", (ptrBuiltin)Nelson::Tessera::fbm3Builtin, 1, -1 },
    { "__tessera_ridged__", (ptrBuiltin)Nelson::Tessera::ridgedBuiltin, 1, -1 },
    { "__tessera_billow__", (ptrBuiltin)Nelson::Tessera::billowBuiltin, 1, -1 },
    { "__tessera_turbulence__", (ptrBuiltin)Nelson::Tessera::turbulenceBuiltin, 1, -1 },
    { "__tessera_domain_warp__", (ptrBuiltin)Nelson::Tessera::domain_warpBuiltin, 1, -1 },
    { "__tessera_voronoi__", (ptrBuiltin)Nelson::Tessera::voronoiBuiltin, 1, -1 },
    { "__tessera_curl__", (ptrBuiltin)Nelson::Tessera::curlBuiltin, 2, -1 },
    { "__tessera_reaction_diffusion__", (ptrBuiltin)Nelson::Tessera::reaction_diffusionBuiltin, 1, -1 },
    { "__tessera_cave__", (ptrBuiltin)Nelson::Tessera::caveBuiltin, 1, -1 },
    { "__tessera_maze__", (ptrBuiltin)Nelson::Tessera::mazeBuiltin, 1, -1 },
    { "__tessera_chaos_game__", (ptrBuiltin)Nelson::Tessera::chaos_gameBuiltin, 1, -1 },
    { "__tessera_halton__", (ptrBuiltin)Nelson::Tessera::haltonBuiltin, 1, -1 },
    { "__tessera_turtle__", (ptrBuiltin)Nelson::Tessera::turtleBuiltin, 1, -1 },
    { "__tessera_dla__", (ptrBuiltin)Nelson::Tessera::dlaBuiltin, 1, -1 },
    { "__tessera_lloyd__", (ptrBuiltin)Nelson::Tessera::lloydBuiltin, 1, -1 },
    { "__tessera_mandelbrot__", (ptrBuiltin)Nelson::Tessera::mandelbrotBuiltin, 1, -1 },
    { "__tessera_julia__", (ptrBuiltin)Nelson::Tessera::juliaBuiltin, 1, -1 },
    { "__tessera_burningship__", (ptrBuiltin)Nelson::Tessera::burningshipBuiltin, 1, -1 },
    { "__tessera_newton__", (ptrBuiltin)Nelson::Tessera::newtonfractalBuiltin, 2, -1 },
    { "__tessera_diamond_square__", (ptrBuiltin)Nelson::Tessera::diamond_squareBuiltin, 1, -1 },
    { "__tessera_erode__", (ptrBuiltin)Nelson::Tessera::erodeBuiltin, 1, -1 },
    { "__tessera_wfc__", (ptrBuiltin)Nelson::Tessera::wfcBuiltin, 1, -1 },
    { "__tessera_gameoflife__", (ptrBuiltin)Nelson::Tessera::gameoflifeBuiltin, 1, -1 },
    { "__tessera_automaton1d__", (ptrBuiltin)Nelson::Tessera::automaton1dBuiltin, 1, -1 },
    { "__tessera_poisson_disk__", (ptrBuiltin)Nelson::Tessera::poisson_diskBuiltin, 1, -1 },
};
//=============================================================================
NLS_GATEWAY_FUNC(gateway)
//=============================================================================
NLS_GATEWAY_REMOVE(gateway)
//=============================================================================
