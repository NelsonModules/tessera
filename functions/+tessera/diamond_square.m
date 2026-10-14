%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.diamond_square(n)
% R = tessera.diamond_square(n, roughness)
% R = tessera.diamond_square(n, roughness, seed)
% R = tessera.diamond_square(n, 'roughness', roughness, 'seed', seed, 'seamless', tf)
%   Diamond-square heightfield, a ((2^n)+1)-by-((2^n)+1) field in
%   [0, 1]. roughness defaults to 0.5, seed to 0.
function r = diamond_square(varargin)
  opts.roughness = 0.5;
  opts.seed = 0;
  opts.seamless = true;
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) < 1, error(_('Wrong number of input arguments.')); end
  if numel(pos) > 3, error(_('Too many input arguments.')); end
  if numel(pos) >= 2, opts.roughness = pos{2}; end
  if numel(pos) >= 3, opts.seed = pos{3}; end
  r = __tessera_diamond_square__(pos{1}, opts.roughness, opts.seed);
  if tessera_bool(opts.seamless)
    r = tessera_make_seamless(r);
  end
end
%=============================================================================
