%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% RGB = tessera.rgb_volume(volume)
% RGB = tessera.rgb_volume(volume, cmap)
%   Convert a rows-by-cols-by-planes scalar volume to RGB. The output is
%   rows-by-cols-by-planes-by-3.
function RGB = rgb_volume(volume, cmap)
  narginchk(1, 2);
  if ndims(volume) ~= 3
    error('tessera:rgb_volume:dimensions', _('volume must be a 3-D array.'));
  end
  if nargin < 2
    RGB = tessera.rgb(volume);
  else
    RGB = tessera.rgb(volume, cmap);
  end
end
%=============================================================================
