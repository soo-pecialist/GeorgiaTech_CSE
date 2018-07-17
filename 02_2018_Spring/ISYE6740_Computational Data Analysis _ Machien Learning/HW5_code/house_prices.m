
close all;
clear;
clc;

%% I) DATA
%% Ia: load
T = readtable('RealEstate.csv');

%% Ib: explore
T.Location = categorical(T.Location); % make categorical
T.Status = categorical(T.Status);

vars = T.Properties.VariableNames;
display(vars);

format compact
summary(T) % summary stats

%% Ic: prepare for regression

cols = {'Price', 'Bedrooms', 'Bathrooms', 'Size', 'Price_SQ_Ft', 'Location'};

xcolsl = {'Bedrooms', 'Bathrooms', 'Size', 'Location'};
xcols = {'Bedrooms', 'Bathrooms', 'Size'}; % exclude location

statuses = categories(T.Status);

%% II) MODELS
%% II0: fitlm: linear regression models using fitlm

T_k = T(T.Status == 'Short Sale', cols); % keep only Short Sale

% basic model, baseline for ridge and lasso below
fitlm(T_k, 'ResponseVar', 'Price', 'PredictorVars', xcols)

% include Location variable, not used in ridge & lasso 
fitlm(T_k, 'ResponseVar', 'Price', 'PredictorVars', xcolsl, ...
      'CategoricalVars', {'Location'})

%% IIx: set up
lambdas = logspace(-4, 4, 9);
L = length(lambdas);

train_ratio= 0.8;

%% IIa: ridge and IIb: lasso

fprintf('\n\n')

% for each status, estimate best lambda

for k = 1:length(statuses)
    sale_type = statuses(k);
    fprintf('type of sale: %s\n', sale_type{1});
    
    y_k = T(T.Status == sale_type, 'Price');
    X_k = T(T.Status == sale_type, xcols);
    
    y_k = table2array(y_k);
    X_k = table2array(X_k);
    
    N = length(X_k);
    fprintf('number of data points: %d\n', N);
    
    % split into training and validation datasets for cross-val
    [train_idx, valid_idx] = train_test_split(N, train_ratio);
    X_train = X_k(train_idx, :);
    X_valid = X_k(valid_idx, :);
    y_train = y_k(train_idx);
    y_valid = y_k(valid_idx);
    fprintf('size of train set: %d\n', length(y_train));
    fprintf('size of valid set: %d\n', length(y_valid));
    
    % ridge models
    bs_r = ridge(y_train, X_train, lambdas, 0);
    % lasso models
    [bs_l, fitinfo_l] = lasso(X_train, y_train, 'Lambda', lambdas);
    % fitinfo needed for intercept value
    
    rmses_r_train = zeros(L, 1); % root mean square errors
    rmses_r_valid = zeros(L, 1);
    rmses_l_train = zeros(L, 1);
    rmses_l_valid = zeros(L, 1);
    
    rmse_r_min = 1e8;
    rmse_l_min = 1e8;
    
    lambda_r_best = 0;
    lambda_l_best = 0;
    
    % predict y and calculate score (RMSE)
    fprintf('RMSEs:\n')
    for i = 1:L
        rmses_r_train(i) = score_ridge(X_train, y_train, bs_r(:, i));
        rmses_r_valid(i) = score_ridge(X_valid, y_valid, bs_r(:, i));
        
        rmses_l_train(i) = score_lasso(X_train, y_train, bs_l(:, i), ...
                                       fitinfo_l.Intercept(i));
        rmses_l_valid(i) = score_lasso(X_valid, y_valid, bs_l(:, i), ...
                                       fitinfo_l.Intercept(i));
        
        fprintf('%.0e: %.2e, %.4e; %.2e, %.4e\n', ...
                lambdas(i), ...
                rmses_r_train(i), rmses_r_valid(i), ...
                rmses_l_train(i), rmses_l_valid(i) ...
                );
            
        if rmses_r_valid(i) <= rmse_r_min
            rmse_r_min = rmses_r_valid(i);
            lambda_r_best = lambdas(i);
        end 
        if rmses_l_valid(i) <= rmse_l_min
            rmse_l_min = rmses_l_valid(i);
            lambda_l_best = lambdas(i);
        end
        
    end
    
    figure;
    semilogx(lambdas, rmses_r_train, '-s', 'LineWidth', 2); hold on;
    semilogx(lambdas, rmses_r_valid, '-s', 'LineWidth', 2);
    semilogx(lambdas, rmses_l_train, ':*', 'LineWidth', 2);
    semilogx(lambdas, rmses_l_valid, ':*', 'LineWidth', 2);
    grid on;
    xlabel('{\bf \lambda}')
    ylabel('{\bf mean square error}')
    legend('ridge - train', 'ridge - valid', ...
           'lasso - train', 'lasso - valid');
    legend('Location','northwest');
    legend('boxoff');
    hold off;
    title(sale_type{1});
    saveas(gcf, fullfile('./figures', sale_type{1}), 'png');
    
    fprintf('best ridge lambda = %.0e\n', lambda_r_best);
    fprintf('best lasso lambda = %.0e\n', lambda_l_best);
    
    fprintf('\n');
end

%% eof




