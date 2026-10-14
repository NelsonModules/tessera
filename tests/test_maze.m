%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.maze(8, 10, 1);
assert_isequal(size(R), [17 21]);
% binary wall/passage map
assert_istrue(all(R(:) == 0 | R(:) == 1));
% outer border is all walls
assert_istrue(all(R(1, :) == 1) && all(R(end, :) == 1));
assert_istrue(all(R(:, 1) == 1) && all(R(:, end) == 1));
% every cell center is carved (a perfect maze visits all cells)
assert_isequal(R(2:2:end-1, 2:2:end-1), zeros(8, 10));
% deterministic
assert_isequal(R, tessera.maze(8, 10, 1));
%=============================================================================
