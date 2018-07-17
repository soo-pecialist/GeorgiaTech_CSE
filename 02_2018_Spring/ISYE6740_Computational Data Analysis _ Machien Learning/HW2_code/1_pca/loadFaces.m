
function [X, poses] = loadFaces(path, subject)

    % arguments: 
    %   - path: path to images 
    %   - subject: subject id (e.g. 01)
    
    % returns:     
    %   - X: images in form 'nrows x ncols x number' of images
    %   - poses: array of strings containing the poses: 'happy' etc 

    list_images = dir([path, 'subject', subject , '*']); % e.g. 'yalefaces/subject01*'
    %list_images(1)
    
    % size of images downsampled by a factor of 4
    if strcmp(path, 'yalefaces/')
        nrows = ceil(243/4);
        ncols = ceil(320/4);
    else % yalecentered
        nrows = ceil(231/4);
        ncols = ceil(195/4);
    end %if-else

    N = length(list_images); % 11
    X = zeros(nrows, ncols, N); % image holder matrix
    poses = cell(N, 1); % 11x1 cell array

    for i = 1:N % 1 to 11
        filename = list_images(i).name; %e.g., 'subject01.centered.gif'
        % extract pose from filename and saves in poses
        fnsplit = strsplit(filename, '.'); % 1×3 cell array 
            % e.g. fnsplit - {'subject01'} {'centered'} {'gif'}
        poses(i) = fnsplit(2); % e.g., centered, glasses, happy, ... 
    
%         if strcmp(char(poses(i)), 'test')
%             fprintf('test image, %d\n', i)
%         end
        
        % load image, downsample, store in X
        x = imread([path, filename]);
        x = imresize(x, [nrows, ncols]);
        X(:, :, i) = x; 
    end
    
end
