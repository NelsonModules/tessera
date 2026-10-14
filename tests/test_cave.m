%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.cave(64, 0.45, 5, 1);
assert_isequal(size(R), [64 64]);
% binary wall/floor map
assert_istrue(all(R(:) == 0 | R(:) == 1));
% both walls and floor are present
assert_istrue(any(R(:) == 0) && any(R(:) == 1));
% borders are walls
assert_istrue(all(R(1, :) == 1) && all(R(end, :) == 1));
assert_istrue(all(R(:, 1) == 1) && all(R(:, end) == 1));
% deterministic
assert_isequal(R, tessera.cave(64, 0.45, 5, 1));
%=============================================================================
