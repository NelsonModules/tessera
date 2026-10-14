%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
function code = tessera_metric_code(value)
  if isnumeric(value)
    code = value;
    return;
  end
  v = lower(char(value));
  switch v
    case {'euclidean', 'l2'}
      code = 0;
    case {'manhattan', 'cityblock', 'l1'}
      code = 1;
    case {'chebyshev', 'chessboard', 'linf'}
      code = 2;
    otherwise
      error('tessera:worley:metric', _('metric must be ''euclidean'', ''manhattan'' or ''chebyshev''.'));
  end
end
%=============================================================================
