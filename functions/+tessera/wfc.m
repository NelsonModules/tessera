%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.wfc(sample, outsize)
% R = tessera.wfc(sample, outsize, N)
% R = tessera.wfc(sample, outsize, N, seed)
% [R, info, trace] = tessera.wfc(sample, outsize, 'N', N, 'seed', seed)
%   Wave function collapse (overlapping model). Learn N-by-N patterns from the
%   integer `sample` matrix and synthesize a new field. info contains retry and
%   contradiction counts; trace contains the collapse decisions.
function varargout = wfc(sample, outsize, varargin)
  narginchk(2, 8);
  nargoutchk(0, 3);
  opts = struct('N', 2, 'seed', 0);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if numel(pos) > 2
    error('tessera:wfc:arguments', _('Too many positional arguments.'));
  end
  names = {'N', 'seed'};
  for k = 1:numel(pos)
    opts.(names{k}) = pos{k};
  end
  n = max(nargout, 1);
  [varargout{1:n}] = __tessera_wfc__(sample, outsize, opts.N, opts.seed);
end
%=============================================================================
