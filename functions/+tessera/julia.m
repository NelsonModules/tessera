%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.julia(sz, c)
% R = tessera.julia(sz, c, maxiter)
% R = tessera.julia(sz, c, maxiter, [xmin xmax ymin ymax])
%   Julia escape-time field for the constant c (a complex scalar or a
%   [real imag] vector). maxiter defaults to 100, the region to [-1.6 1.6 -1.2 1.2].
function r = julia(varargin)
  r = __tessera_julia__(varargin{:});
end
%=============================================================================
