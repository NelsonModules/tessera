%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.worley(sz)
% R = tessera.worley(sz, cells)
% R = tessera.worley(sz, cells, seed)
% R = tessera.worley(sz, 'cells', c, 'feature', 'F2-F1', 'metric', 'manhattan')
%   Worley (cellular) noise as a rows-by-cols field in [0, 1]: the normalized
%   distance to the nearest feature point. cells (feature points per axis)
%   defaults to 8, seed to 0.
function r = worley(varargin)
  opts.cells = 8;
  opts.seed = 0;
  opts.feature = 'F1';
  opts.metric = 'euclidean';
  opts.seamless = false;
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) < 1, error(_('Wrong number of input arguments.')); end
  if numel(pos) > 3, error(_('Too many input arguments.')); end
  if numel(pos) >= 2, opts.cells = pos{2}; end
  if numel(pos) >= 3, opts.seed = pos{3}; end
  feature = tessera_feature_code(opts.feature);
  metric = tessera_metric_code(opts.metric);
  seamless = double(tessera_bool(opts.seamless));
  if feature == 1 && metric == 0 && seamless == 0
    r = __tessera_worley__(pos{1}, opts.cells, opts.seed);
  else
    r = __tessera_worley_options__(pos{1}, opts.cells, opts.seed, feature, metric, seamless);
  end
end
%=============================================================================
