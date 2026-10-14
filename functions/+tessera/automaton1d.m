%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% A = tessera.automaton1d(rule, width, generations)
% A = tessera.automaton1d(rule, width, generations, init)
%   Wolfram elementary cellular automaton (rule 0..255). Returns a
%   generations-by-width 0/1 matrix. Without `init` a single active cell is
%   placed at the center of the first row; otherwise `init` (length width) is
%   used as the first row.
function r = automaton1d(varargin)
  r = __tessera_automaton1d__(varargin{:});
end
%=============================================================================
