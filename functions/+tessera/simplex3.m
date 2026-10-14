%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% V = tessera.simplex3(sz)
% V = tessera.simplex3(sz, frequency, seed)
% V = tessera.simplex3(sz, 'frequency', f, 'seed', s, 'seamless', tf)
%   3-D simplex coherent noise volume in [0, 1]. sz is a scalar or
%   [rows cols planes].
function r = simplex3(varargin)
  opts.frequency = 8;
  opts.seed = 0;
  opts.seamless = false;
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) < 1, error(_('Wrong number of input arguments.')); end
  if numel(pos) > 3, error(_('Too many input arguments.')); end
  if numel(pos) >= 2, opts.frequency = pos{2}; end
  if numel(pos) >= 3, opts.seed = pos{3}; end
  r = __tessera_simplex3__(pos{1}, opts.frequency, opts.seed, double(tessera_bool(opts.seamless)));
end
%=============================================================================
