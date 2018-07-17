function [U, V] = myRecommender(rateMatrix, lowRank, verbose)
    % Please type your name here:
    name = 'Soo Hyeon Kim';
    disp(name); % Do not delete this line.
    
    % Check for verbose, set false by default
    if nargin < 3
        verbose = false;
    end

    % Parameters
    maxIter = 256; % Choose your own.
    learningRate = 1e-3; % Choose your own.
    regularizer = 0.01; % Choose your own.
    stopping_criterion = 0.0001;
    
    % Random initialization:
    [nu, nm] = size(rateMatrix);
    U = rand(nu, lowRank) / lowRank;
    V = rand(nm, lowRank) / lowRank;
    
    % Get non-zero ratings
    [r, c, ratings] = find(rateMatrix);
    N = size(ratings, 1);
    
    if verbose
        disp(N / (nu*nm)); % sparsity: 0.0503
    end

    % Gradient Descent:
    currErr = 9999;
    for t = 1:maxIter
        predictions = sum(U(r, :) .* V(c, :), 2); % Nx1
        err = predictions - ratings;
        
        prevErr = currErr;
        currErr = norm(err) / sqrt(N);
        if verbose
            fprintf('iter %4d: rmse = %.4f\n', t, currErr);
        end        
        if abs(currErr - prevErr) < stopping_criterion
            break
        end
        
        % calculate gradients and perform parameter updates
        for k = 1:N
            dU_k = err(k) * V(c(k), :) + regularizer * U(r(k), :);
            dV_k = err(k) * U(r(k), :) + regularizer * V(r(k), :);
            U(r(k), :) = U(r(k), :) - learningRate * dU_k;
            V(c(k), :) = V(c(k), :) - learningRate * dV_k;
        end
                                                 
    end
end