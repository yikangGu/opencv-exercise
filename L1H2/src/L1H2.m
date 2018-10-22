img = imread('../folder/1.jpg');
gray = rgb2gray(img);
canny = edge(gray, 'canny');
dilate = imdilate(canny, strel('square', 10));
fill = imfill(dilate, [1 1], 4);

% find Contours
[B,L] = bwboundaries(~fill, 'noholes');
disp(length(B));

drawing = label2rgb(L, @white, [0,0,0]);
subplot(1,2,1);
imshow(drawing);

hold on
for i = 1:length(B)
   boundary = B{i};
   plot(boundary(:,2), boundary(:,1), 'r', 'LineWidth', 2)
end

% see : https://stackoverflow.com/questions/27192048/equivalent-threshold-function-of-opencv-in-matlab
% equals the inRange() function in python
minval = [0, 0, 100];
maxval = [40, 30, 255];
blue = true(size(img,1), size(img,2));
for p = 1 : 3
    blue = blue & (img(:,:,p) >= minval(p) & img(:,:,p) <= maxval(p));
end

% find contours
[B,blueImgLabel] = bwboundaries(blue, 'noholes');
disp(length(B));

% see : https://www.mathworks.com/help/images/ref/regionprops.html?s_tid=doc_ta
% find the center of shape
s = regionprops(blueImgLabel, 'Centroid');
centroids = cat(1, s.Centroid);

subplot(1,2,2);
imshow(img);
hold on
plot(centroids(1,1),centroids(1,2), 'w.');
plot(centroids(3,1),centroids(3,2), 'w.');
plot((centroids(1,1) + centroids(3,1)) / 2, (centroids(1,2) + centroids(3,2)) / 2, 'w.');

