function [ U, V ] = globalSVD( rateMatrix, lowRank )
    % Parameters
    maxIter = 200;
    learningRate = 0.0005;
    regularizer = 0.01;
    
    [n1, n2] = size(rateMatrix);
    U = rand(n1, lowRank) / lowRank;
    V = rand(n2, lowRank) / lowRank;
    [r,c,realRate] = find(rateMatrix);
    rateCount = size(realRate, 1);

    % Gradient Descent:
    round = 0;
    currErr = 9999;
    prevErr = 99999;
    while ((round < maxIter) && (currErr - prevErr < -0.0001))
        prediction = sum(U(r,:) .* V(c,:), 2);
        err = realRate - prediction;
        
        % Stochastic GD
        for k=1:rateCount
            U(r(k),:) = U(r(k),:) + learningRate*(err(k) * V(c(k),:) - regularizer * U(r(k),:));
            V(c(k),:) = V(c(k),:) + learningRate*(err(k) * U(r(k),:) - regularizer * V(c(k),:));
        end

        round = round + 1;
        
        prevErr = currErr;
        currErr = sqrt(sum(err .^ 2) / rateCount);
    end
end
