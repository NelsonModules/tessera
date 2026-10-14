%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.billow(sz)
% R = tessera.billow(sz, octaves)
% R = tessera.billow(sz, octaves, seed)
%   Billow noise in [0, 1] (puffy, cloud-like lobes). octaves defaults to 5,
%   seed to 0.
function r = billow(varargin)
  r = __tessera_billow__(varargin{:});
end
%=============================================================================
