%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% RGB = tessera.rgb(field)
% RGB = tessera.rgb(field, cmap)
%   Map a scalar field in [0, 1] to an RGB image in one step. N-dimensional
%   fields keep their dimensions and receive one final RGB dimension.
function rgb = rgb(field, cmap)
  narginchk(1, 2);
  if nargin < 2
    cmap = tessera.colormap('terrain', 256);
  elseif ischar(cmap) || (isstring(cmap) && isscalar(cmap))
    cmap = tessera.colormap(cmap, 256);
  end
  n = size(cmap, 1);
  idx = round(min(max(field, 0), 1) * (n - 1)) + 1;
  dims = size(field);
  channels = cell(1, 3);
  for k = 1:3
    channel = cmap(idx(:), k);
    channels{k} = reshape(channel, dims);
  end
  rgb = cat(numel(dims) + 1, channels{:});
end
%=============================================================================
