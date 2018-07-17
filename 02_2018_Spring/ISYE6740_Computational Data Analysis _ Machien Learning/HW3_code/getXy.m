
function [X, y] = getXy(Xy)

    % split dataset into X and y
    % convert datatypes, divide X by 255

    [~, d] = size(Xy);

    X = double(Xy(:, 1:d-1));
    y = double(Xy(:, d));
    
    X = X ./ 255;

end