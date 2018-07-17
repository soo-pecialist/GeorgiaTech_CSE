
close all;
clear;
clc;

iptsetpref('ImshowBorder', 'tight'); % reduces margin while saving images

%% load data

load('cs.mat');
img = double(img);

%% visualize image

figure; 
imagesc(img); 
colormap gray;
pbaspect([1 1 1]); % force square aspect ratio as image is 50x50

saveas(gcf, '.\figures\sparse.png')

%% generate noisy measurements of image using random weights + noise

A = randn(1300, 2500);
y = A * img(:) + 5 * randn(1300, 1);

%% plot noisy weights (one example)

figure
imagesc(reshape(A(42, :), 50, 50));
colormap gray
pbaspect([1 1 1])

saveas(gcf, '.\figures\noisyw.png')

%% ridge regression

lambda = 0.5; % regularization parameter
b_r = ridge(y, A, lambda); 

b_r = reshape(b_r, 50, 50);
rmse_r = sqrt(immse(b_r, img));

figure; 
imagesc(reshape(b_r, 50, 50));
colormap gray; % image recovered by ridge regression

%% lasso regression

lambda = 0.5;
b_l = lasso(A, y, 'Lambda', lambda);

b_l = reshape(b_l, 50, 50);
rmse_l = sqrt(immse(b_l, img));

figure; 
imagesc(b_l);
colormap gray; % image recovered by lasso regression

%% grid search: set up

lambdas = [0.01, 0.1, 0.5, 1.0, 10., 100.]; % values of lambda to try
L = length(lambdas);

mses_r = zeros(L, 1); % mean square errors
mses_l = zeros(L, 1);

mse_r_min = 999;
mse_l_min = 999;

lambda_r_best = 0;
lambda_l_best = 0;

%% grid search: ridge

bs_r = ridge(y, A, lambdas); % estimates coeffs for all lambdas at once
bs_r = reshape(bs_r, 50, 50, L);

fprintf('\n');
for i = 1:L
    lambda = lambdas(i);
    fprintf('evaluating lambda = %.2f\n', lambda);
    mses_r(i) = immse(bs_r(:, :, i), img); % mean square reconst. error
    fprintf('mse ridge = %.4f\n', mses_r(i));
    if mses_r(i) < mse_r_min
        mse_r_min = mses_r(i);
        lambda_r_best = lambda;
    end
    fprintf('\n');
end

fprintf('best lambda for ridge = %.2f\n', lambda_r_best);

%% grid search: lasso

bs_l = lasso(A, y, 'Lambda', lambdas); % estimates coeffs for all lambdas
bs_l = reshape(bs_l, 50, 50, L);

fprintf('\n');
for i = 1:L
    lambda = lambdas(i);
    fprintf('evaluating lambda = %.2f\n', lambda);
    mses_l(i) = immse(bs_l(:, :, i), img);
    fprintf('rmse lasso = %.4f\n', mses_l(i));
    if mses_l(i) < mse_l_min
        mse_l_min = mses_l(i);
        lambda_l_best = lambda;
    end
    fprintf('\n');
end

fprintf('best lambda for lasso = %.2f\n', lambda_l_best);

%% ridge: plot MSEs as a function of lambda

figure
semilogx(lambdas, mses_r, '-s', 'LineWidth', 2) % use log-scale for x-axis
ylim([0.09, 0.096])
ax = gca;
grid on
xlabel('{\bf Lambda}')
ylabel('{\bf Mean Square Error}')

%% lasso: plot MSEs as a function of lambda
 
figure
semilogx(lambdas, mses_l, ':*', 'LineWidth', 2)
ax = gca;
grid on
xlabel('{\bf Lambda}')
ylabel('{\bf Mean Square Error}')

%% plot: both ridge and lasso MSEs
 
figure
semilogx(lambdas, mses_r, '-s', 'LineWidth', 2)
hold
semilogx(lambdas, mses_l, ':*', 'LineWidth', 2)
ax = gca;
grid on
xlabel('{\bf \lambda}')
ylabel('{\bf reconstruction error}')
legend('ridge', 'lasso');
legend('boxoff');

saveas(gcf, '.\figures\lambda.png')

%% ridge: visualize best model

r_best_idx = find(lambdas == lambda_r_best);
b_r_best = bs_r(:, :, r_best_idx);

figure; 
imagesc(b_r_best);
colormap gray;
pbaspect([1 1 1])

saveas(gcf, '.\figures\ridge.png')

%% lasso: visualize best model

l_best_idx = find(lambdas == lambda_l_best);
b_l_best = lasso(A, y, 'Lambda', lambdas(l_best_idx));
b_l_best = reshape(b_l_best, 50, 50);

figure; 
imagesc(b_l_best);
colormap gray;
pbaspect([1 1 1])

saveas(gcf, '.\figures\lasso.png')

%% eof
