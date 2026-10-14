%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.domain_warp(sz)
% R = tessera.domain_warp(sz, octaves)
% R = tessera.domain_warp(sz, octaves, strength)
% R = tessera.domain_warp(sz, octaves, strength, seed)
%   Domain-warped fBm in [0, 1] (Quilez): fBm whose coordinates are displaced
%   by fBm, producing swirling, organic fields. octaves defaults to 5,
%   strength to 4, seed to 0.
function r = domain_warp(varargin)
  r = __tessera_domain_warp__(varargin{:});
end
%=============================================================================
