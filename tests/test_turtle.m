%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
% a unit square closes back on itself
P = tessera.turtle('F+F+F+F', 90, 1);
assert_isequal(size(P, 2), 2);
assert_isapprox(P(1, :), [0 0], 0, 1e-9);
assert_isapprox(P(end, :), [0 0], 0, 1e-9);
%=============================================================================
% branches insert NaN breaks in the polyline
B = tessera.turtle('F[+F][-F]F', 90, 1);
assert_istrue(any(isnan(B(:))));
%=============================================================================
% L-system expansion can be fed directly to turtle coordinates.
S = tessera.lsystem('F', {'F', 'F+F--F+F'}, 2);
K = tessera.turtle(S, 60, 1);
assert_isequal(size(K, 2), 2);
assert_isapprox(K(1, :), [0 0], 0, 1e-9);
assert_istrue(size(K, 1) > 10);
assert_istrue(all(isfinite(K(:))));
%=============================================================================
% pure / deterministic
assert_isequal(P, tessera.turtle('F+F+F+F', 90, 1));
%=============================================================================
assert_checkerror('tessera.turtle(123, 90)', _('commands must be a character vector or string.'));
%=============================================================================
