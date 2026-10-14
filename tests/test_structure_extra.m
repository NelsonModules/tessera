%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================

colors = 4;
T = tessera.wang_tiles([8 9], colors, 11);
assert_isequal(size(T), [8 9]);
ids = T - 1;
west = mod(ids, colors);
east = mod(floor(ids ./ colors), colors);
north = mod(floor(ids ./ (colors ^ 2)), colors);
south = mod(floor(ids ./ (colors ^ 3)), colors);
assert_isequal(east(:, 1:end - 1), west(:, 2:end));
assert_isequal(south(1:end - 1, :), north(2:end, :));
assert_isequal(T, tessera.wang_tiles([8 9], colors, 11));
assert_istrue(numel(unique(T(:))) > 1);

sample = [1 1 2; 1 2 2; 3 3 2];
M = tessera.tile_wfc(sample, [10 11], 5);
assert_isequal(size(M), [10 11]);
assert_istrue(all(ismember(unique(M(:)), unique(sample(:)))));
assert_isequal(M, tessera.tile_wfc(sample, [10 11], 5));

D = tessera.dla([31 29], 120, 3);
assert_isequal(size(D), [31 29]);
assert_istrue(all(D(:) == 0 | D(:) == 1));
assert_isequal(D(16, 15), 1);
assert_isequal(D, tessera.dla([31 29], 120, 3));

L = tessera.lloyd([24 20], 7, 2, 3);
assert_isequal(size(L), [24 20]);
assert_istrue(all(L(:) >= 1 & L(:) <= 7));
assert_isequal(L, round(L));
assert_isequal(L, tessera.lloyd([24 20], 7, 2, 3));
[L2, sites] = tessera.lloyd([24 20], 'cells', 7, 'iterations', 2, 'seed', 3);
assert_isequal(L2, L);
assert_isequal(size(sites), [7 2]);
assert_istrue(all(sites(:, 1) >= 1 & sites(:, 1) <= 20));
assert_istrue(all(sites(:, 2) >= 1 & sites(:, 2) <= 24));

obstacles = zeros(20, 20);
obstacles(10, 10) = 1;
D2 = tessera.dla([20 20], 'particles', 80, 'seed', 4, ...
  'obstacles', obstacles, 'seeds', [5 5; 15 15], 'direction', [1 0], 'periodic', true);
assert_isequal(size(D2), [20 20]);
assert_isequal(D2(10, 10), 1);
assert_isequal(D2(5, 5), 1);
assert_isequal(D2(15, 15), 1);

sampleWfc = [1 1 2; 1 2 2; 3 3 2];
[W, info, trace] = tessera.wfc(sampleWfc, [8 9], 'N', 2, 'seed', 5);
assert_isequal(size(W), [8 9]);
assert_istrue(isstruct(info));
assert_istrue(isfield(info, 'success'));
assert_istrue(isfield(info, 'contradictions'));
assert_isequal(size(trace), [72 3]);

%=============================================================================
