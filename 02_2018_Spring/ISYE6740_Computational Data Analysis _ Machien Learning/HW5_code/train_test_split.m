function [train_idx, test_idx] = train_test_split(N, train_ratio)
    idx = randperm(N);
    split = round(train_ratio * N);
    train_idx = idx(1:split);
    test_idx = idx(split+1:N);
end