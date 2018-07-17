
function [err_train, err_test] = ModelKNN(Xtrain, ytrain, Xtest, ytest, k)

    % (no fit step in KNN)
    
    % predict on train
    dist_train = l2distances(Xtrain, Xtrain);
    yhat_train = knnpredict(dist_train, ytrain, k);
    
    % predict on test
    dist_test = l2distances(Xtest, Xtrain);
    yhat_test = knnpredict(dist_test, ytrain, k);
    
    % score
    err_train = errorscore(yhat_train, ytrain);
    err_test = errorscore(yhat_test, ytest);
    
end
