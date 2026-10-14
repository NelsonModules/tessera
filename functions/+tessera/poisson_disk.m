%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% P = tessera.poisson_disk(width, height, radius)
% P = tessera.poisson_disk(width, height, radius, k)
% P = tessera.poisson_disk(width, height, radius, k, seed)
%   Bridson blue-noise sampling: points at least `radius` apart in the
%   width-by-height domain, returned as an N-by-2 [x y] matrix. k (candidates
%   per active sample) defaults to 30, seed to 0.
function r = poisson_disk(varargin)
  r = __tessera_poisson_disk__(varargin{:});
end
%=============================================================================
