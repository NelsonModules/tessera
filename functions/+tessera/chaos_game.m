%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% P = tessera.chaos_game(n)
% P = tessera.chaos_game(n, kind)
% P = tessera.chaos_game(n, kind, seed)
%   Iterated function system by the chaos game, returned as an n-by-2 [x y]
%   point set. kind is 'fern' (Barnsley fern, default) or 'sierpinski'
%   (Sierpinski triangle). seed defaults to 0.
function r = chaos_game(varargin)
  r = __tessera_chaos_game__(varargin{:});
end
%=============================================================================
