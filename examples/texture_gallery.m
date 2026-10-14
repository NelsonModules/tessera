%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% A compact gallery of 2-D textures, structures and a 3-D volume slice.
%=============================================================================
f = figure('Color', 'w', 'Name', 'Tessera texture gallery', 'NumberTitle', 'off', ...
  'Position', [100 100 1440 900]);
subplot(3, 4, 1);
imagesc(tessera.domain_warp(320, 5, 4, 1));
axis('image'); axis('off'); title('domain warp');
subplot(3, 4, 2);
imagesc(tessera.worley(320, 'cells', 12, 'seed', 1, 'feature', 'f2-f1', ...
  'metric', 'chebyshev', 'seamless', true));
axis('image'); axis('off'); title('worley F2-F1');
subplot(3, 4, 3);
imagesc(tessera.reaction_diffusion(256, 3000, 0.055, 0.062, 1));
axis('image'); axis('off'); title('reaction-diffusion');
subplot(3, 4, 4);
imagesc(sin(6 * pi * tessera.turbulence(320, 6, 1)));
axis('image'); axis('off'); title('marble (turbulence)');
subplot(3, 4, 5);
V = tessera.fbm3([256 256 8], 'octaves', 5, 'seed', 2, 'seamless', true);
RGB = tessera.rgb_volume(V, 'terrain');
image(squeeze(RGB(:, :, 4, :)));
axis('image'); axis('off'); title('3-D fBm / RGB slice');
subplot(3, 4, 6);
obstacles = zeros(220);
obstacles(110, 45:175) = 1;
imagesc(tessera.dla(220, 'particles', 700, 'seed', 3, ...
  'obstacles', obstacles, 'direction', [0 1], 'periodic', true));
axis('image'); axis('off'); title('DLA with barrier');
subplot(3, 4, 7);
[L, sites] = tessera.lloyd(220, 'cells', 12, 'iterations', 4, 'seed', 4);
imagesc(L);
axis('image'); axis('off'); hold('on');
plot(sites(:, 1), sites(:, 2), 'w.', 'MarkerSize', 8);
hold('off'); title('Lloyd sites');
subplot(3, 4, 8);
wang = tessera.wang_tiles([48 48], 'colors', 4, 'seed', 5);
imagesc(tesseraWangPreview(wang, 4, 5), 'Interpolation', 'linear');
axis('image'); axis('off'); title('Wang edge field');
subplot(3, 4, 9);
imagesc(tessera.gabor_noise(256, 'impulses', 72, 'frequency', 10, 'seed', 6));
axis('image'); axis('off'); title('Gabor noise');
subplot(3, 4, 10);
imagesc(tessera.wavelet_noise(256, 'octaves', 4, 'seed', 7));
axis('image'); axis('off'); title('wavelet noise');
subplot(3, 4, 11);
[U, W] = tessera.curl(256, 5, 8);
imagesc(sqrt(U .^ 2 + W .^ 2));
axis('image'); axis('off'); title('curl magnitude');
subplot(3, 4, 12);
sample = [1 1 1 2 2 2 2 1; 1 3 3 2 4 4 2 1; ...
  1 3 3 2 4 4 2 1; 2 2 2 2 2 2 2 2; ...
  2 4 4 2 3 3 2 2; 2 4 4 2 3 3 2 2; ...
  2 2 2 2 2 2 2 2; 1 1 1 2 2 2 2 1];
imagesc(tessera.wfc(sample, [64 64], 'N', 2, 'seed', 9), ...
  'Interpolation', 'linear');
axis('image'); axis('off'); title('overlapping WFC');
colormap(tessera.colormap('ice'));
axesList = findobj(f, 'Type', 'axes');
for k = 1:numel(axesList)
  zoom(axesList(k), 'on');
end
set(f, 'WindowKeyPressFcn', @tesseraTextureGalleryKeyPress);
%=============================================================================
function R = tesseraWangPreview(T, colors, tilePixels)
  ids = T - 1;
  west = mod(ids, colors);
  east = mod(floor(ids ./ colors), colors);
  north = mod(floor(ids ./ (colors ^ 2)), colors);
  south = mod(floor(ids ./ (colors ^ 3)), colors);
  [rows, cols] = size(T);
  [u, v] = meshgrid(linspace(0, 1, tilePixels), ...
    linspace(0, 1, tilePixels));
  R = zeros(rows * tilePixels, cols * tilePixels);
  for c = 1:cols
    for r = 1:rows
      horizontal = (1 - v) * north(r, c) + v * south(r, c);
      vertical = (1 - u) * west(r, c) + u * east(r, c);
      tile = 0.5 * (horizontal + vertical);
      rowRange = (r - 1) * tilePixels + (1:tilePixels);
      colRange = (c - 1) * tilePixels + (1:tilePixels);
      R(rowRange, colRange) = tile;
    end
  end
  R = R / max(1, colors - 1);
end
%=============================================================================
function tesseraTextureGalleryKeyPress(fig, event)
  ax = get(fig, 'CurrentAxes');
  if isempty(ax) || ~isgraphics(ax, 'axes') || ~isfield(event, 'Key')
    return
  end
  key = lower(char(event.Key));
  switch key
    case {'subtract', 'hyphen'}
      zoom(ax, 1 / 1.5);
    case {'home', 'r'}
      zoom(ax, 'reset');
    case {'add', 'equal'}
      zoom(ax, 1.5);
  end
end
