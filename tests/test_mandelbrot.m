%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.mandelbrot(64);
assert_isequal(size(R), [64 64]);
assert_istrue(all(R(:) >= 0));
assert_istrue(any(R(:) == 0));   % points inside the set
assert_istrue(any(R(:) > 0));    % points that escape
%=============================================================================
R2 = tessera.mandelbrot(32, 50, [-2 1 -1.5 1.5]);
assert_isequal(size(R2), [32 32]);
%=============================================================================
% deterministic
assert_isequal(tessera.mandelbrot(40), tessera.mandelbrot(40));
%=============================================================================
% reference values: c = 0 is inside the set (never escapes), c = 2 escapes
assert_isequal(tessera.mandelbrot(1, 100, [0 0 0 0]), 0);
assert_istrue(tessera.mandelbrot(1, 100, [2 2 0 0]) > 0);
%=============================================================================
