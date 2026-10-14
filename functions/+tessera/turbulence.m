%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.turbulence(sz)
% R = tessera.turbulence(sz, octaves)
% R = tessera.turbulence(sz, octaves, seed)
%   Turbulence (absolute-value fBm) in [0, 1] (marble, flame-like detail).
%   octaves defaults to 5, seed to 0.
function r = turbulence(varargin)
  r = __tessera_turbulence__(varargin{:});
end
%=============================================================================
