%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.mandelbrot(sz)
% R = tessera.mandelbrot(sz, maxiter)
% R = tessera.mandelbrot(sz, maxiter, [xmin xmax ymin ymax])
%   Mandelbrot escape-time field (smooth iteration count, 0 inside the set).
%   maxiter defaults to 100, the region to [-2.5 1 -1.25 1.25].
function r = mandelbrot(varargin)
  r = __tessera_mandelbrot__(varargin{:});
end
%=============================================================================
