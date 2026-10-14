//=============================================================================
// Copyright (c) 2016-present Allan CORNET (Nelson)
//=============================================================================
// This file is part of the tessera module for Nelson.
//=============================================================================
// LICENCE_BLOCK_BEGIN
// SPDX-License-Identifier: LGPL-3.0-or-later
// LICENCE_BLOCK_END
//=============================================================================
#include "turtleBuiltin.hpp"
#include "TesseraBuiltinHelpers.hpp"
#include "InputOutputArgumentsCheckers.hpp"
#include "tessera.hpp"
#include <string>
//=============================================================================
namespace Nelson {
namespace Tessera {
//=============================================================================
ArrayOfVector
turtleBuiltin(int nLhs, const ArrayOfVector& argIn)
{
    nargincheck(argIn, 2, 3);
    nargoutcheck(nLhs, 0, 1);
    const ArrayOf& cmdArg = argIn[0];
    if (!cmdArg.isRowVectorCharacterArray() && !cmdArg.isScalarStringArray()) {
        Error(L"tessera:turtle:invalidCommands", L"commands must be a character vector or string.");
    }
    std::wstring w = cmdArg.asWideString();
    std::string cmd(w.begin(), w.end());
    double angle = tesseraScalarDouble(argIn[1], L"angle");
    double step = (argIn.size() > 2) ? tesseraScalarDouble(argIn[2], L"step") : 1.0;

    int len = static_cast<int>(cmd.size());
    int maxPoints = 2 * len + 16;
    std::vector<double> xs(static_cast<size_t>(maxPoints));
    std::vector<double> ys(static_cast<size_t>(maxPoints));
    int count = tessera_turtle(
        cmd.c_str(), len, angle, step, 0.0, 0.0, 90.0, xs.data(), ys.data(), maxPoints);

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
