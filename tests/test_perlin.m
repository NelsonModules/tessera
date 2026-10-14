%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.perlin(32);
assert_isequal(size(R), [32 32]);
assert_istrue(all(R(:) >= 0 & R(:) <= 1));
%=============================================================================
assert_isequal(size(tessera.perlin([10 20])), [10 20]);
%=============================================================================
A = tessera.perlin(16, 8, 42);
assert_isequal(A, tessera.perlin(16, 8, 42));
assert_isfalse(isequal(A, tessera.perlin(16, 8, 7)));
%=============================================================================
assert_checkerror('tessera.perlin(16, 8, 1, 2)', _('Too many input arguments.'));
%=============================================================================
% reference value: Perlin gradient noise vanishes at a lattice point (-> 0.5)
assert_isequal(tessera.perlin(1, 0, 0), 0.5);
%=============================================================================
