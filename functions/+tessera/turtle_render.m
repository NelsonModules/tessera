%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
% [P, W, C] = tessera.turtle_render(commands, angle, step, ...)
%   Interpret an L-system with per-symbol step, width and color parameters.
%   P contains NaN rows between branches, W contains the pen width and C the
%   RGB color associated with every vertex. Parameters are structs whose field
%   names are command symbols, for example struct('F', [1 2]).
function [P, W, C] = turtle_render(commands, angle, step, varargin)
  narginchk(2, 12);
  if nargin < 3, step = 1; end
  opts = struct('width', 1, 'color', [0.15 0.45 0.20], 'symbol_steps', struct(), ...
    'symbol_widths', struct(), 'symbol_colors', struct(), 'origin', [0 0], 'heading', 0);
  [pos, opts] = tessera_parse_options(varargin, opts);
  if ~isempty(pos)
    error('tessera:turtle_render:arguments', _('Use name-value options after step.'));
  end
  commands = char(commands);
  if ~isscalar(angle) || ~isscalar(step) || step <= 0
    error('tessera:turtle_render:geometry', _('angle must be scalar and step must be positive.'));
  end
  if numel(opts.origin) ~= 2 || numel(opts.color) ~= 3
    error('tessera:turtle_render:style', _('origin must have two elements and color three.'));
  end
  if ~isscalar(opts.width) || opts.width <= 0
    error('tessera:turtle_render:width', _('width must be positive.'));
  end
  if ~isstruct(opts.symbol_steps) || ~isstruct(opts.symbol_widths) || ...
      ~ isstruct(opts.symbol_colors)
    error('tessera:turtle_render:parameters', _('symbol parameters must be structs.'));
  end
  radians = pi / 180;
  x = opts.origin(1);
  y = opts.origin(2);
  heading = opts.heading * radians;
  penWidth = opts.width;
  penColor = opts.color(:).';
  stack = cell(0, 1);
  P = [x y];
  W = penWidth;
  C = penColor;
  for k = 1:length(commands)
    symbol = commands(k);
    if symbol == '+'
      heading = heading + angle * radians;
    elseif symbol == '-'
      heading = heading - angle * radians;
    elseif symbol == '['
      stack{end + 1} = {x, y, heading, penWidth, penColor}; %#ok<AGROW>
    elseif symbol == ']'
      if ~isempty(stack)
        state = stack{end};
        stack(end) = [];
        x = state{1}; y = state{2}; heading = state{3};
        penWidth = state{4}; penColor = state{5};
        P(end + 1, :) = [NaN NaN]; %#ok<AGROW>
        W(end + 1, 1) = penWidth; %#ok<AGROW>
        C(end + 1, :) = penColor; %#ok<AGROW>
        P(end + 1, :) = [x y]; %#ok<AGROW>
        W(end + 1, 1) = penWidth; %#ok<AGROW>
        C(end + 1, :) = penColor; %#ok<AGROW>
      end
    elseif symbol == '>'
      penWidth = penWidth * 1.25;
    elseif symbol == '<'
      penWidth = max(eps, penWidth / 1.25);
    elseif symbol == 'F' || symbol == 'G' || symbol == 'f'
      localStep = tessera_symbol_scalar(opts.symbol_steps, symbol, step);
      localWidth = tessera_symbol_scalar(opts.symbol_widths, symbol, penWidth);
      localColor = tessera_symbol_color(opts.symbol_colors, symbol, penColor);
      x = x + localStep * cos(heading);
      y = y + localStep * sin(heading);
      if symbol == 'f'
        P(end + 1, :) = [NaN NaN]; %#ok<AGROW>
        W(end + 1, 1) = localWidth; %#ok<AGROW>
        C(end + 1, :) = localColor; %#ok<AGROW>
        P(end + 1, :) = [x y]; %#ok<AGROW>
      else
        P(end + 1, :) = [x y]; %#ok<AGROW>
      end
      W(end + 1, 1) = localWidth; %#ok<AGROW>
      C(end + 1, :) = localColor; %#ok<AGROW>
    end
  end
end
%=============================================================================
function value = tessera_symbol_scalar(parameters, symbol, fallback)
  value = fallback;
  if isfield(parameters, symbol)
    candidate = parameters.(symbol);
    if isscalar(candidate) && candidate > 0
      value = candidate;
    end
  end
end
%=============================================================================
function value = tessera_symbol_color(parameters, symbol, fallback)
  value = fallback;
  if isfield(parameters, symbol)
    candidate = parameters.(symbol);
    if numel(candidate) == 3 && all(candidate(:) >= 0) && all(candidate(:) <= 1)
      value = candidate(:).';
    end
  end
end
%=============================================================================
