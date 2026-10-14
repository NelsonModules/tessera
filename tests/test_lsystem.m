%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
% Lindenmayer's algae: A -> AB, B -> A.
S = tessera.lsystem('A', {'A', 'AB'; 'B', 'A'}, 4);
assert_isequal(S, 'ABAABABA');
%=============================================================================
% zero iterations returns the axiom unchanged
assert_isequal(tessera.lsystem('F', {'F', 'FF'}, 0), 'F');
%=============================================================================
assert_checkerror('tessera.lsystem(''A'', {''A''}, 2)', ...
  'rules must be an N-by-2 cell array of {symbol, replacement}.');
%=============================================================================
