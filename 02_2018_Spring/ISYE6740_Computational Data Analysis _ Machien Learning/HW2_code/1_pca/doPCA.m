
function [W, S, Ac] = doPCA(X, poses, k, disp)
    % arguments: X: image matrix
    %            poses: array of strings
    %            k: number of principal components
    %            disp: boolean, whether to plot normalized faces
    % returns:   W: Eigen vectors (Principal Components), d x k
    %            S: diagonal matrix with Eigen values in diagonal, k x k
    %            Ac: normalized & centered images (used in SVD method)

    % reshape by flattening rows x columns into 1D arrays
    [nrows, ncols, N] = size(X);
    d = nrows * ncols; 
    A = reshape(X, d, N); % d x N
    A = A'; % N x d

    % normalize features by dividing by standard deviation
    stdA = std(A, 1, 1); % feature-wise (column-wise) std
    % add an epslion term before division to avoid division by zero
    epsilon = 1e-6;
    Anorm = A * diag(1./(stdA + epsilon)); % N * d
    Anorm = Anorm'; % d * N %e.g., 4880 x 11

    % center Anorm by subtracting mean
    Amean = mean(Anorm, 2); % row-wise(feature-wise) %e.g. 4880 x 1
    Ac = bsxfun(@minus, Anorm, Amean);

    % display normalized and centered faces
    if disp % true
        figure; colormap(gray); title('normalized & centered faces');
        mu = mean(X, 3);
        subplot(3, 4, 1); imagesc(mu); title('mean');
        for i = 1:11
            x = Ac(:, i);
            x = reshape(x, nrows, ncols);
            subplot(3, 4, i+1); imagesc(x); title(poses(i));
        end
    end

    % compute variance - covariance matrix
    C = Ac * Ac' ./ N; % d x d

    % calculate Eigen vectors W and Eigen values S
    k = min(k, N);
    [W, S] = eigs(C, k);
end
