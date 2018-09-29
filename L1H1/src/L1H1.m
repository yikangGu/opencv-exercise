img = imread('../folder/fig1.jpg');
% or use imbinarize func
binaryImg = rgb2gray(img);
[rows, cols] = size(binaryImg);

for x=1:cols
    for y=1:rows
        if img(y,x) == 2
            binaryImg(y,x) = 255;
        else
            binaryImg(y,x) = 0;
        end
    end
end

dilateImg = imdilate(binaryImg,strel('square',40));
cannyImg = edge(dilateImg,'Canny');

[B,L] = bwboundaries(cannyImg,'noholes');
rgbImg = ind2rgb(cannyImg, dilateImg);

imshow(label2rgb(~L, @white, [0,0,0]))
hold on
for k = 1:length(B)
   boundary = B{k};
   plot(boundary(:,2), boundary(:,1), 'r', 'LineWidth', 2)
end

disp(length(B))