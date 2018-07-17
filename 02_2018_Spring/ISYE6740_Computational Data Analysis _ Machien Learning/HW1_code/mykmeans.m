function [ class, centroid ] = mykmeans( pixels, K )
%
% Author: Soo Hyeon Kim
% Version: 1.25.2018
%
% Your goal is implementing your own K-means.
%
% Input:
%     pixels: data set. Each row contains one data point. For image
%     dataset, it contains 3 columns, each column corresponding to Red,
%     Green, and Blue component.
%
%     K: the number of desired clusters. Too high value of K may result in
%     empty cluster error. Then, you need to reduce it.
%
% Output:
%     class: the class assignment of each data point in pixels. The
%     assignment should be 1, 2, 3, etc. For K = 5, for example, each cell
%     of class should be either 1, 2, 3, 4, or 5. The output should be a
%     column vector with size(pixels, 1) elements.
%
%     centroid: the location of K centroids in your result. With images,
%     each centroid corresponds to the representative color of each
%     cluster. The output should be a matrix with K rows and
%     3 columns. The range of values should be [0, 255].
%     
%
% You may run the following line, then you can see what should be done.
% For submission, you need to code your own implementation without using
% the kmeans matlab function directly. That is, you need to comment it out.

    init = randperm(size(pixels, 1), K); %randomly initialize centroids 
    centroid = pixels(init, :); %initial centroids are samples from pixels
    new_centroid = zeros(size(centroid)); %to overwrite in loop
    
    dif = 2; %initialize to anything > 1
    Distance = 'euclidean'; %distance metric
    %SSE = zeros(0, 1);
    
    %all_centroids = centroid;
    
    while dif > .01 %iterate until centroids are constant
        
        %distance from each datapoint to each centroid:
        dist = pdist2(pixels, centroid, Distance); 
        %class assignments (closest centroid to each datapoint):
        [mins, class] = min(dist, [], 2);
        %SSE = [SSE; sum(mins.^2)]; %sum of squared errors
        
        for i = 1:K %for each cluster
            cluster = pixels(class == i, :); %all samples assigned to cluster
            if size(cluster, 1) < 1 %if no samples, leave centroid where it is
                new_centroid(i, :) = centroid(i, :);
                break
            end %if
            %new centroid is mean of each column
            new_centroid(i, :) = sum(cluster, 1) ./ size(cluster, 1);
        end %for
        
        dif =  sum(sum(abs(centroid - new_centroid)));
        %difference between current centroids and new centroids
        
        centroid = new_centroid;
        
        %all_centroids = cat(3, all_centroids, new_centroid);
        
    end %while 

	%[class, centroid] = kmeans(pixels, K);
end

