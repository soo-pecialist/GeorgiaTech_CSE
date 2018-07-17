
function [err_train, err_test] = ModelLR(Xtrain, ytrain, Xtest, ytest)

    % fit
    B = glmfit(Xtrain, ytrain, 'binomial', 'link', 'logit', 'constant', 'off');
    
    % predict
    prob_train = glmval(B, Xtrain, 'logit', 'constant', 'off');
    yhat_train = round(prob_train);
    prob_test = glmval(B, Xtest, 'logit', 'constant', 'off');
    yhat_test = round(prob_test);
    
    % score
    err_train = errorscore(yhat_train, ytrain);
    err_test = errorscore(yhat_test, ytest);
    
end
