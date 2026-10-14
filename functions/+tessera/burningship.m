%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.burningship(sz)
% R = tessera.burningship(sz, maxiter)
% R = tessera.burningship(sz, maxiter, [xmin xmax ymin ymax])
%   Burning Ship escape-time field (smooth iteration count, 0 inside the set).
%   maxiter defaults to 100, the region to [-2.5 1.5 -2 1].
function r = burningship(varargin)
  r = __tessera_burningship__(varargin{:});
end
%=============================================================================
