%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================

P = tessera.perlin([12 10], 'frequency', 3, 'seed', 4, 'seamless', true);
assert_isequal(size(P), [12 10]);
assert_isequal(P(1, :), P(end, :));
assert_isequal(P(:, 1), P(:, end));

assert_isequal(tessera.perlin(16, 'frequency', 4, 'seed', 7), tessera.perlin(16, 4, 7));

S = tessera.simplex([12 10], 'frequency', 3, 'seed', 4, 'seamless', true);
assert_isapprox(S(1, :), S(end, :), 0, 1e-12);
assert_isapprox(S(:, 1), S(:, end), 0, 1e-12);

F = tessera.fbm([12 10], 'octaves', 4, 'frequency', 3, 'lacunarity', 2, 'gain', 0.45, 'seed', 4, 'seamless', true);
assert_isequal(size(F), [12 10]);
assert_isequal(F(1, :), F(end, :));
assert_isequal(F(:, 1), F(:, end));

D = tessera.diamond_square(4, 'roughness', 0.5, 'seed', 3, 'seamless', true);
assert_isequal(D(1, :), D(end, :));
assert_isequal(D(:, 1), D(:, end));

Wf2 = tessera.worley(24, 'cells', 5, 'seed', 2, 'feature', 'f2', 'metric', 'manhattan');
Wedge = tessera.worley([24 20], 'cells', 5, 'seed', 2, 'feature', 'f2-f1', 'metric', 'chebyshev', 'seamless', true);
assert_isequal(size(Wf2), [24 24]);
assert_isequal(size(Wedge), [24 20]);
assert_istrue(all(Wf2(:) >= 0 & Wf2(:) <= 1));
assert_istrue(all(Wedge(:) >= 0 & Wedge(:) <= 1));
assert_isequal(Wedge(1, :), Wedge(end, :));
assert_isequal(Wedge(:, 1), Wedge(:, end));
assert_isfalse(isequal(Wf2, tessera.worley(24, 'cells', 5, 'seed', 2, 'feature', 'f2-f1')));

%=============================================================================
