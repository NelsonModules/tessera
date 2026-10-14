%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
% A blinker oscillates with period 2.
g = zeros(5, 5);
g(2:4, 3) = 1;
g1 = tessera.gameoflife(g, 1);
expected = zeros(5, 5);
expected(3, 2:4) = 1;
assert_isequal(g1, expected);
%=============================================================================
% back to the vertical bar after two generations
assert_isequal(tessera.gameoflife(g, 2), g);
%=============================================================================
% default is a single generation
assert_isequal(tessera.gameoflife(g), g1);
%=============================================================================
% a 2x2 block is a still life
block = zeros(4, 4);
block(2:3, 2:3) = 1;
assert_isequal(tessera.gameoflife(block, 5), block);
%=============================================================================
% reference: a glider is translated by (1,1) after 4 generations
gl = zeros(16, 16);
gl(1, 2) = 1; gl(2, 3) = 1; gl(3, 1) = 1; gl(3, 2) = 1; gl(3, 3) = 1;
assert_isequal(tessera.gameoflife(gl, 4), circshift(gl, [1, 1]));
%=============================================================================
