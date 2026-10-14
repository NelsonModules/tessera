%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.julia(64, -0.8 + 0.156i);
assert_isequal(size(R), [64 64]);
assert_istrue(all(R(:) >= 0));
%=============================================================================
% complex scalar and [real imag] vector forms agree
R2 = tessera.julia(64, [-0.8 0.156]);
assert_isequal(R, R2);
%=============================================================================
R3 = tessera.julia(32, -0.8 + 0.156i, 80, [-1.5 1.5 -1.5 1.5]);
assert_isequal(size(R3), [32 32]);
%=============================================================================
% reference: the Julia set of c = 0 is the closed unit disk
assert_isequal(tessera.julia(1, 0, 100, [0.5 0.5 0 0]), 0); % |z0| = 0.5 < 1, bounded
assert_istrue(tessera.julia(1, 0, 100, [2 2 0 0]) > 0);     % |z0| = 2 > 1, escapes
%=============================================================================
