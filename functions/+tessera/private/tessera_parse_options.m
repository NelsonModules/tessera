%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
function [positional, opts] = tessera_parse_options(args, opts)
  positional = {};
  k = 1;
  while k <= numel(args) && ~tessera_is_text_scalar(args{k})
    positional{end + 1} = args{k}; %#ok<AGROW>
    k = k + 1;
  end
  if mod(numel(args) - k + 1, 2) ~= 0
    error('tessera:options:nameValue', _('Name-value options must be paired.'));
  end
  names = fieldnames(opts);
  for n = k:2:numel(args)
    if ~tessera_is_text_scalar(args{n})
      error('tessera:options:name', _('Option names must be character vectors or strings.'));
    end
    key = tessera_option_key(args{n});
    matched = '';
    for j = 1:numel(names)
      if strcmp(key, tessera_option_key(names{j}))
        matched = names{j};
        break;
      end
    end
    if isempty(matched)
      error('tessera:options:unknownName', _('Unknown option ''%s''.'), char(args{n}));
    end
    opts.(matched) = args{n + 1};
  end
end
%=============================================================================
function tf = tessera_is_text_scalar(value)
  tf = ischar(value) || (isstring(value) && isscalar(value));
end
%=============================================================================
function key = tessera_option_key(value)
  key = lower(char(value));
  key = strrep(key, '_', '');
  key = strrep(key, '-', '');
end
%=============================================================================
