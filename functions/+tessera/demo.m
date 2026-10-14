%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
%=============================================================================
% This file is part of the tessera module for Nelson.
%=============================================================================
% LICENCE_BLOCK_BEGIN
% SPDX-License-Identifier: LGPL-3.0-or-later
% LICENCE_BLOCK_END
%=============================================================================
% tessera.demo()
% f = tessera.demo()
%   Render a gallery of tessera generators in a single figure and return its
%   handle. A compact overview of the procedural-generation suite.
function varargout = demo()
  nargoutchk(0, 1);
  f = figure();
  tiles = {...
    'perlin', tessera.perlin(256, 6, 1); ...
    'fbm', tessera.fbm(256, 6, 1); ...
    'ridged', tessera.ridged(256, 6, 1); ...
    'worley', tessera.worley(256, 10, 1); ...
    'voronoi', tessera.voronoi(256, 12, 1); ...
    'domain\_warp', tessera.domain_warp(256, 5, 4, 1); ...
    'mandelbrot', tessera.mandelbrot(256, 200); ...
    'julia', tessera.julia(256, -0.8 + 0.156i, 200); ...
    'reaction\_diffusion', tessera.reaction_diffusion(128, 2000, 0.055, 0.062, 1)};
  for k = 1:size(tiles, 1)
    subplot(3, 3, k);
    imagesc(tiles{k, 2});
    axis('equal');
    axis('tight');
    axis('off');
    title(tiles{k, 1});
  end
  colormap(tessera.colormap('terrain'));
  if nargout > 0
    varargout{1} = f;
  end
end
%=============================================================================
