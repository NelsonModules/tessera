%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
function code = tessera_feature_code(value)
  if isnumeric(value)
    code = value;
    return;
  end
  v = lower(char(value));
  switch v
    case {'f1', 'nearest'}
      code = 1;
    case {'f2', 'second'}
      code = 2;
    case {'f2-f1', 'f2minusf1', 'edges', 'edge'}
      code = 3;
    otherwise
      error('tessera:worley:feature', _('feature must be ''F1'', ''F2'' or ''F2-F1''.'));
  end
end
%=============================================================================
