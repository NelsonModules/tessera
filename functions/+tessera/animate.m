%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% tessera.animate(kind)
% tessera.animate(kind, frames)
% tessera.animate(kind, 'frames', frames, 'size', size, ...)
% f = tessera.animate(...)
%   Play a procedural-generation animation in a figure. Name-value options are
%   'frames', 'size', 'palette', 'seed' and 'output'. The output option writes
%   an animated GIF while the figure remains visible.
%   kind is one of:
%     'julia'           - a Julia set morphing as its constant travels a circle
%     'mandelbrot_zoom' - an endless zoom into the Mandelbrot seahorse valley
%     'life'            - Conway's game of life evolving from a noisy seed
%     'reaction'        - a Gray-Scott reaction-diffusion pattern growing
%     'fbm3_slice'      - animated slices through a seamless 3-D fBm volume
%     'worley_edges'    - animated Worley F2-F1 edge texture
%     'turtle'          - progressive drawing of an L-system turtle curve
%     'dla_growth'      - DLA cluster growth with increasing particles
%     'lloyd_relaxation'- Voronoi sites relaxing over iterations
%     'gabor_sweep'     - Gabor noise while the carrier frequency changes
%     'wavelet_sweep'   - wavelet noise while the octave count changes
%     'curl_particles'  - particles advected by a curl-noise flow field
%     'wang_scroll'     - Wang tile map scrolling as a tiled texture
%   frames defaults to 200. Requires a graphics-capable session.
function varargout = animate(kind, varargin)
  narginchk(1, 12);
  nargoutchk(0, 1);
  opts = struct('frames', 200, 'size', [], 'palette', '', 'seed', [], 'output', '');
  [positional, opts] = tessera_parse_options(varargin, opts);
  if numel(positional) > 1
    error('tessera:animate:arguments', _('Only one positional argument is allowed.'));
  end
  if ~isempty(positional)
    opts.frames = positional{1};
  end
  if ~isnumeric(opts.frames) || ~isscalar(opts.frames) || opts.frames ~= round(opts.frames) || ...
      opts.frames < 1
    error('tessera:animate:frames', _('frames must be a positive integer.'));
  end
  opts.size = tessera_animation_size(opts.size);
  if ~isempty(opts.seed)
    if ~isnumeric(opts.seed) || ~isscalar(opts.seed) || opts.seed ~= round(opts.seed)
      error('tessera:animate:seed', _('seed must be an integer scalar.'));
    end
  end
  if ~(isempty(opts.palette) || ischar(opts.palette) || ...
      (isstring(opts.palette) && isscalar(opts.palette)) || isnumeric(opts.palette))
    error('tessera:animate:palette', _('palette must be a name or an N-by-3 matrix.'));
  end
  if ~isempty(opts.output)
    if ~(ischar(opts.output) || (isstring(opts.output) && isscalar(opts.output)))
      error('tessera:animate:output', _('output must be a file name.'));
    end
    opts.output = char(opts.output);
  end
  kind = lower(char(kind));
  f = figure('Color', 'w', 'Name', 'Tessera animation', 'NumberTitle', 'off', ...
    'Position', [100 100 960 720]);
  switch kind
    case 'julia'
      sz = tessera_animation_default_size(opts.size, 400);
      theta = linspace(0, 2 * pi, opts.frames);
      h = imagesc(tessera.julia(sz, 0.7885 * exp(1i * theta(1)), 150));
      h.Interpolation = 'linear';
      axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'fire'));
      title('tessera: Julia morph');
      tessera_animation_frame(f, opts.output, 1);
      for k = 2:opts.frames
        set(h, 'CData', tessera.julia(sz, 0.7885 * exp(1i * theta(k)), 150));
        tessera_animation_frame(f, opts.output, k);
      end
    case 'mandelbrot_zoom'
      sz = tessera_animation_default_size(opts.size, 400);
      cx = -0.743643887037151;
      cy = 0.131825904205330;
      w = 1.5;
      h = imagesc(tessera.mandelbrot(sz, 250, [cx - w, cx + w, cy - w, cy + w]));
      h.Interpolation = 'linear';
      axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'fire'));
      title('tessera: Mandelbrot zoom');
      tessera_animation_frame(f, opts.output, 1);
      for k = 2:opts.frames
        w = w * 0.95;
        set(h, 'CData', tessera.mandelbrot(sz, 250, [cx - w, cx + w, cy - w, cy + w]));
        tessera_animation_frame(f, opts.output, k);
      end
    case 'life'
      sz = tessera_animation_default_size(opts.size, 200);
      seed = tessera_animation_seed(opts.seed, 7);
      g = double(tessera.value_noise(sz, 100, seed) > 0.5);
      h = imagesc(g);
      h.Interpolation = 'nearest';
      axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'gray'));
      title('tessera: game of life');
      tessera_animation_frame(f, opts.output, 1);
      for k = 2:opts.frames
        g = tessera.gameoflife(g, 1);
        set(h, 'CData', g);
        tessera_animation_frame(f, opts.output, k);
      end
    case 'reaction'
      sz = tessera_animation_default_size(opts.size, 180);
      h = imagesc(tessera.reaction_diffusion(sz, 200, 0.055, 0.062, 1));
      h.Interpolation = 'linear';
      axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'ice'));
      title('tessera: reaction-diffusion');
      tessera_animation_frame(f, opts.output, 1);
      for k = 2:opts.frames
        set(h, 'CData', tessera.reaction_diffusion(sz, 200 * k, 0.055, 0.062, 1));
        tessera_animation_frame(f, opts.output, k);
      end
    case 'fbm3_slice'
      volumeSize = tessera_animation_volume_size(opts.size, opts.frames, 180);
      V = tessera.fbm3(volumeSize, 'octaves', 5, ...
        'seed', tessera_animation_seed(opts.seed, 42), 'seamless', true);
      h = imagesc(V(:, :, 1));
      h.Interpolation = 'linear';
      axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'terrain'));
      title('tessera: seamless 3-D fBm slices');
      tessera_animation_frame(f, opts.output, 1);
      for k = 2:opts.frames
        set(h, 'CData', V(:, :, k));
        tessera_animation_frame(f, opts.output, k);
      end
    case 'worley_edges'
      sz = tessera_animation_default_size(opts.size, 220);
      seed = tessera_animation_seed(opts.seed, 1);
      h = imagesc(tessera.worley(sz, 'cells', 12, 'seed', seed, 'feature', 'f2-f1', ...
        'metric', 'chebyshev', 'seamless', true));
      h.Interpolation = 'linear';
      axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'fire'));
      title('tessera: Worley F2-F1 edges');
      tessera_animation_frame(f, opts.output, 1);
      for k = 2:opts.frames
        set(h, 'CData', tessera.worley(sz, 'cells', 12, 'seed', seed + k - 1, ...
          'feature', 'f2-f1', ...
          'metric', 'chebyshev', 'seamless', true));
        tessera_animation_frame(f, opts.output, k);
      end
    case 'turtle'
      S = tessera.lsystem('F', {'F', 'F+F--F+F'}, 4);
      P = tessera.turtle(S, 60, 1);
      % Prime the axes with the complete curve so progressive updates keep
      % stable limits instead of zooming into the initial point.
      h = plot(P(:, 1), P(:, 2), 'LineWidth', 1.5);
      axis('equal'); axis('off');
      title('tessera: L-system turtle');
      n = size(P, 1);
      set(h, 'XData', P(1:min(2, n), 1), 'YData', P(1:min(2, n), 2));
      tessera_animation_frame(f, opts.output, 1);
      for k = 2:opts.frames
        last = max(2, min(n, round(1 + (k - 1) * (n - 1) / max(1, opts.frames - 1))));
        set(h, 'XData', P(1:last, 1), 'YData', P(1:last, 2));
        tessera_animation_frame(f, opts.output, k);
      end
    case 'dla_growth'
      animateDlaGrowth(f, opts);
    case 'lloyd_relaxation'
      animateLloydRelaxation(f, opts);
    case 'gabor_sweep'
      animateGaborSweep(f, opts);
    case 'wavelet_sweep'
      animateWaveletSweep(f, opts);
    case 'curl_particles'
      animateCurlParticles(f, opts);
    case 'wang_scroll'
      animateWangScroll(f, opts);
    otherwise
      error('tessera:animate:kind', _('unsupported animation kind.'));
  end
  if nargout > 0
    varargout{1} = f;
  end
end
%=============================================================================
function animateCurlParticles(f, opts)
  sz = tessera_animation_default_size(opts.size, 180);
  if ~isscalar(sz)
    sz = max(sz);
  end
  seed = tessera_animation_seed(opts.seed, 9);
  points = tessera.poisson_disk(1, 1, 0.055, 20, seed);
  paths = tessera.advect_curl(points, opts.frames - 1, 'size', sz, ...
    'seed', seed, 'periodic', true);
  [U, V] = tessera.curl(sz, 8, seed);
  h = imagesc(sqrt(U .^ 2 + V .^ 2));
  h.Interpolation = 'linear';
  axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'ice'));
  hold('on');
  p = plot(paths(1, :, 1) * (sz - 1) + 1, paths(1, :, 2) * (sz - 1) + 1, ...
    '.', 'Color', [1 1 1], 'MarkerSize', 12);
  title('tessera: curl particle advection');
  tessera_animation_frame(f, opts.output, 1);
  for k = 2:opts.frames
    set(p, 'XData', paths(k, :, 1) * (sz - 1) + 1, ...
      'YData', paths(k, :, 2) * (sz - 1) + 1);
    tessera_animation_frame(f, opts.output, k);
  end
  hold('off');
end
%=============================================================================
function animateDlaGrowth(f, opts)
  sz = tessera_animation_default_size(opts.size, 150);
  seed = tessera_animation_seed(opts.seed, 3);
  particles = round(linspace(20, 900, opts.frames));
  h = imagesc(tessera.dla(sz, particles(1), seed));
  h.Interpolation = 'nearest';
  axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'gray'));
  title('tessera: DLA growth');
  tessera_animation_frame(f, opts.output, 1);
  for k = 2:opts.frames
    set(h, 'CData', tessera.dla(sz, particles(k), seed));
    tessera_animation_frame(f, opts.output, k);
  end
end
%=============================================================================
function animateLloydRelaxation(f, opts)
  sz = tessera_animation_default_size(opts.size, 180);
  seed = tessera_animation_seed(opts.seed, 4);
  maxIterations = 8;
  h = imagesc(tessera.lloyd(sz, 12, 1, seed));
  h.Interpolation = 'nearest';
  axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'terrain'));
  title('tessera: Lloyd relaxation');
  tessera_animation_frame(f, opts.output, 1);
  for k = 2:opts.frames
    iterations = max(1, round(1 + (k - 1) * (maxIterations - 1) / max(1, opts.frames - 1)));
    set(h, 'CData', tessera.lloyd(sz, 12, iterations, seed));
    tessera_animation_frame(f, opts.output, k);
  end
end
%=============================================================================
function animateGaborSweep(f, opts)
  sz = tessera_animation_default_size(opts.size, 180);
  seed = tessera_animation_seed(opts.seed, 5);
  h = imagesc(tessera.gabor_noise(sz, 96, 5, seed));
  h.Interpolation = 'linear';
  axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'ice'));
  title('tessera: Gabor frequency sweep');
  tessera_animation_frame(f, opts.output, 1);
  for k = 2:opts.frames
    t = (k - 1) / max(1, opts.frames - 1);
    frequency = 5 + 5 * (1 - cos(2 * pi * t));
    set(h, 'CData', tessera.gabor_noise(sz, 96, frequency, seed));
    tessera_animation_frame(f, opts.output, k);
  end
end
%=============================================================================
function animateWaveletSweep(f, opts)
  sz = tessera_animation_default_size(opts.size, 180);
  seed = tessera_animation_seed(opts.seed, 0);
  maxOctaves = 6;
  h = imagesc(tessera.wavelet_noise(sz, 1, seed));
  h.Interpolation = 'linear';
  axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'ice'));
  title('tessera: wavelet octave sweep');
  tessera_animation_frame(f, opts.output, 1);
  for k = 2:opts.frames
    t = (k - 1) / max(1, opts.frames - 1);
    octaves = max(1, round(1 + (maxOctaves - 1) * 0.5 * (1 - cos(2 * pi * t))));
    set(h, 'CData', tessera.wavelet_noise(sz, octaves, seed));
    tessera_animation_frame(f, opts.output, k);
  end
end
%=============================================================================
function animateWangScroll(f, opts)
  tileSize = opts.size;
  if isempty(tileSize)
    tileSize = [96 96];
  end
  seed = tessera_animation_seed(opts.seed, 7);
  tiles = tessera.wang_tiles(tileSize, 4, seed);
  h = imagesc(tiles);
  h.Interpolation = 'nearest';
  axis('image'); axis('off'); colormap(tessera_animation_colormap(opts.palette, 'jet'));
  title('tessera: Wang tile scroll');
  tessera_animation_frame(f, opts.output, 1);
  for k = 2:opts.frames
    set(h, 'CData', circshift(tiles, [0, k - 1]));
    tessera_animation_frame(f, opts.output, k);
  end
end
%=============================================================================
function tessera_animation_frame(f, output, index)
  drawnow();
  if isempty(output)
    return
  end
  frameData = getframe(f).cdata;
  if index == 1
    imwrite(frameData, output, 'gif', 'LoopCount', Inf, 'DelayTime', 0.05);
  else
    imwrite(frameData, output, 'gif', 'WriteMode', 'append', 'DelayTime', 0.05);
  end
end
%=============================================================================
function dims = tessera_animation_size(value)
  if isempty(value)
    dims = [];
    return
  end
  if ~isnumeric(value) || ~(isscalar(value) || numel(value) == 2) || ...
      any(~isfinite(value(:))) || any(value(:) < 1)
    error('tessera:animate:size', _('size must be a positive scalar or two-element vector.'));
  end
  dims = round(double(value(:).'));
  if numel(dims) == 1
    dims = dims(1);
  end
end
%=============================================================================
function value = tessera_animation_default_size(value, defaultValue)
  if isempty(value)
    value = defaultValue;
  end
end
%=============================================================================
function value = tessera_animation_volume_size(value, frames, defaultValue)
  value = tessera_animation_default_size(value, defaultValue);
  if isscalar(value)
    value = [value value max(2, frames)];
  else
    value = [value max(2, frames)];
  end
end
%=============================================================================
function seed = tessera_animation_seed(value, defaultValue)
  if isempty(value)
    seed = defaultValue;
  else
    seed = double(value);
  end
end
%=============================================================================
function map = tessera_animation_colormap(palette, defaultName)
  if isempty(palette)
    palette = defaultName;
  end
  if isnumeric(palette)
    if size(palette, 2) ~= 3 || any(palette(:) < 0) || any(palette(:) > 1)
      error('tessera:animate:palette', _('Palette must have N-by-3 values in [0, 1].'));
    end
    map = palette;
    return
  end
  name = lower(char(palette));
  switch name
    case 'gray'
      map = gray(256);
    case 'jet'
      map = jet(256);
    otherwise
      map = tessera.colormap(name);
  end
end
%=============================================================================
