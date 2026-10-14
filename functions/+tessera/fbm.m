%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.fbm(sz)
% R = tessera.fbm(sz, octaves)
% R = tessera.fbm(sz, octaves, seed)
% R = tessera.fbm(sz, 'octaves', o, 'frequency', f, 'lacunarity', l, 'gain', g)
%   Fractional Brownian motion (summed Perlin octaves) as a rows-by-cols field
%   in [0, 1]. octaves defaults to 5, seed to 0 (lacunarity 2, gain 0.5).
function r = fbm(varargin)
  opts.octaves = 5;
  opts.seed = 0;
  opts.frequency = 4;
  opts.lacunarity = 2;
  opts.gain = 0.5;
  opts.seamless = false;
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) < 1, error(_('Wrong number of input arguments.')); end
  if numel(pos) > 3, error(_('Too many input arguments.')); end
  if numel(pos) >= 2, opts.octaves = pos{2}; end
  if numel(pos) >= 3, opts.seed = pos{3}; end
  r = __tessera_fbm__(pos{1}, opts.octaves, opts.seed, opts.frequency, opts.lacunarity, opts.gain, double(tessera_bool(opts.seamless)));
end
%=============================================================================
