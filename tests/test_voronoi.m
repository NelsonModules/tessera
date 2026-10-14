%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.voronoi(64, 8, 1);
assert_isequal(size(R), [64 64]);
% ids are positive integers in [1, cells^2]
assert_istrue(all(R(:) >= 1 & R(:) <= 64));
assert_isequal(R, round(R));
% more than one cell appears
assert_istrue(numel(unique(R(:))) > 1);
% deterministic
assert_isequal(R, tessera.voronoi(64, 8, 1));
%=============================================================================
