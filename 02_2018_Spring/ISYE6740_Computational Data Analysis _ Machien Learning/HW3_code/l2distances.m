
function [dist] = l2distances(X1, X2)

    % X1: N1 x d
    % X2: N2 x d
    % dist: Euclidean distances, N1 x N2
    
    N1 = size(X1, 1);
    N2 = size(X2, 1);
    
    X1sq = sum(X1 .* X1, 2); % N1 x 1
    X2sq = sum(X2 .* X2, 2); % N2 x 1
    
    X1X2 = X1 * X2'; % N1 x N2
    
    distsq = repmat(X1sq, 1, N2) + repmat(X2sq', N1, 1) - 2 * X1X2;
        
    dist = sqrt(distsq);
    
end