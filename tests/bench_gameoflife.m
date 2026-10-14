%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
g = double(tessera.value_noise(256, 100, 7) > 0.5);
tic();
g = tessera.gameoflife(g, 100);
toc();
%=============================================================================
