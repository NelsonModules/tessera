%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
%=============================================================================
% This file is part of the tessera module for Nelson.
%=============================================================================
% LICENCE_BLOCK_BEGIN
% SPDX-License-Identifier: LGPL-3.0-or-later
% LICENCE_BLOCK_END
%=============================================================================
% cmap = tessera.colormap(name)
% cmap = tessera.colormap(name, n)
%   Procedural-generation friendly colormaps as an n-by-3 RGB matrix in [0, 1].
%   name is one of 'terrain', 'fire', 'ice' or 'grayscale'. n defaults to 256.
function cmap = colormap(name, n)
  narginchk(1, 2);
  nargoutchk(0, 1);
  if nargin < 2
    n = 256;
  end
  mustBeInteger(n);
  mustBePositive(n);
  name = lower(char(name));
  t = linspace(0, 1, n)';
  switch name
    case 'grayscale'
      cmap = [t, t, t];
    case 'fire'
      r = min(max(t * 3, 0), 1);
      g = min(max(t * 3 - 1, 0), 1);
      b = min(max(t * 3 - 2, 0), 1);
      cmap = [r, g, b];
    case 'ice'
      r = t .^ 1.6;
      g = t .^ 1.1;
      b = min(max(0.35 + 0.65 * t, 0), 1);
      cmap = [r, g, b];
    case 'terrain'
      cmap = tessera_terrain_colormap(t);
    otherwise
      error('tessera:colormap:unknownName', _('Unknown colormap ''%s''.'), name);
  end
end
%=============================================================================
function cmap = tessera_terrain_colormap(t)
  % Deep water -> shallow water -> lowland -> hills -> mountain -> snow.
  stops = [0.00, 0.10, 0.25, 0.45, 0.75, 1.00];
  colors = [ ...
    0.02, 0.12, 0.38; ...  % deep water
    0.10, 0.35, 0.65; ...  % shallow water
    0.25, 0.55, 0.25; ...  % lowland green
    0.55, 0.50, 0.25; ...  % hills
    0.45, 0.33, 0.22; ...  % mountain brown
    0.98, 0.98, 1.00];     % snow
  cmap = zeros(numel(t), 3);
  for c = 1:3
    cmap(:, c) = interp1(stops, colors(:, c)', t, 'linear');
  end
  cmap = min(max(cmap, 0), 1);
end
%=============================================================================
