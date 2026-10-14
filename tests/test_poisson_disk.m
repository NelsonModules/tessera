%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
r = 10;
pts = tessera.poisson_disk(100, 100, r, 30, 1);
assert_isequal(size(pts, 2), 2);
assert_istrue(size(pts, 1) > 0);
%=============================================================================
% all samples lie inside the domain
assert_istrue(all(pts(:, 1) >= 0 & pts(:, 1) <= 100));
assert_istrue(all(pts(:, 2) >= 0 & pts(:, 2) <= 100));
%=============================================================================
% the minimum spacing is respected
n = size(pts, 1);
mind = Inf;
for i = 1:n
  for j = (i + 1):n
    d = sqrt((pts(i, 1) - pts(j, 1)) ^ 2 + (pts(i, 2) - pts(j, 2)) ^ 2);
    if d < mind
      mind = d;
    end
  end
end
assert_istrue(mind >= r - 1e-6);
%=============================================================================
% deterministic with a fixed seed
assert_isequal(pts, tessera.poisson_disk(100, 100, r, 30, 1));
%=============================================================================
