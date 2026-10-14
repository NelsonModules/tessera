%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================

points = [0.1 0.2; 0.5 0.5; 0.8 0.7];
paths = tessera.advect_curl(points, 12, 'size', 32, 'frequency', 5, ...
  'seed', 4, 'periodic', true);
assert_isequal(size(paths), [13 3 2]);
assert_isequal(paths(1, :, 1), points(:, 1).');
assert_isequal(paths(1, :, 2), points(:, 2).');
assert_istrue(all(paths(:) >= 0 & paths(:) <= 1));
%=============================================================================
