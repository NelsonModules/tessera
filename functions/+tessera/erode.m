%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.erode(heightfield, droplets)
% R = tessera.erode(heightfield, droplets, seed)
%   Hydraulic erosion of a heightfield by droplet simulation, returned
%   normalized to [0, 1]. droplets is the number of water droplets to simulate,
%   seed (default 0) selects their start positions.
function r = erode(varargin)
  r = __tessera_erode__(varargin{:});
end
%=============================================================================
