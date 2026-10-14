%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.perlin(sz)
% R = tessera.perlin(sz, frequency)
% R = tessera.perlin(sz, frequency, seed)
% R = tessera.perlin(sz, 'frequency', f, 'seed', s, 'seamless', tf)
%   Perlin gradient noise as a rows-by-cols field in [0, 1]. sz is a scalar
%   (square field) or a [rows cols] vector. frequency defaults to 8, seed to 0.
function r = perlin(varargin)
  opts.frequency = 8;
  opts.seed = 0;
  opts.seamless = false;
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) < 1, error(_('Wrong number of input arguments.')); end
  if numel(pos) > 3, error(_('Too many input arguments.')); end
  if numel(pos) >= 2, opts.frequency = pos{2}; end
  if numel(pos) >= 3, opts.seed = pos{3}; end
  r = __tessera_perlin__(pos{1}, opts.frequency, opts.seed, double(tessera_bool(opts.seamless)));
end
%=============================================================================
