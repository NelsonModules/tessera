%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
function tf = tessera_bool(value)
  if islogical(value)
    tf = value;
  elseif isnumeric(value)
    tf = value ~= 0;
  elseif ischar(value) || (isstring(value) && isscalar(value))
    v = lower(char(value));
    tf = strcmp(v, 'true') || strcmp(v, 'on') || strcmp(v, 'yes') || strcmp(v, '1');
  else
    tf = false;
  end
  tf = logical(tf);
end
%=============================================================================
