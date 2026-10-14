%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% RGB = tessera.rgb_sequence(frames)
% RGB = tessera.rgb_sequence(frames, cmap)
%   Convert a rows-by-cols-by-frame scalar sequence to a
%   rows-by-cols-by-3-by-frame RGB sequence.
function RGB = rgb_sequence(frames, cmap)
  narginchk(1, 2);
  if ndims(frames) ~= 3
    error('tessera:rgb_sequence:dimensions', _('frames must be a 3-D array.'));
  end
  rows = size(frames, 1);
  cols = size(frames, 2);
  count = size(frames, 3);
  RGB = zeros(rows, cols, 3, count);
  for k = 1:count
    if nargin < 2
      RGB(:, :, :, k) = tessera.rgb(frames(:, :, k));
    else
      RGB(:, :, :, k) = tessera.rgb(frames(:, :, k), cmap);
    end
  end
end
%=============================================================================
