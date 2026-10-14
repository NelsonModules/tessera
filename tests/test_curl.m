%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
[U, V] = tessera.curl(32, 8, 1);
assert_isequal(size(U), [32 32]);
assert_isequal(size(V), [32 32]);
%=============================================================================
% the two components form a genuine vector field (they differ)
assert_isfalse(isequal(U, V));
%=============================================================================
% deterministic for a fixed seed
[U2, V2] = tessera.curl(32, 8, 1);
assert_isequal(U, U2);
assert_isequal(V, V2);
%=============================================================================
% single-output form returns the U component
assert_isequal(tessera.curl(32, 8, 1), U);
%=============================================================================
