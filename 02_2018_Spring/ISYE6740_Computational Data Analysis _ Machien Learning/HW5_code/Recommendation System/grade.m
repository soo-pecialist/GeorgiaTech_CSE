clear;

% Use real data:
load ('movieLens100K_split2');
rateMatrix = train;
testMatrix = test;
testMatrix2 = test2;

score = 0;

% Global SVD Test:
tic;
[U1, V1] = myRecommender(rateMatrix, 1);
logTime1 = toc;
trainRMSE1 = norm((U1*V1' - rateMatrix) .* (rateMatrix > 0), 'fro') / sqrt(nnz(rateMatrix > 0));
testRMSE1 = norm((U1*V1' - testMatrix) .* (testMatrix > 0), 'fro') / sqrt(nnz(testMatrix > 0));
test2RMSE1 = norm((U1*V1' - testMatrix2) .* (testMatrix2 > 0), 'fro') / sqrt(nnz(testMatrix2 > 0));
fprintf('SVD-%d\t%.4f\t%.4f\t%.4f\t%.2f\n', 1, trainRMSE1, testRMSE1, test2RMSE1, logTime1);

tic;
[U6, V6] = myRecommender(rateMatrix, 6);
logTime6 = toc;
trainRMSE6 = norm((U6*V6' - rateMatrix) .* (rateMatrix > 0), 'fro') / sqrt(nnz(rateMatrix > 0));
testRMSE6 = norm((U6*V6' - testMatrix) .* (testMatrix > 0), 'fro') / sqrt(nnz(testMatrix > 0));
test2RMSE6 = norm((U6*V6' - testMatrix2) .* (testMatrix2 > 0), 'fro') / sqrt(nnz(testMatrix2 > 0));
fprintf('SVD-%d\t%.4f\t%.4f\t%.4f\t%.2f\n', 6, trainRMSE6, testRMSE6, test2RMSE6, logTime6);

fprintf('4(code): ');

% size test (4 pts)
if (size(U1, 1) == size(rateMatrix, 1))
    score = score + 6;
else
    fprintf('[Size Error 1] Number of rows in U does not match to user count. (-0.6pts) ');
end
if (size(U1, 2) == 1)
    score = score + 4;
else
    fprintf('[Size Error 2] Number of columns in U does not match to lowRank. (-0.4pts) ');
end
if (size(V1, 1) == size(rateMatrix, 2))
    score = score + 6;
else
    fprintf('[Size Error 3] Number of rows in V does not match to item count. (-0.6pts) ');
end
if (size(V1, 2) == 1)
    score = score + 4;
else
    fprintf('[Size Error 4] Number of columns in V does not match to lowRank. (-0.4pts) ');
end
if (size(U6, 1) == size(rateMatrix, 1))
    score = score + 6;
else
    fprintf('[Size Error 5] Number of rows in U does not match to user count. (-0.6pts) ');
end
if (size(U6, 2) == 6)
    score = score + 4;
else
    fprintf('[Size Error 6] Number of columns in U does not match to lowRank. (-0.4pts) ');
end
if (size(V6, 1) == size(rateMatrix, 2))
    score = score + 6;
else
    fprintf('[Size Error 7] Number of rows in V does not match to item count. (-0.6pts) ');
end
if (size(V6, 2) == 6)
    score = score + 4;
else
    fprintf('[Size Error 8] Number of columns in V does not match to lowRank. (-0.4pts) ');
end

% Time (2 pt)
timeScore1 = floor(10 / (ceil(logTime1/20)));
timeScore6 = floor(10 / (ceil(logTime6/50)));
score = score + timeScore1 + timeScore6;
if (timeScore1 + timeScore6 < 20)
    fprintf('[Time] Elapsed time is %.1f sec. (-%.1fpts) ', logTime1 + logTime6, (10-timeScore1-timeScore6)/10);
end

% Evaluation (9 pts)
evalScore1 = min(max(floor(-400 * test2RMSE1 + 425), 0), 40);
evalScore6 = min(max(floor(-500 * test2RMSE6 + 530), 0), 50);
score = score + evalScore1 + evalScore6;
if (evalScore1 + evalScore6 < 90)
    fprintf('Test RMSE is %.4f (rank=1) and %.4f (rank=6). (-%.1fpts)\n', test2RMSE1, test2RMSE6, (90-evalScore1-evalScore6)/10);
else
    fprintf('Test RMSE is %.4f (rank=1) and %.4f (rank=6).\n', test2RMSE1, test2RMSE6);
end

fprintf('Total score: %.1f\n', score/10);

