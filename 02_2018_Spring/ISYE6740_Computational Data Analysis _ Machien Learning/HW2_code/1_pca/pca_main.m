
%% QUESTION 1: EIGEN FACES
close all;
clear;
clc;

%%% load faces

path = 'yalefaces/';
subject = '14';
% if you want to see the centered picture, uncomment next line
% path = 'yalecentered/'; % an alternative dataset with centered images

[X, poses] = loadFaces(path, subject); % including .test.gif
[X14, poses14] = screening(path, X, poses); % excluding .test.gif

[nrows, ncols, N] = size(X14);
% X dimension : 61x80x10 (because we removed .test.gif)

%% calculate mean face
mu = mean(X14, 3); % mean of features

%%% display mean face and all poses

figure; colormap(gray);
subplot(3, 4, 1); imagesc(mu); title('mean');
for i = 1:11
    x = X(:, :, i);
    subplot(3, 4, i+1); imagesc(x); title(poses(i));
end
 % if this incurs error, just comment out
 % you must have bioinformatic toolbox for suptitle() 
%suptitle('Mean face & All the faces');

%% do PCA

% As N = 10, the maximum number of non-zero principal components is 10.
% we can choose a smaller number, but for completeness, here we pick 10
k = 6;
k = min(k, N);

%We find eigenvectors for 10 faces excluding test.gif
[W, S, Ac] = doPCA(X14, poses14, N, false); %for completeness
%S

%% verify orthonormality

% W is a set of Eigen vectors, which implies they are mutually orthogonal
% and of magnitude 1. Hence non-diagonal terms should be zero and 
% diagonal terms equal to 1
check = W' * W;

% size(check)
% size(W)

%% examine Eigen values

% S contains the Eigen values in its diagonal
lambda = diag(S);

% normalize the Eigen values by dividing by their sum
lambdaN = lambda / sum(lambda);
sum(lambdaN);

%how much % variance do 6 eigen values take up
six_eigen_values_take_up = sum(lambda(1:6))/sum(lambda) * 100;
fprintf("Six principal components take up %.3f (%%) of variance\n",...
    six_eigen_values_take_up);

% the normalized values show how much of the variance is captured
% by each principal component. here we plot a cumulative sum
cumLN = cumsum(lambdaN);
figure;
plot(cumLN); xlabel('no. of principal components'); ylabel('% variance');

%% visualize top k(=6) Eigen faces

figure; colormap(gray); 
subplot(2, 4, 1); imagesc(mu); title('mean');
for i = 1:6
    x = W(:, i);
    % reshape Eigen face into 2D image
    x = reshape(x, nrows, ncols);
    % display Eigen face with normalized Eigen value
    subplot(2, 4, i+1); imagesc(x); title(i);
end
% you must have bioinformatic toolbox for suptitle() 
%suptitle('Mean face & Eigen-faces'); % if this incurs error, just comment out

%%
% %% SVD approach (optional)
% % This is an alternative approach which yields the same results.
% 
% [u, s, v] = svd(Ac, 0);
% 
% Lambda = diag(s) .^ 2;
% LambdaN = Lambda / sum(Lambda);
% sum(LambdaN)
% 
% cumLN = cumsum(LambdaN);
% figure;
% plot(cumLN); xlabel('no. of principal components'); ylabel('% variance');
% 
% %%%% visualize top k Eigen faces (SVD)
% 
% figure; colormap(gray);
% subplot(2, 4, 1); imagesc(mu); title('mean');
% for i = 1:min(6, N)
%     e = u(:, i);
%     e = reshape(e, nrows, ncols);
%     subplot(2, 4, i+1); imagesc(e); title(LambdaN(i));
% end


%% QUESTION 2: FACE RECOGNITION
%%% calc Eigen faces for subjects 01 for comparison

% subject = '01';
[X01, poses01] = loadFaces(path, '01');
W01 = doPCA(X01, poses01, length(poses01), false);
% size(W01) % 11 eigenvectors

%% calculate projections of test image on PC1 of both images

% load test image into x, reshape into 1D vector
X_test = X(:, :, 10); % .test.gif
z = reshape(X_test, nrows * ncols, 1); % 4880 x 1

zTu01 = zeros(1,6);
zTu14 = zeros(1,6);
abs_zTu01 = zeros(1,6);
abs_zTu14 = zeros(1,6);
cosine_sim01 = zeros(1,6);
cosine_sim14 = zeros(1,6);

fprintf('\n<<Similarity Score>>\n');
for n = 1:6  % n-th principal component
    % calculate projection of z on first principal component for both subjects 
    zTu01(n) = z' * W01(:, n);
    zTu14(n) = z' * W(:, n);
    abs_zTu01(n) = abs(zTu01(n));
    abs_zTu14(n) = abs(zTu14(n));

    %{ 
        Interestingly, for PC1, recognition fails (it is recognized to be more
        close to subject 01) However, for PC2, PC3, it is recognized as subject
        14 correctly. 
        it's because maybe subject 01 has similar facial expression ('surprised')
        and corresponding facial is excluded from subject 14 for the test
        that is subject01 data is more rich, exhaustive.
        and another guess is when we downsized the image, it might have
        affected the test result given that results trend shows slightly
        different when we use cenetered image from non-processed image
    %}

    % reference - cosine similarities
    % the projections may be normalized and expressed as cosine similarities
     cosine_sim01(n) = zTu01(n) / (norm(z) * norm(W01(:, n)));
     cosine_sim14(n) = zTu14(n) / (norm(z) * norm(W(:, n)));
%      cosine_sim01(n), cosine_sim14(n)

    fprintf('(PC %d)\n', n);
    fprintf('> Projection - subject01 = %.5f\n\t       subject14 = %f\n',...
        abs_zTu01(n), abs_zTu14(n));
%     fprintf('>     Cosine - subject01 = %.5f\n\t       subject14 = %f\n',...
%         cosine_sim01(n), cosine_sim14(n));

    if abs_zTu01(n) > abs_zTu14(n)
        fprintf('More similar to subject "01"\n\n');
    else 
        fprintf('More similar to subject "14"\n\n');
    end

end
%% end of file



