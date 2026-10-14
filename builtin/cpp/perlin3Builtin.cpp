//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "perlin3Builtin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
perlin3Builtin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 1, 4);
    nargoutcheck(nLhs, 0, 1);
    int rows = 0, cols = 0, planes = 0;
    tesseraParseSize3(argIn[0], rows, cols, planes);
    double frequency = (argIn.size() > 1) ? tesseraScalarDouble(argIn[1], L"frequency") : 8.0;
    unsigned int seed
        = (argIn.size() > 2) ? static_cast<unsigned int>(tesseraScalarInt(argIn[2], L"seed")) : 0u;
    int seamless = (argIn.size() > 3) ? tesseraScalarInt(argIn[3], L"seamless") : 0;
    double* data = static_cast<double*>(ArrayOf::allocateArrayOf(
        NLS_DOUBLE, static_cast<indexType>(rows) * cols * planes, stringVector(), true));
    tessera_perlin3(rows, cols, planes, frequency, seed, seamless, data);
    ArrayOf out(NLS_DOUBLE,
        Dimensions(std::vector<indexType> { static_cast<indexType>(rows), static_cast<indexType>(cols),
            static_cast<indexType>(planes) }),
        data);
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
