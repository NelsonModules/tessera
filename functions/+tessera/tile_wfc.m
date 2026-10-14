%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.tile_wfc(sample, outSize)
% R = tessera.tile_wfc(sample, outSize, seed)
%   Simple tiled-model synthesis learned from immediate sample adjacencies.
function R = tile_wfc(sample, outSize, seed)
  narginchk(2, 3);
  if nargin < 3, seed = 0; end
  values = unique(sample(:));
  [rows, cols] = tessera_size2_local(outSize);
  R = zeros(rows, cols);
  state = uint32(seed + 1);
  for c = 1:cols
    for r = 1:rows
      candidates = values;
      if c > 1
        candidates = candidates(tessera_can_follow(sample, candidates, R(r, c - 1), 2));
      end
      if r > 1
        candidates = candidates(tessera_can_follow(sample, candidates, R(r - 1, c), 1));
      end
      if isempty(candidates)
        candidates = values;
      end
      [state, pick] = tessera_pick(state, numel(candidates));
      R(r, c) = candidates(pick);
    end
  end
end
%=============================================================================
function ok = tessera_can_follow(sample, candidates, previous, direction)
  ok = false(size(candidates));
  for k = 1:numel(candidates)
    value = candidates(k);
    if direction == 2
      matches = sample(:, 1:end - 1) == previous & sample(:, 2:end) == value;
    else
      matches = sample(1:end - 1, :) == previous & sample(2:end, :) == value;
    end
    ok(k) = any(matches(:));
  end
end
%=============================================================================
function [rows, cols] = tessera_size2_local(sz)
  if isscalar(sz)
    rows = sz; cols = sz;
  else
    rows = sz(1); cols = sz(2);
  end
  rows = max(1, round(rows));
  cols = max(1, round(cols));
end
%=============================================================================
function [state, idx] = tessera_pick(state, n)
  state = bitxor(state, bitshift(state, 13));
  state = bitxor(state, bitshift(state, -17));
  state = bitxor(state, bitshift(state, 5));
  idx = double(mod(state, uint32(n))) + 1;
end
%=============================================================================
