%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% paths = tessera.advect_curl(points, steps)
% paths = tessera.advect_curl(points, steps, 'dt', dt, 'size', sz, ...)
%   Advect normalized [x y] particle positions through a curl-noise field.
%   paths has dimensions (steps + 1)-by-particles-by-2.
function paths = advect_curl(points, steps, varargin)
  narginchk(2, 12);
  opts = struct('dt', 0.02, 'size', 128, 'frequency', 8, 'seed', 0, 'periodic', true);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if ~isempty(pos)
    names = {'dt', 'size', 'frequency', 'seed', 'periodic'};
    if numel(pos) > numel(names)
      error('tessera:advect_curl:arguments', _('Too many positional arguments.'));
    end
    for k = 1:numel(pos)
      opts.(names{k}) = pos{k};
    end
  end
  if ~isnumeric(points) || ndims(points) ~= 2 || size(points, 2) ~= 2
    error('tessera:advect_curl:points', _('points must be an N-by-2 matrix.'));
  end
  if ~isscalar(steps) || steps < 1 || steps ~= round(steps)
    error('tessera:advect_curl:steps', _('steps must be a positive integer.'));
  end
  if ~isscalar(opts.dt) || opts.dt <= 0
    error('tessera:advect_curl:dt', _('dt must be positive.'));
  end
  dims = tessera_advect_size(opts.size);
  if ~isscalar(opts.frequency) || opts.frequency <= 0
    error('tessera:advect_curl:frequency', _('frequency must be positive.'));
  end
  if ~isscalar(opts.seed) || opts.seed ~= round(opts.seed)
    error('tessera:advect_curl:seed', _('seed must be an integer scalar.'));
  end
  periodic = logical(opts.periodic);
  if any(points(:) < 0) || any(points(:) > 1)
    error('tessera:advect_curl:points', _('points must lie in the [0, 1] range.'));
  end
  [U, V] = tessera.curl(dims, opts.frequency, opts.seed);
  count = size(points, 1);
  paths = zeros(steps + 1, count, 2);
  paths(1, :, 1) = points(:, 1).';
  paths(1, :, 2) = points(:, 2).';
  for k = 1:steps
    for p = 1:count
      x = paths(k, p, 1);
      y = paths(k, p, 2);
      vx = tessera_advect_sample(U, x, y, periodic);
      vy = tessera_advect_sample(V, x, y, periodic);
      x = x + opts.dt * vx;
      y = y + opts.dt * vy;
      if periodic
        x = x - floor(x);
        y = y - floor(y);
      else
        x = min(1, max(0, x));
        y = min(1, max(0, y));
      end
      paths(k + 1, p, 1) = x;
      paths(k + 1, p, 2) = y;
    end
  end
end
%=============================================================================
function dims = tessera_advect_size(value)
  if isscalar(value)
    dims = [round(value), round(value)];
  elseif isnumeric(value) && numel(value) == 2
    dims = round(value(:).');
  else
    error('tessera:advect_curl:size', _('size must be a scalar or two-element vector.'));
  end
  if any(dims < 2)
    error('tessera:advect_curl:size', _('size must be at least 2-by-2.'));
  end
end
%=============================================================================
function value = tessera_advect_sample(field, x, y, periodic)
  rows = size(field, 1);
  cols = size(field, 2);
  if periodic
    x = x - floor(x);
    y = y - floor(y);
  else
    x = min(1, max(0, x));
    y = min(1, max(0, y));
  end
  gx = 1 + x * (cols - 1);
  gy = 1 + y * (rows - 1);
  c0 = floor(gx);
  r0 = floor(gy);
  c1 = c0 + 1;
  r1 = r0 + 1;
  if periodic
    c1 = mod(c1 - 1, cols) + 1;
    r1 = mod(r1 - 1, rows) + 1;
  else
    c1 = min(cols, c1);
    r1 = min(rows, r1);
  end
  ax = gx - c0;
  ay = gy - r0;
  v00 = field(r0, c0);
  v10 = field(r0, c1);
  v01 = field(r1, c0);
  v11 = field(r1, c1);
  value = (1 - ay) * ((1 - ax) * v00 + ax * v10) + ...
    ay * ((1 - ax) * v01 + ax * v11);
end
%=============================================================================
