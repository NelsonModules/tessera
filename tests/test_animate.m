%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================

assert_checkerror("tessera.animate('wavelet_sweep', 'frames', 0)", ...
  _('frames must be a positive integer.'));
assert_checkerror("tessera.animate('wavelet_sweep', 'unknown', 1)", ...
  _('Unknown option ''unknown''.'));
assert_checkerror("tessera.animate('wavelet_sweep', 'size', [0 16])", ...
  _('size must be a positive scalar or two-element vector.'));
%=============================================================================
