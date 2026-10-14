%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
H = tessera.fbm(128, 6, 1);
E = tessera.erode(H, 5000, 1);
assert_isequal(size(E), [128 128]);
assert_istrue(all(E(:) >= 0 & E(:) <= 1));
%=============================================================================
% erosion reshapes the terrain and is deterministic for a fixed seed
assert_isfalse(isequal(E, H));
assert_isequal(E, tessera.erode(H, 5000, 1));
%=============================================================================
assert_checkerror('tessera.erode(ones(1, 5), 10)', 'heightfield must be at least 2x2.');
%=============================================================================
