%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
[root, iter] = tessera.newton(64);
assert_isequal(size(root), [64 64]);
assert_isequal(size(iter), [64 64]);
%=============================================================================
% root index is one of {0, 1, 2, 3}
u = unique(root(:));
assert_istrue(all(ismember(u, [0; 1; 2; 3])));
assert_istrue(any(root(:) == 1));
%=============================================================================
% single output form returns the root map
r = tessera.newton(32);
assert_isequal(size(r), [32 32]);
%=============================================================================
% reference: a start point exactly at a root converges to that root
assert_isequal(tessera.newton(1, 50, [1 1 0 0]), 1); % z0 = 1 -> root #1
%=============================================================================
