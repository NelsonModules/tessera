%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% Real-time animation: wavelet noise with a changing octave count.
% Requires a graphics-capable session.
%=============================================================================
tessera.animate('wavelet_sweep', 'frames', 120, 'size', 192, ...
  'palette', 'ice', 'seed', 3);
%=============================================================================
