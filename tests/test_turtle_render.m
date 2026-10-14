%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================

parameters = struct('F', [1 2], 'G', [0.5 1]);
colors = struct('F', [0.1 0.7 0.2], 'G', [0.8 0.2 0.1]);
[P, W, C] = tessera.turtle_render('F[+G]F', 90, 1, ...
  'symbol_steps', parameters, 'symbol_colors', colors, 'width', 2);
assert_isequal(size(P, 2), 2);
assert_isequal(size(W, 1), size(P, 1));
assert_isequal(size(C), [size(P, 1) 3]);
assert_istrue(any(isnan(P(:))));
assert_isequal(W(1), 2);
%=============================================================================
