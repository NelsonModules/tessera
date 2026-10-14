%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
H = tessera.halton(16, 2);
assert_isequal(size(H), [16 2]);
assert_istrue(all(H(:) >= 0 & H(:) < 1));
%=============================================================================
% first point is the radical inverse of 1 in bases 2 and 3
assert_isapprox(H(1, :), [0.5, 1/3], 1e-12);
%=============================================================================
assert_isequal(size(tessera.halton(5)), [5 2]);
assert_checkerror('tessera.halton(10, 99)', 'dims must be an integer in the range [1, 16].');
%=============================================================================
