%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% Procedural terrain: a ridged multifractal heightfield shaded as a surface
% with the built-in terrain colormap.
%=============================================================================
H = tessera.ridged([400 400], 7, 42);
figure('Color', 'w', 'Name', 'Tessera procedural terrain', 'NumberTitle', 'off', ...
  'Position', [100 100 1000 800]);
surf(H, 'EdgeColor', 'none');
shading('interp');
colormap(tessera.colormap('terrain'));
view(45, 60);
axis('tight');
title('tessera: procedural terrain (ridged multifractal)');
%=============================================================================
