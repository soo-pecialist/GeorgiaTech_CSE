
function [err_train, err_test] = ModelKNNm(Xtrain, ytrain, Xtest, ytest, k)

    % fit
    knn_model = fitcknn(Xtrain, ytrain, 'NumNeighbors', k);
    
    % predict
    yhat_train = predict(knn_model, Xtrain);
    yhat_test = predict(knn_model, Xtest);
    
    % score
    err_train = errorscore(yhat_train, ytrain);
    err_test = errorscore(yhat_test, ytest);
    
end
