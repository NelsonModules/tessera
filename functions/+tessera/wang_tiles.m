%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% T = tessera.wang_tiles(sz)
% T = tessera.wang_tiles(sz, colors, seed)
% T = tessera.wang_tiles(sz, 'colors', colors, 'seed', seed)
%   Generate Wang tile ids whose adjacent edge colors match.
function T = wang_tiles(sz, varargin)
  narginchk(1, 6);
  opts = struct('colors', 3, 'seed', 0);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) > 2
    error('tessera:wang_tiles:arguments', _('Too many positional arguments.'));
  end
  names = {'colors', 'seed'};
  for k = 1:numel(pos)
    opts.(names{k}) = pos{k};
  end
  colors = max(1, round(opts.colors));
  seed = round(opts.seed);
  if ~isscalar(colors) || ~isscalar(seed) || seed ~= opts.seed
    error('tessera:wang_tiles:arguments', _('colors and seed are invalid.'));
  end
  [rows, cols] = tessera_size2(sz);
  colors = max(1, round(colors));
  H = tessera_random_int(rows, cols + 1, colors, seed + 11);
  V = tessera_random_int(rows + 1, cols, colors, seed + 29);
  T = 1 + H(:, 1:cols) + colors * H(:, 2:cols + 1) + ...
    colors ^ 2 * V(1:rows, :) + colors ^ 3 * V(2:rows + 1, :);
end
%=============================================================================
function [rows, cols] = tessera_size2(sz)
  if isscalar(sz)
    rows = sz;
    cols = sz;
  else
    rows = sz(1);
    cols = sz(2);
  end
  rows = max(1, round(rows));
  cols = max(1, round(cols));
end
%=============================================================================
function R = tessera_random_int(rows, cols, colors, seed)
  modulus = 4294967296;
  state = uint32(mod(double(seed + 1) + double(0:(rows * cols - 1)) * ...
    747796405, modulus));
  state = bitxor(state, bitshift(state, -16));
  state = uint32(mod(double(state) * 2246822519, modulus));
  state = bitxor(state, bitshift(state, -13));
  state = uint32(mod(double(state) * 3266489917, modulus));
  state = bitxor(state, bitshift(state, -16));
  R = reshape(double(mod(state, uint32(colors))), rows, cols);
end
%=============================================================================
