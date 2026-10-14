%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.dla(sz)
% R = tessera.dla(sz, particles, seed)
% R = tessera.dla(sz, 'particles', particles, 'seed', seed, ...)
%   Diffusion-limited aggregation. Options include 'obstacles', 'seeds',
%   'direction' and 'periodic'. Seeds are an N-by-2 [row col] matrix.
function r = dla(sz, varargin)
  narginchk(1, 14);
  opts = struct('particles', 200, 'seed', 0, 'obstacles', [], 'seeds', [], ...
    'direction', [0 0], 'periodic', false);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) > 2
    error('tessera:dla:arguments', _('Too many positional arguments.'));
  end
  names = {'particles', 'seed'};
  for k = 1:numel(pos)
    opts.(names{k}) = pos{k};
  end
  dims = tessera_dla_size(sz);
  obstacles = opts.obstacles;
  if isempty(obstacles)
    obstacles = zeros(dims);
  elseif ~isnumeric(obstacles) || ~isequal(size(obstacles), dims)
    error('tessera:dla:obstacles', _('obstacles must have the field size.'));
  end
  seeds = tessera_dla_seeds(opts.seeds, dims);
  direction = opts.direction;
  if ~isnumeric(direction) || numel(direction) ~= 2
    error('tessera:dla:direction', _('direction must have two elements.'));
  end
  periodic = double(opts.periodic ~= 0);
  r = __tessera_dla__(sz, opts.particles, opts.seed, obstacles, seeds, direction, periodic);
end
%=============================================================================
function dims = tessera_dla_size(sz)
  if isscalar(sz), dims = [round(sz) round(sz)]; else, dims = round(sz(1:2)); end
  if any(dims < 2), error('tessera:dla:size', _('size must be at least 2-by-2.')); end
end
%=============================================================================
function seeds = tessera_dla_seeds(value, dims)
  if isempty(value)
    seeds = zeros(0, 2);
  elseif isequal(size(value), dims)
    [rows, cols] = find(value > 0);
    seeds = [rows cols];
  elseif isnumeric(value) && size(value, 2) == 2
    seeds = round(value);
  else
    error('tessera:dla:seeds', _('seeds must be an N-by-2 matrix or a field mask.'));
  end
end
%=============================================================================
