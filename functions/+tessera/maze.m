%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.maze(rows, cols)
% R = tessera.maze(rows, cols, seed)
%   Recursive-backtracker maze on a rows-by-cols cell grid. Returns a
%   (2*rows+1)-by-(2*cols+1) field (1 = wall, 0 = passage). seed defaults to 0.
function r = maze(varargin)
  r = __tessera_maze__(varargin{:});
end
%=============================================================================
