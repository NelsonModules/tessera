%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% R = tessera.reaction_diffusion(sz)
% R = tessera.reaction_diffusion(sz, steps)
% R = tessera.reaction_diffusion(sz, steps, feed, kill)
% R = tessera.reaction_diffusion(sz, steps, feed, kill, seed)
%   Gray-Scott reaction-diffusion field in [0, 1] (organic spots and stripes).
%   steps defaults to 2000, feed to 0.055, kill to 0.062, seed to 0.
function r = reaction_diffusion(varargin)
  r = __tessera_reaction_diffusion__(varargin{:});
end
%=============================================================================
