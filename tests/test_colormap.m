%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
c = tessera.colormap('terrain');
assert_isequal(size(c), [256 3]);
assert_istrue(all(c(:) >= 0 & c(:) <= 1));
%=============================================================================
assert_isequal(size(tessera.colormap('fire', 64)), [64 3]);
assert_isequal(size(tessera.colormap('ice', 10)), [10 3]);
assert_isequal(size(tessera.colormap('grayscale', 4)), [4 3]);
%=============================================================================
g = tessera.colormap('grayscale', 3);
assert_isequal(g, [0 0 0; 0.5 0.5 0.5; 1 1 1]);
%=============================================================================
assert_checkerror('tessera.colormap(''nope'')', 'Unknown colormap ''nope''.');
%=============================================================================
