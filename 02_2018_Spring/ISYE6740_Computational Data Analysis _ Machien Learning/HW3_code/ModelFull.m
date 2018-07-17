
function [err_train, err_test] = ModelFull(Xtrain, ytrain, Xtest, ytest)

    % You implement this function by assuming a full covariance matrix. 
    % err_train is the error rate on the train data
    % err_test is the error rate on the test data

    % FIT
    pos_idx = find(ytrain == 1);
    neg_idx = find(ytrain == 0);

    pi_pos = length(pos_idx);
    pi_neg = length(neg_idx);

    % pixel means
    pos_mean = mean(Xtrain(pos_idx, :));
    neg_mean = mean(Xtrain(neg_idx, :));

    % pixel var-covar matrices
    pos_cov = cov(Xtrain(pos_idx, :));
    neg_cov = cov(Xtrain(neg_idx, :));

    % eigen decomposition
    [U_pos, S_pos] = eig(pos_cov);
    [U_neg, S_neg] = eig(neg_cov);

    % 
    epsilon = 1e-2;
    V_pos = U_pos * diag(1 ./ sqrt(diag(S_pos) + epsilon));
    V_neg = U_neg * diag(1 ./ sqrt(diag(S_neg) + epsilon));
   
    % 
    S_pos = diag(S_pos);
    S_neg = diag(S_neg);

    % PREDICT
    pos_train = -sum(((Xtrain - repmat(pos_mean, size(Xtrain, 1), 1)) * V_pos).^2, 2)/2 - log(sum(S_pos))/2 + log(pi_pos);
    neg_train = -sum(((Xtrain - repmat(neg_mean, size(Xtrain, 1), 1)) * V_neg).^2, 2)/2 - log(sum(S_neg))/2 + log(pi_neg);

    pos_test = -sum(((Xtest - repmat(pos_mean, size(Xtest, 1), 1)) * V_pos).^2, 2)/2 - log(sum(S_pos))/2 + log(pi_pos);
    neg_test = -sum(((Xtest - repmat(neg_mean, size(Xtest, 1), 1)) * V_neg).^2, 2)/2 - log(sum(S_neg))/2 + log(pi_neg);

    yhat_train = max(sign(pos_train - neg_train), 0);
    yhat_test = max(sign(pos_test - neg_test), 0);

    % SCORE
    err_train = errorscore(yhat_train, ytrain);
    err_test = errorscore(yhat_test, ytest);

end
