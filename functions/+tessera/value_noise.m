%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.value_noise(sz)
% R = tessera.value_noise(sz, frequency)
% R = tessera.value_noise(sz, frequency, seed)
%   Value noise as a rows-by-cols field in [0, 1]. sz is a scalar (square field)
%   or a [rows cols] vector. frequency defaults to 8, seed to 0.
function r = value_noise(varargin)
  r = __tessera_value_noise__(varargin{:});
end
%=============================================================================
