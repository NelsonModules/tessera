%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.value_noise(32);
assert_isequal(size(R), [32 32]);
assert_istrue(all(R(:) >= 0 & R(:) <= 1));
%=============================================================================
A = tessera.value_noise(16, 8, 42);
assert_isequal(A, tessera.value_noise(16, 8, 42));
assert_isfalse(isequal(A, tessera.value_noise(16, 8, 7)));
%=============================================================================
