%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% V = tessera.fbm3(sz)
% V = tessera.fbm3(sz, octaves, seed)
% V = tessera.fbm3(sz, 'octaves', o, 'frequency', f, 'lacunarity', l, 'gain', g)
%   3-D fractional Brownian motion volume in [0, 1].
function r = fbm3(varargin)
  opts.octaves = 5;
  opts.seed = 0;
  opts.frequency = 4;
  opts.lacunarity = 2;
  opts.gain = 0.5;
  opts.seamless = false;
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) < 1, error(_('Wrong number of input arguments.')); end
  if numel(pos) > 6, error(_('Too many input arguments.')); end
  if numel(pos) >= 2, opts.octaves = pos{2}; end
  if numel(pos) >= 3, opts.seed = pos{3}; end
  if numel(pos) >= 4, opts.frequency = pos{4}; end
  if numel(pos) >= 5, opts.lacunarity = pos{5}; end
  if numel(pos) >= 6, opts.gain = pos{6}; end
  r = __tessera_fbm3__(pos{1}, opts.octaves, opts.seed, opts.frequency, opts.lacunarity, opts.gain, double(tessera_bool(opts.seamless)));
end
%=============================================================================
