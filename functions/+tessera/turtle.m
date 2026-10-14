%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% P = tessera.turtle(commands, angle)
% P = tessera.turtle(commands, angle, step)
%   Interpret an L-system / turtle-graphics command string as a 2-D path,
%   returned as an N-by-2 [x y] matrix (NaN rows break the polyline, e.g. at
%   branches). Commands: F/G draw forward, f moves without drawing, + turns left
%   by `angle` degrees, - turns right, [ pushes state, ] pops state. step
%   defaults to 1. Pairs with tessera.lsystem to draw plants and fractal curves.
function r = turtle(varargin)
  r = __tessera_turtle__(varargin{:});
end
%=============================================================================
