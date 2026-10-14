%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
R = tessera.reaction_diffusion(48, 300, 0.055, 0.062, 1);
assert_isequal(size(R), [48 48]);
assert_istrue(all(R(:) >= 0 & R(:) <= 1));
%=============================================================================
assert_isequal(R, tessera.reaction_diffusion(48, 300, 0.055, 0.062, 1));
assert_isfalse(isequal(R, tessera.reaction_diffusion(48, 300, 0.055, 0.062, 2)));
%=============================================================================
