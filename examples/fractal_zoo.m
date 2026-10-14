%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% A tour of escape-time fractals with region-aware interactive zoom.
% Each zoomed panel is rendered again for its current complex-plane window.
%=============================================================================
f = figure('Color', 'w', 'Name', 'Tessera fractal zoo', 'NumberTitle', 'off', ...
  'Position', [100 100 1100 820]);

regions = {[-2.5 1 -1.25 1.25], [-1.6 1.6 -1.2 1.2], ...
  [-2.5 1.5 -2 1], [-2 2 -2 2]};
kinds = {'mandelbrot', 'julia', 'burningship', 'newton'};
constants = {[], -0.8 + 0.156i, [], []};
maxIterations = {300, 300, 300, 50};
titles = {'Mandelbrot', 'Julia', 'Burning Ship', 'Newton'};

axesList = cell(1, 4);
images = cell(1, 4);
for k = 1:4
  axesList{k} = subplot(2, 2, k);
  data = tesseraFractalZooData(kinds{k}, 400, maxIterations{k}, ...
    constants{k}, regions{k});
  images{k} = imagesc(axesList{k}, regions{k}(1:2), regions{k}(3:4), data, ...
    'Interpolation', 'linear');
  axis(axesList{k}, 'image');
  set(axesList{k}, 'XLim', regions{k}(1:2), 'YLim', regions{k}(3:4), ...
    'YDir', 'normal');
  title(axesList{k}, titles{k});
  zoom(axesList{k}, 'on');
end
colormap(f, tessera.colormap('fire'));

state.axes = axesList;
state.images = images;
state.kinds = kinds;
state.constants = constants;
state.maxIterations = maxIterations;
state.regions = regions;
state.titles = titles;
setappdata(f, 'TesseraFractalZooState', state);
set(f, 'WindowButtonUpFcn', @tesseraFractalZooRefresh, ...
  'WindowScrollWheelFcn', @tesseraFractalZooRefresh);

%=============================================================================
function tesseraFractalZooRefresh(fig, ~)
  if ~isgraphics(fig, 'figure') || ~isappdata(fig, 'TesseraFractalZooState')
    return
  end
  state = getappdata(fig, 'TesseraFractalZooState');
  for k = 1:numel(state.axes)
    ax = state.axes{k};
    imageHandle = state.images{k};
    if ~isgraphics(ax, 'axes') || ~isgraphics(imageHandle, 'image')
      continue
    end
    region = [get(ax, 'XLim'), get(ax, 'YLim')];
    previousRegion = state.regions{k};
    if isequal(region, previousRegion)
      continue
    end
    data = tesseraFractalZooData(state.kinds{k}, 400, ...
      state.maxIterations{k}, state.constants{k}, region);
    set(imageHandle, 'CData', data, 'XData', region(1:2), ...
      'YData', region(3:4));
    set(ax, 'XLim', region(1:2), 'YLim', region(3:4));
    state.regions{k} = region;
  end
  setappdata(fig, 'TesseraFractalZooState', state);
  drawnow();
end

%=============================================================================
function data = tesseraFractalZooData(kind, sz, maxIterations, constant, region)
  switch kind
    case 'mandelbrot'
      data = tessera.mandelbrot(sz, maxIterations, region);
    case 'julia'
      data = tessera.julia(sz, constant, maxIterations, region);
    case 'burningship'
      data = tessera.burningship(sz, maxIterations, region);
    case 'newton'
      data = tessera.newton(sz, maxIterations, region);
    otherwise
      error('Tessera:fractalZoo:unknownKind', 'Unknown fractal kind.');
  end
end
