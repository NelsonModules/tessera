%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% root = tessera.newton(sz)
% [root, iter] = tessera.newton(sz, maxiter, [xmin xmax ymin ymax])
%   Newton fractal for p(z) = z^3 - 1. `root` holds the converged root index
%   (1..3, or 0 when none) and `iter` the iteration count. maxiter defaults to
%   50, the region to [-2 2 -2 2].
function varargout = newton(varargin)
  n = max(nargout, 1);
  [varargout{1:n}] = __tessera_newton__(varargin{:});
end
%=============================================================================
