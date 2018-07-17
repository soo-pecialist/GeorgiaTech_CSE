
function [err_train, err_test] = ModelDiagonal(Xtrain, ytrain, Xtest, ytest)

    % You implment this function by assuming a diagonal covariance matrix. 
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

    % pixel variances
    pos_cov = diag(cov(Xtrain(pos_idx, :)));
    neg_cov = diag(cov(Xtrain(neg_idx, :)));

    % pixel std. dev.s
    epsilon = 1e-2;
    V_pos = diag(1 ./ sqrt((pos_cov) + epsilon));
    V_neg = diag(1 ./ sqrt((neg_cov) + epsilon));
    
    % pixel variances
    S_pos = pos_cov;
    S_neg = neg_cov;

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
