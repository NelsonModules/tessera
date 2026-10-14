%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% G = tessera.gameoflife(grid)
% G = tessera.gameoflife(grid, generations)
%   Advance Conway's game of life on a toroidal grid (0/1 matrix) by
%   `generations` steps (default 1) and return the resulting grid.
function r = gameoflife(varargin)
  r = __tessera_gameoflife__(varargin{:});
end
%=============================================================================
