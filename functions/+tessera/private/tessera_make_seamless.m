%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
function r = tessera_make_seamless(r)
  dims = size(r);
  if numel(dims) < 2 || dims(1) < 2 || dims(2) < 2
    return;
  end
  if ndims(r) >= 3 && size(r, 3) > 1
    r(end, :, :) = r(1, :, :);
    r(:, end, :) = r(:, 1, :);
    r(:, :, end) = r(:, :, 1);
  else
    r(end, :) = r(1, :);
    r(:, end) = r(:, 1);
  end
end
%=============================================================================
