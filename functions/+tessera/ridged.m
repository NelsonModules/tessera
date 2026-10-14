%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.ridged(sz)
% R = tessera.ridged(sz, octaves)
% R = tessera.ridged(sz, octaves, seed)
%   Ridged multifractal noise in [0, 1] (sharp ridges, mountain ranges).
%   octaves defaults to 5, seed to 0.
function r = ridged(varargin)
  r = __tessera_ridged__(varargin{:});
end
%=============================================================================
