%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.burningship(48);
assert_isequal(size(R), [48 48]);
assert_istrue(all(R(:) >= 0));
assert_istrue(any(R(:) == 0));
assert_istrue(any(R(:) > 0));
%=============================================================================
assert_isequal(size(tessera.burningship(32, 60, [-2 2 -2 2])), [32 32]);
%=============================================================================
