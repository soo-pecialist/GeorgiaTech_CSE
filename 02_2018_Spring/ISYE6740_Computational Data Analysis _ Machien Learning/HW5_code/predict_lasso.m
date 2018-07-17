function yhat = predict_lasso(X, beta, beta0)
    yhat = beta0 + X * beta;
end