function rmse = score_lasso(X, y, beta, beta0)
    yhat = predict_lasso(X, beta, beta0);
    rmse = sqrt(immse(yhat, y));
end