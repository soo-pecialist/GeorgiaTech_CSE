function yhat = predict_ridge(X, beta)
    % beta(1) is the intercept
    yhat = beta(1) + X * beta(2:length(beta));
end