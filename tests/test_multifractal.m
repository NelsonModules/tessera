%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
generators = {@tessera.ridged, @tessera.billow, @tessera.turbulence};
for g = generators
  f = g{1};
  R = f(48, 5, 1);
  assert_isequal(size(R), [48 48]);
  assert_istrue(all(R(:) >= 0 & R(:) <= 1));
  assert_isequal(R, f(48, 5, 1));
  assert_isfalse(isequal(R, f(48, 5, 9)));
end
%=============================================================================
W = tessera.domain_warp(48, 5, 4, 1);
assert_isequal(size(W), [48 48]);
assert_istrue(all(W(:) >= 0 & W(:) <= 1));
assert_isequal(W, tessera.domain_warp(48, 5, 4, 1));
assert_isfalse(isequal(W, tessera.domain_warp(48, 5, 4, 2)));
%=============================================================================
