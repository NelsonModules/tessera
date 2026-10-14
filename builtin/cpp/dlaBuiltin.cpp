//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "dlaBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
#include <vector>
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
dlaBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 7);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0;
    tesseraParseSize(argIn[0], rows, cols);
    int particles = (argIn.size() > 1) ? tesseraScalarInt(argIn[1], L"particles") : 200;
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    std::vector<double> obstacles(static_cast<size_t>(rows) * cols, 0.0);
    std::vector<double> seeds;
    double directionY = 0.0;
    double directionX = 0.0;
    int periodic = 0;
    if (argIn.size() > 3) {
        if (!argIn[3].isNumeric() || argIn[3].rows() != rows || argIn[3].columns() != cols) {
            Error(L"tessera:dla:invalidObstacles", L"obstacles must have the field size.");
        }
        obstacles = argIn[3].asDoubleVector();
    }
    if (argIn.size() > 4) {
        if (!argIn[4].isNumeric() || argIn[4].columns() != 2) {
            Error(L"tessera:dla:invalidSeeds", L"seeds must be an N-by-2 matrix.");
        }
        seeds = argIn[4].asDoubleVector();
    }
    if (argIn.size() > 5) {
        if (!argIn[5].isNumeric() || argIn[5].elementCount() != 2) {
            Error(L"tessera:dla:invalidDirection", L"direction must have two elements.");
        }
        std::vector<double> direction = argIn[5].asDoubleVector();
        directionY = direction[0];
        directionX = direction[1];
    }
    if (argIn.size() > 6) {
        periodic = tesseraScalarInt(argIn[6], L"periodic") != 0 ? 1 : 0;
    }
    ArrayOf out = ArrayOf::doubleMatrix2d(rows, cols);
    tessera_dla(rows, cols, particles, seed, obstacles.data(), seeds.empty() ? nullptr : seeds.data(),
        seeds.empty() ? 0 : static_cast<int>(seeds.size() / 2), directionY, directionX, periodic,
        static_cast<double*>(out.readWriteDataPointer()));
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
