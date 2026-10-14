%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
F = tessera.chaos_game(2000, 'fern', 1);
assert_isequal(size(F), [2000 2]);
% the Barnsley fern lives in the upper half-plane
assert_istrue(all(F(:, 2) >= 0));
assert_isequal(F, tessera.chaos_game(2000, 'fern', 1));
%=============================================================================
S = tessera.chaos_game(2000, 'sierpinski', 1);
assert_isequal(size(S), [2000 2]);
% the Sierpinski triangle is bounded by its unit vertices
assert_istrue(all(S(:, 1) >= -1e-9 & S(:, 1) <= 1 + 1e-9));
%=============================================================================
assert_checkerror('tessera.chaos_game(10, ''nope'')', 'kind must be ''fern'' or ''sierpinski''.');
%=============================================================================
