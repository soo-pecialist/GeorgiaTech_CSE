
% This is the main routine for homework 4.

close all;
clear;
clc;

%% Visualize some random samples

load('usps-2cls.mat');
[N, d] = size(mydata);

r = round(rand(1, 16) * N);

figure; colormap(gray);
for t = 1:16
    x = mydata(r(t), 1:d-1)';
    subplot(4, 4, t); imagesc(reshape(x, 16, 16)); title(r(t));
end

%% Let p change from 0.1, 0.2, 0.5, 0.8, 0.9 to compare the performance of 
% each classifier

ps = [0.1, 0.2, 0.5, 0.8, 0.9];
P = length(ps);

%% Repeat the experiments T times for each p

% T = 100;
T = 10;

%% Save errors for each p and each iteration

err_Full = zeros(P, T, 2);
err_Diagonal = zeros(P, T, 2);
err_Spherical = zeros(P, T, 2);

k = [5, 10, 15, 30];
err_knn_m = zeros(P, T, 2, length(k)); % inbuilt function
err_knn = zeros(P, T, 2, length(k));  %handcoded function

err_logistic = zeros(P, T, 2);


%% Loops

for i = 1:P
    
    for t = 1 : T

        % Get random train test split
        [train, test] = SplitData(ps(i));

        % Create X, y
        [Xtrain, ytrain] = getXy(train);
        [Xtest, ytest] = getXy(test);

        % Bayes
        [err_train, err_test] = ModelFull(Xtrain, ytrain, Xtest, ytest);
        err_Full(i, t, :) = [err_train, err_test];
        [err_train, err_test] = ModelDiagonal(Xtrain, ytrain, Xtest, ytest);
        err_Diagonal(i, t, :) = [err_train, err_test];
        [err_train, err_test] = ModelSpherical(Xtrain, ytrain, Xtest, ytest);
        err_Spherical(i, t, :) = [err_train, err_test];

        % kNN - inbuilt (m for matlab)
        for j = 1:length(k)
            [err_train, err_test] = ModelKNNm(Xtrain, ytrain, Xtest, ytest, k(j));
            err_knn_m(i, t, :, j) = [err_train, err_test];
        end

        % kNN - handcoded
        for j = 1:length(k)
            [err_train, err_test] = ModelKNN(Xtrain, ytrain, Xtest, ytest, k(j));
            err_knn(i, t, :, j) = [err_train, err_test];
        end
        
        % Logistic Regression
        [err_train, err_test] = ModelLR(Xtrain, ytrain, Xtest, ytest);
        err_logistic(i, t, :) = [err_train, err_test];

    end % T loop
    
end % p loop

%% Results

clc;

for i = 1:P
    
    fprintf('p = %.1f\n\n', ps(i));
    
    mean_err_Full = mean(err_Full(i, :, :));
    mean_err_Diagonal = mean(err_Diagonal(i, :, :));
    mean_err_Spherical = mean(err_Spherical(i, :, :));

    mean_err_knn_m = mean(err_knn_m(i, :, :, :));
    mean_err_knn_m = squeeze(mean_err_knn_m);

    mean_err_knn = mean(err_knn(i, :, :, :));
    mean_err_knn = squeeze(mean_err_knn);

    mean_err_logistic = mean(err_logistic(i, :, :));

    fprintf('err_full      : %.4f, %.4f\n', mean_err_Full(1), mean_err_Full(2));
    fprintf('err_diagonal  : %.4f, %.4f\n', mean_err_Diagonal(1), mean_err_Diagonal(2));
    fprintf('err_spherical : %.4f, %.4f\n', mean_err_Spherical(1), mean_err_Spherical(2));
    fprintf('\n');

    for j = 1:length(k)
        fprintf('err_knn_m_%02d  : %.4f, %.4f\n', k(j), mean_err_knn_m(1, j), mean_err_knn_m(2, j));
    end
    fprintf('\n');

    for j = 1:length(k)
        fprintf('err_knn_%02d    : %.4f, %.4f\n', k(j), mean_err_knn(1, j), mean_err_knn(2, j));
    end
    
    fprintf('\n');    fprintf('err_logistic  : %.4f, %.4f\n', mean_err_logistic(1), mean_err_logistic(2));
    fprintf('\n');
    
    fprintf('\n');
end

%% Prepare mean error rates for plotting

err_f = squeeze(mean(err_Full, 2));
err_d = squeeze(mean(err_Diagonal, 2));
err_s = squeeze(mean(err_Spherical, 2));

err_m = squeeze(mean(err_knn_m, 2));
err_k = squeeze(mean(err_knn, 2));

err_l = squeeze(mean(err_logistic, 2));

%% Plot 1: Bayes

figure;

hold on;

plot(ps, err_f(:, 1), '-s', ps, err_f(:, 2), ':*', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 
plot(ps, err_d(:, 1), '-s', ps, err_d(:, 2), ':*', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 
plot(ps, err_s(:, 1), '-s', ps, err_s(:, 2), ':*', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 

xlim([0 1]); xlabel('training ratio (p)');
ylabel('error rate');

legend('full - train', 'full - test', ...
       'diagonal - train', 'diagonal - test', ...
       'spherical - train', 'spherical - test', ...
       'Location', 'north', 'Orientation', 'horizontal');
legend('boxoff'); 

hold off;

%% Plot 2: KNN

figure;

hold on;

plot(ps, err_k(:, 1, 1), '-s', ps, err_k(:, 2, 1), ':x', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 
plot(ps, err_k(:, 1, 2), '-s', ps, err_k(:, 2, 2), ':x', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 
plot(ps, err_k(:, 1, 3), '-s', ps, err_k(:, 2, 3), ':x', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 
plot(ps, err_k(:, 1, 4), '-s', ps, err_k(:, 2, 4), ':x', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 

xlim([0 1]); xlabel('training ratio (p)');
ylabel('error rate');

legend('k =  5 - train', 'k =  5 - test', ...
       'k = 10 - train', 'k = 10 - test', ...
       'k = 15 - train', 'k = 15 - test', ...
       'k = 30 - train', 'k = 30 - test', ...
       'Location', 'north', 'Orientation', 'horizontal');
legend('boxoff');   
   
hold off;

%% Plot 3 - Bayes (full) vs KNN (k=5) vs Logistic Regression

figure;

hold on;

plot(ps, err_f(:, 1), '-s', ps, err_f(:, 2), ':*', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 
plot(ps, err_k(:, 1, 1), '-s', ps, err_k(:, 2, 1), ':*', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 
plot(ps, err_l(:, 1), '-s', ps, err_l(:, 2), ':*', 'LineWidth', 2, 'MarkerFaceColor', 'auto'); 

xlim([0 1]); xlabel('training ratio (p)');
ylabel('error rate');

legend('full - train', 'full - test', ...
       'k = 5 - train', 'k = 5 - test', ...
       'logistic - train', 'logistic - test', ...
       'Location', 'north', 'Orientation', 'horizontal');
legend('boxoff'); 

hold off;

%% EOF


