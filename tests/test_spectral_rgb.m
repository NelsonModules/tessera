%=============================================================================
% Copyright (c) 2016-present Allan CORNET (Nelson)
% SPDX-License-Identifier: LGPL-3.0-or-later
%=============================================================================

G = tessera.gabor_noise([18 20], 12, 6, 4);
assert_isequal(size(G), [18 20]);
assert_istrue(all(G(:) >= 0 & G(:) <= 1));
assert_isequal(G, tessera.gabor_noise([18 20], 12, 6, 4));
assert_isequal(G, tessera.gabor_noise([18 20], 'impulses', 12, 'frequency', 6, 'seed', 4));

W = tessera.wavelet_noise([18 20], 3, 4);
assert_isequal(size(W), [18 20]);
assert_istrue(all(W(:) >= 0 & W(:) <= 1));
assert_isequal(W, tessera.wavelet_noise([18 20], 3, 4));
assert_isequal(W, tessera.wavelet_noise([18 20], 'octaves', 3, 'seed', 4));

field = [0 0.5; 1 0.25];
cmap = [0 0 0; 1 0 0; 1 1 1];
RGB = tessera.rgb(field, cmap);
assert_isequal(size(RGB), [2 2 3]);
assert_istrue(all(RGB(:) >= 0 & RGB(:) <= 1));
assert_isequal(size(tessera.rgb(field, 'fire')), [2 2 3]);

volume = reshape(linspace(0, 1, 12), [2 2 3]);
RGBVolume = tessera.rgb_volume(volume, cmap);
assert_isequal(size(RGBVolume), [2 2 3 3]);
RGBSequence = tessera.rgb_sequence(volume, cmap);
assert_isequal(size(RGBSequence), [2 2 3 3]);

%=============================================================================
