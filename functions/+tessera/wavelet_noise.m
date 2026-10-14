%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.wavelet_noise(sz)
% R = tessera.wavelet_noise(sz, octaves, seed)
% R = tessera.wavelet_noise(sz, 'octaves', octaves, 'seed', seed)
%   Band-pass wavelet-style noise formed from octave differences.
function R = wavelet_noise(sz, varargin)
  narginchk(1, 6);
  opts = struct('octaves', 4, 'seed', 0);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) > 2
    error('tessera:wavelet_noise:arguments', _('Too many positional arguments.'));
  end
  names = {'octaves', 'seed'};
  for k = 1:numel(pos)
    opts.(names{k}) = pos{k};
  end
  octaves = round(opts.octaves);
  seed = round(opts.seed);
  if ~isscalar(octaves) || octaves < 1 || ~isscalar(seed) || seed ~= opts.seed
    error('tessera:wavelet_noise:arguments', _('octaves and seed are invalid.'));
  end
  dims = tessera_size2_wavelet(sz);
  R = zeros(dims(1), dims(2));
  for k = 1:octaves
    fine = tessera.value_noise(sz, 2 ^ (k + 1), seed + k);
    coarse = tessera.value_noise(sz, 2 ^ k, seed + k);
    R = R + (fine - coarse) ./ k;
  end
  R = tessera_normalize01_wavelet(R);
end
%=============================================================================
function dims = tessera_size2_wavelet(sz)
  if isscalar(sz), dims = [sz sz]; else, dims = [sz(1) sz(2)]; end
  dims = max(1, round(dims));
end
%=============================================================================
function R = tessera_normalize01_wavelet(R)
  lo = min(R(:)); hi = max(R(:));
  if hi > lo, R = (R - lo) ./ (hi - lo); else, R(:) = 0; end
end
%=============================================================================
