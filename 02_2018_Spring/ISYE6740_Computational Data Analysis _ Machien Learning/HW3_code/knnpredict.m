
function [yhat] = knnpredict(dist, labels, k)

    % get indices of k nearest neighbours
    % (note: when run on training data, the nearest neighbour
    %  is the point itself)
    [~, idx] = sort(dist, 2);
    knns = idx(:, 1:k);
    
    % predict using majority vote from knns
    lbls = labels(knns);
    npos = sum(lbls == 1, 2);
    nneg = sum(lbls == 0, 2);
    yhat = max(sign(npos - nneg), 0);
    
end