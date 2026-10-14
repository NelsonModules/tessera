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
#include "ArrayOf.hpp"
#include "ErrorID.hpp"
#include <cmath>
#include <vector>
//=============================================================================
namespace Nelson {
//=============================================================================
// Parse a size argument: a scalar s gives an s x s field, a [rows cols] vector
// gives a rows x cols field.
inline void
tesseraParseSize(const ArrayOf& arg, int& rows, int& cols)
{
    if (!arg.isNumeric() || arg.isEmpty()) {
        Error(L"tessera:invalidSize", L"Size must be a positive scalar or a [rows cols] vector.");
    }
    std::vector<double> v = arg.asDoubleVector();
    if (v.size() == 1) {
        rows = static_cast<int>(std::lround(v[0]));
        cols = rows;
    } else if (v.size() == 2) {
        rows = static_cast<int>(std::lround(v[0]));
        cols = static_cast<int>(std::lround(v[1]));
    } else {
        Error(L"tessera:invalidSize", L"Size must be a positive scalar or a [rows cols] vector.");
    }
    if (rows < 1 || cols < 1) {
        Error(L"tessera:invalidSize", L"Size must be a positive scalar or a [rows cols] vector.");
    }
}
//=============================================================================
// Parse a 3-D size argument: scalar s gives s x s x s, [r c p] gives a volume.
inline void
tesseraParseSize3(const ArrayOf& arg, int& rows, int& cols, int& planes)
{
    if (!arg.isNumeric() || arg.isEmpty()) {
        Error(L"tessera:invalidSize3", L"Size must be a positive scalar or a [rows cols planes] vector.");
    }
    std::vector<double> v = arg.asDoubleVector();
    if (v.size() == 1) {
        rows = static_cast<int>(std::lround(v[0]));
        cols = rows;
        planes = rows;
    } else if (v.size() == 3) {
        rows = static_cast<int>(std::lround(v[0]));
        cols = static_cast<int>(std::lround(v[1]));
        planes = static_cast<int>(std::lround(v[2]));
    } else {
        Error(L"tessera:invalidSize3", L"Size must be a positive scalar or a [rows cols planes] vector.");
    }
    if (rows < 1 || cols < 1 || planes < 1) {
        Error(L"tessera:invalidSize3", L"Size must be a positive scalar or a [rows cols planes] vector.");
    }
}
//=============================================================================
inline double
tesseraScalarDouble(const ArrayOf& arg, const std::wstring& name)
{
    if (!arg.isNumeric() || arg.isEmpty()) {
        Error(L"tessera:notScalar", L"%s must be a numeric scalar.", name);
    }
    std::vector<double> v = arg.asDoubleVector();
    if (v.size() != 1) {
        Error(L"tessera:notScalar", L"%s must be a numeric scalar.", name);
    }
    return v[0];
}
//=============================================================================
inline int
tesseraScalarInt(const ArrayOf& arg, const std::wstring& name)
{
    return static_cast<int>(std::lround(tesseraScalarDouble(arg, name)));
}
//=============================================================================
// Parse an optional 4-element region [xmin xmax ymin ymax].
inline void
tesseraParseRegion(const ArrayOf& arg, double& xmin, double& xmax, double& ymin, double& ymax)
{
    std::vector<double> v = arg.asDoubleVector();
    if (v.size() != 4) {
        Error(L"tessera:invalidRegion", L"Region must be a 4-element vector [xmin xmax ymin ymax].");
    }
    xmin = v[0];
    xmax = v[1];
    ymin = v[2];
    ymax = v[3];
}
//=============================================================================
} // namespace Nelson
//=============================================================================
