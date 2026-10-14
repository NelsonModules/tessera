%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% S = tessera.lsystem(axiom, rules, iterations)
%   Expand an L-system (Lindenmayer system) string. axiom is the initial string,
%   rules an N-by-2 cell array of {symbol, replacement} productions, and
%   iterations the number of rewriting passes. Returns the expanded string,
%   ready for a turtle interpretation.
%
%   Example (Koch curve rewriting):
%     S = tessera.lsystem('F', {'F', 'F+F-F-F+F'}, 3);
function s = lsystem(axiom, rules, iterations)
  narginchk(3, 3);
  nargoutchk(0, 1);
  if ~iscell(rules) || size(rules, 2) ~= 2
    error('tessera:lsystem:rules', _('rules must be an N-by-2 cell array of {symbol, replacement}.'));
  end
  mustBeInteger(iterations);
  mustBeNonnegative(iterations);
  s = char(axiom);
  for it = 1:iterations
    parts = cell(1, length(s));
    for k = 1:length(s)
      c = s(k);
      rep = c;
      for r = 1:size(rules, 1)
        if strcmp(rules{r, 1}, c)
          rep = rules{r, 2};
          break;
        end
      end
      parts{k} = rep;
    end
    s = [parts{:}];
  end
end
%=============================================================================
