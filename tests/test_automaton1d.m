%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
% rule 0: a lone central cell dies immediately.
a0 = tessera.automaton1d(0, 11, 3);
assert_isequal(size(a0), [3 11]);
assert_isequal(a0(1, 6), 1);
assert_isequal(a0(2, :), zeros(1, 11));
assert_isequal(a0(3, :), zeros(1, 11));
%=============================================================================
% rule 255: every cell becomes alive on the next row.
a255 = tessera.automaton1d(255, 7, 2);
assert_isequal(a255(2, :), ones(1, 7));
%=============================================================================
% explicit initial row is used verbatim as the first generation.
init = [1 0 0 0 1];
ai = tessera.automaton1d(30, 5, 1, init);
assert_isequal(ai(1, :), init);
%=============================================================================
assert_checkerror('tessera.automaton1d(256, 10, 3)', _('rule must be an integer in the range [0, 255].'));
%=============================================================================
% reference: rule 90 is left XOR right; from a single centered cell, row 2 has
% exactly the two cells one step either side of the center.
a90 = tessera.automaton1d(90, 11, 2);
assert_isequal(sum(a90(2, :)), 2);
assert_isequal(a90(2, 5), 1);
assert_isequal(a90(2, 7), 1);
%=============================================================================
