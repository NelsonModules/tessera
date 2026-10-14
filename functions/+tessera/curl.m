%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% [U, V] = tessera.curl(sz)
% [U, V] = tessera.curl(sz, frequency)
% [U, V] = tessera.curl(sz, frequency, seed)
%   Curl noise: a divergence-free 2-D flow field (U, V) obtained as the curl of
%   a Perlin potential. Ideal for advecting particles along smooth, swirling,
%   mass-conserving flows. frequency defaults to 8, seed to 0.
function varargout = curl(varargin)
  n = max(nargout, 1);
  [varargout{1:n}] = __tessera_curl__(varargin{:});
end
%=============================================================================
