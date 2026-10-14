%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================

P = tessera.perlin3([8 7 5]);
assert_isequal(size(P), [8 7 5]);
assert_istrue(all(P(:) >= 0 & P(:) <= 1));

A = tessera.perlin3([6 5 4], 'frequency', 5, 'seed', 12);
B = tessera.perlin3([6 5 4], 5, 12);
assert_isequal(A, B);
assert_isfalse(isequal(A, tessera.perlin3([6 5 4], 5, 13)));

S = tessera.simplex3([7 6 5], 'frequency', 4, 'seed', 3);
assert_isequal(size(S), [7 6 5]);
assert_istrue(all(S(:) >= 0 & S(:) <= 1));
assert_isequal(S, tessera.simplex3([7 6 5], 'frequency', 4, 'seed', 3));

ST = tessera.simplex3([6 5 4], 'frequency', 3, 'seed', 3, 'seamless', true);
assert_isapprox(ST(1, :, :), ST(end, :, :), 0, 1e-12);
assert_isapprox(ST(:, 1, :), ST(:, end, :), 0, 1e-12);
assert_isapprox(ST(:, :, 1), ST(:, :, end), 0, 1e-12);

F = tessera.fbm3([5 6 4], 'octaves', 3, 'frequency', 3, 'lacunarity', 2, 'gain', 0.45, 'seed', 9);
assert_isequal(size(F), [5 6 4]);
assert_istrue(all(F(:) >= 0 & F(:) <= 1));
assert_isequal(F, tessera.fbm3([5 6 4], 3, 9, 3, 2, 0.45));

T = tessera.fbm3([5 6 4], 'seed', 9, 'seamless', true);
assert_isequal(T(1, :, :), T(end, :, :));
assert_isequal(T(:, 1, :), T(:, end, :));
assert_isequal(T(:, :, 1), T(:, :, end));

%=============================================================================
