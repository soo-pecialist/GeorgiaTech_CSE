
function [error] = errorscore(yhat, y)

    error = sum(abs(yhat - y)) / length(y);

end