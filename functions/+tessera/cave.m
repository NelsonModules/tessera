%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.cave(sz)
% R = tessera.cave(sz, fill)
% R = tessera.cave(sz, fill, steps)
% R = tessera.cave(sz, fill, steps, seed)
%   Cellular-automaton cave map (1 = wall, 0 = floor) from a smoothed random
%   fill. fill defaults to 0.45, steps to 5, seed to 0.
function r = cave(varargin)
  r = __tessera_cave__(varargin{:});
end
%=============================================================================
