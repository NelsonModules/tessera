%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
tic();
R = tessera.perlin3([128 128 64], 'frequency', 8, 'seed', 1, 'seamless', true);
toc();
%=============================================================================
