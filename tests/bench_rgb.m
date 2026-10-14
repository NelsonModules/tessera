%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
H = tessera.fbm(512, 'seed', 1);
tic();
R = tessera.rgb(H, 'terrain');
toc();
%=============================================================================
