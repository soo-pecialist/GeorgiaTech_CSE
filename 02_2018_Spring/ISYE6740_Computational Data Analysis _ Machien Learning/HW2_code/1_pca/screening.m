function [X_new, poses_new] = screening(path, X, poses)

    % size of images downsampled by a factor of 4
    if strcmp(path, 'yalefaces/')
        nrows = ceil(243/4);
        ncols = ceil(320/4);
    else % yalecentered
        nrows = ceil(231/4);
        ncols = ceil(195/4);
    end %if-else

    N = length(poses); % 11
    X_new = zeros(nrows, ncols, N-1); % image holder matrix
    poses_new = cell(N-1, 1); % 10x1 cell array
    
    j = 1;
    for i = 1:N
        if strcmp(char(poses(i)), 'test')
            continue
        end
    
        X_new(:, :, j) = X(:, :, i);
        poses_new(j) = poses(i);
        j = j + 1;
    end
    
end