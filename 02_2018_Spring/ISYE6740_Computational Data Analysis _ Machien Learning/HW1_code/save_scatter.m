K = 8; %number of clusters
%un-comment lines on mykmeans dealing with "all_centroids" and set this as secondary output, then run:
[class, all_centroids] = mykmeans(pixels, K); 
length = size(all_centroids, 3);
%create 3D scatter plot of all datapoints in pixels:
scatter3(pixels(:, 1), pixels(:, 2), pixels(:, 3), 3, 'filled')
hold on

for i = 1:K %for each cluster, add to 3D plot:
	%line showing the path of each cluster centroid as algorithm converges (in black):
    plot3(reshape(all_centroids(i, 1, :), [length,1]), reshape(all_centroids(i, 2, :), [length, 1]), reshape(all_centroids(i, 3, :), [length,1]), 'k', 'LineWidth', 3)
	%add a marker for the location of each centroid on each iteration (in black):
    scatter3(reshape(all_centroids(i, 1, :), [length,1]), reshape(all_centroids(i, 2, :), [length, 1]), reshape(all_centroids(i, 3, :), [length, 1]), 40, 'ok', 'filled')
	%final cluster centroids after clustering (in red):
    scatter3(all_centroids(i, 1, length), all_centroids(i, 2, length), all_centroids(i, 3, length), 120, 'or', 'filled')
    
end %for

%tried to define new colors but didn't work
or = [.85, .33, .10];
pl = [.33, .11, .55];

colors = ['b', 'r', 'g', 'y', 'c', 'm', pl, or];

%new figure
figure
hold on
for i = 1:K %for each cluster, plot in 3D:
	%scatter plot of all samples in that cluster (each in a different color)
    scatter3(pixels(class == i, 1), pixels(class == i, 2), pixels(class == i, 3), 5, colors(i), 'filled')
	%show final centroids of each cluster in black:
    scatter3(all_centroids(i, 1, length), all_centroids(i, 2, length), all_centroids(i, 3, length), 120, 'ok', 'filled')
end %for