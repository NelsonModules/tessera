%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.voronoi(sz)
% R = tessera.voronoi(sz, cells)
% R = tessera.voronoi(sz, cells, seed)
%   Voronoi cell-id map: each pixel holds the 1-based index of the nearest
%   feature point of a cells-by-cells jittered grid. cells defaults to 8,
%   seed to 0.
function r = voronoi(varargin)
  r = __tessera_voronoi__(varargin{:});
end
%=============================================================================
