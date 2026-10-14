%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.diamond_square(6);
assert_isequal(size(R), [65 65]);
assert_istrue(all(R(:) >= 0 & R(:) <= 1));
%=============================================================================
% side is (2^n)+1
assert_isequal(size(tessera.diamond_square(4)), [17 17]);
%=============================================================================
A = tessera.diamond_square(6, 0.5, 3);
assert_isequal(A, tessera.diamond_square(6, 0.5, 3));
assert_isfalse(isequal(A, tessera.diamond_square(6, 0.5, 9)));
%=============================================================================
