%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.gabor_noise(sz)
% R = tessera.gabor_noise(sz, impulses, frequency, seed)
% R = tessera.gabor_noise(sz, 'impulses', impulses, 'frequency', frequency, 'seed', seed)
%   Sparse Gabor-kernel noise with controllable dominant frequency.
function R = gabor_noise(sz, varargin)
  narginchk(1, 8);
  opts = struct('impulses', 64, 'frequency', 8, 'seed', 0);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) > 3
    error('tessera:gabor_noise:arguments', _('Too many positional arguments.'));
  end
  names = {'impulses', 'frequency', 'seed'};
  for k = 1:numel(pos)
    opts.(names{k}) = pos{k};
  end
  impulses = round(opts.impulses);
  frequency = opts.frequency;
  seed = round(opts.seed);
  if ~isscalar(impulses) || impulses < 1 || ~isscalar(frequency) || frequency <= 0 || ...
      ~ isscalar(seed) || seed ~= opts.seed
    error('tessera:gabor_noise:arguments', _('impulses, frequency and seed are invalid.'));
  end
  [rows, cols] = tessera_size2_gabor(sz);
  [x, y] = meshgrid(linspace(0, 1, cols), linspace(0, 1, rows));
  R = zeros(rows, cols);
  state = uint32(seed + 1);
  sigma = 0.12;
  for k = 1:impulses
    [state, cx] = tessera_rand(state);
    [state, cy] = tessera_rand(state);
    [state, theta] = tessera_rand(state);
    [state, phase] = tessera_rand(state);
    dx = x - cx;
    dy = y - cy;
    carrier = cos(2 * pi * frequency * (cos(2 * pi * theta) * dx + sin(2 * pi * theta) * dy) ...
      + 2 * pi * phase);
    envelope = exp(-(dx .^ 2 + dy .^ 2) / (2 * sigma ^ 2));
    R = R + envelope .* carrier;
  end
  R = tessera_normalize01(R);
end
%=============================================================================
function [rows, cols] = tessera_size2_gabor(sz)
  if isscalar(sz), rows = sz; cols = sz; else, rows = sz(1); cols = sz(2); end
  rows = max(1, round(rows)); cols = max(1, round(cols));
end
%=============================================================================
function [state, value] = tessera_rand(state)
  state = bitxor(state, bitshift(state, 13));
  state = bitxor(state, bitshift(state, -17));
  state = bitxor(state, bitshift(state, 5));
  value = double(bitand(state, uint32(16777215))) / double(16777216);
end
%=============================================================================
function R = tessera_normalize01(R)
  lo = min(R(:)); hi = max(R(:));
  if hi > lo, R = (R - lo) ./ (hi - lo); else, R(:) = 0; end
end
%=============================================================================
