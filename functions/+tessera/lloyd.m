%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.lloyd(sz)
% R = tessera.lloyd(sz, cells, iterations, seed)
% [R, sites] = tessera.lloyd(sz, 'cells', cells, 'iterations', iterations, 'seed', seed)
%   Voronoi cell-id field after Lloyd relaxation.
function varargout = lloyd(sz, varargin)
  narginchk(1, 8);
  nargoutchk(0, 2);
  opts = struct('cells', 16, 'iterations', 2, 'seed', 0);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) > 3
    error('tessera:lloyd:arguments', _('Too many positional arguments.'));
  end
  names = {'cells', 'iterations', 'seed'};
  for k = 1:numel(pos)
    opts.(names{k}) = pos{k};
  end
  n = max(nargout, 1);
  [varargout{1:n}] = __tessera_lloyd__(sz, opts.cells, opts.iterations, opts.seed);
end
%=============================================================================
