%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.fbm(64);
assert_isequal(size(R), [64 64]);
assert_istrue(all(R(:) >= 0 & R(:) <= 1));
%=============================================================================
A = tessera.fbm(32, 6, 42);
assert_isequal(A, tessera.fbm(32, 6, 42));
assert_isfalse(isequal(A, tessera.fbm(32, 6, 7)));
%=============================================================================
% more octaves adds detail, so the result differs from a single octave
assert_isfalse(isequal(tessera.fbm(32, 1, 1), tessera.fbm(32, 6, 1)));
%=============================================================================
