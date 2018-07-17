function rmse = score_ridge(X, y, beta)
    yhat = predict_ridge(X, beta);
    rmse = sqrt(immse(yhat, y));
end