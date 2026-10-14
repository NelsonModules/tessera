%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================
if ~ismodule('tessera')
  run([fileparts(nfilename('fullpathext')), '/../loader.m']);
end
%=============================================================================
% A periodic checkerboard sample: the output must reuse only its two values.
sample = [1 2 1 2; 2 1 2 1; 1 2 1 2; 2 1 2 1];
R = tessera.wfc(sample, [16 16], 2, 1);
assert_isequal(size(R), [16 16]);
assert_istrue(all(ismember(unique(R(:)), [1; 2])));
%=============================================================================
% deterministic for a fixed seed
assert_isequal(R, tessera.wfc(sample, [16 16], 2, 1));
%=============================================================================
% a single-value sample can only produce a constant field
assert_isequal(tessera.wfc(5 * ones(4, 4), [8 8], 2, 1), 5 * ones(8, 8));
%=============================================================================
assert_checkerror('tessera.wfc([1 2; 3 4], 8, 3)', 'sample must be at least N x N.');
%=============================================================================
