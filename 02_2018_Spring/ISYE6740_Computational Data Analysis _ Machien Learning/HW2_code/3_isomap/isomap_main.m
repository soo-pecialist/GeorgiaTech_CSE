
close all;
clear;
clc;

%% load data

X = dlmread('images.txt');
[d, N] = size(X); % 4096 x 698

% transpose to have in the form N(datapoint #) x d(feature #)
X = X'; % 698 x 4096

%% visualize randomly chosen faces

% generate 16 random numbers <= (0,698] = [1, 698]
r = round(rand(1, 16) * 698);

% use random numbers as indices to images
% plot after reshaping 4096 pixel values into 64 x 64 square images
figure; colormap(gray);
for i = 1:16
    x = X(r(i), :)'; % transpose to get 4096 x 1 vector
    subplot(4, 4, i); imagesc(reshape(x, 64, 64)); title(r(i));
end

%% calculate pairwise distances

% Pairwise distances between 698 images are calculated efficiently
% using matrix operations. The squared distance between two images
% xi and xj is d-sq = (xi - xj)^2
% = xi^2 + xj^2 - 2 * xi * xj
% The above calculation is carried out for all images simultaneously
% as seen below:

Xsq = sum(X .^ 2, 2); % 698 x 1
XX = X * X'; % 698 x 698

Dsq = repmat(Xsq, 1, N) + repmat(Xsq', N, 1) - 2 * XX; % 698 x 698
% Dsq(1:20, 1:20) % to check
% Diagonal terms should be zero but are not due to rounding errors.
% set them to zero to avoid take square-roots of negative numbers
Dsq = Dsq - diag(diag(Dsq));

D = Dsq .^ 0.5;

%% plot distances as an image

figure;
imagesc(D); colorbar;

%% construction of proximity graph

% The proximity graph is returned by isomapII in the variable E.
% It would be redundant to construct E again, especially as the
% input for isomapII is the distance matrix D.

%% run isomap algorithm, display embeddings in 2D

% set option 'dijkstra' to zero as dijkstra.m file is not available
% k, the number of neighbours is a parameter than can be experimented with
% option 'overlay' may be set to zero in case of large k to avoid clutter

options = struct('dijkstra', 0); % , 'overlay', 0);
[Y, R, E] = isomapII(D, 'k', 16, options);

%% visualize proximity graph E

figure;
imagesc(E); colorbar;

%% find points close to each other in 2D

% Embedding coordinates of images are returned in Y.
% Here we use the 2D embedding coordinates to calculate pairwise distnaces.
% Instead of calculating distances manually, here we use function
% L2_distance (which is used in isomapii.m) to do the same calculation
% as before.

dist2d = L2_distance(Y.coords{2}, Y.coords{2});

% sort neighbours by distances, idx will contain the indices of neighbours
% in ascending order, with i-th column corresponding to image i
[sorted, idx] = sort(dist2d);

%% plot three closest faces for 4 randomly chosen faces

% generate four random image indices
r = round(rand(1, 4) * 698);

% plot random image, and its three closest neighbours in the 2D space
figure; colormap(gray);
for i = 1:4
    x = X(r(i), :)';
    subplot(4, 4, 4*(i-1)+1); imagesc(reshape(x, 64, 64)); title(r(i));
    for j = 2:4
        k = idx(j, r(i));
        z = X(k, :)';
        subplot(4, 4, 4*(i-1)+j); imagesc(reshape(z, 64, 64)); title(k);
    end
end

%% end of file
