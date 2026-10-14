//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "poisson_diskBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
poisson_diskBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 3, 5);
    nargoutcheck(nLhs, 0, 1);
    double width = tesseraScalarDouble(argIn[0], L"width");
    double height = tesseraScalarDouble(argIn[1], L"height");
    double radius = tesseraScalarDouble(argIn[2], L"radius");
    if (width <= 0.0 || height <= 0.0 || radius <= 0.0) {
        Error(L"tessera:poisson_disk:invalidArgs", L"width, height and radius must be positive.");
    }
    int k = (argIn.size() > 3) ? tesseraScalarInt(argIn[3], L"k") : 30;
    unsigned int seed
        = (argIn.size() > 4) ? static_cast<unsigned int>(tesseraScalarInt(argIn[4], L"seed")) : 0u;

    // Upper bound on the number of samples that fit at minimum spacing `radius`.
    double area = width * height;
    int maxPoints = static_cast<int>(area * 1.3 / (radius * radius)) + 64;
    std::vector<double> xs(static_cast<size_t>(maxPoints));
    std::vector<double> ys(static_cast<size_t>(maxPoints));

    int count = tessera_poisson_disk(width, height, radius, k, seed, xs.data(), ys.data(), maxPoints);

    ArrayOf out = ArrayOf::doubleMatrix2d(count, 2);
    if (count > 0) {
        double* data = static_cast<double*>(out.readWriteDataPointer());
        for (int i = 0; i < count; ++i) {
            data[i] = xs[i];
            data[i + count] = ys[i];
        }
    }
    ArrayOfVector retval;
    retval << out;
    return retval;
}
//=============================================================================
} // namespace Tessera
} // namespace Nelson
//=============================================================================
