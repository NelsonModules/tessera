%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% P = tessera.halton(n)
% P = tessera.halton(n, dims)
%   Halton low-discrepancy sequence: n points in [0, 1)^dims returned as an
%   n-by-dims matrix. dims defaults to 2 (max 16).
function r = halton(varargin)
  r = __tessera_halton__(varargin{:});
end
%=============================================================================
