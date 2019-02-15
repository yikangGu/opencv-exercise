clear;close all;clc;%ready to processing
I = imread('1.jpg');%read and show the original image
figure;imshow(I);title('original image');
p = rgb2gray(I);%thange the original image rgb form into gray form
figure;imshow(p);title('gray image');%show the gray image

%we can search the pixle value by the matlab and find that the hidden value
%is 225, so we change the 225 into 0 the show the hidden image
[rows, cols] = size(p);
for x=1:cols
    for y=1:rows
        if I(y,x) == 225
            p(y,x) =0;
        end
    end
end
figure;imshow(p);title('map image');%show the hidden map
p= imcomplement(p);%change the image into opppsing 
figure;imshow(p);title('complement image')
p = medfilt2(p,[4,4]);%remove the noise from the image by medfilter
figure;imshow(p);title('after the medfilt process image')
bw=im2bw(p,graythresh(p));%change the image into bw form
figure;imshow(bw);title('change into bw image')
bw=bwmorph(bw,'erode');%process the image by erosion
figure;imshow(bw);title('after erode processing image')
dilateImg = imdilate(bw,strel('sphere',3));%use the strel processing the change the sphere form of the image
figure;imshow(dilateImg);title('after dilate processing image')
%bw=~bw;
%dilateImg=~dilateImg;
fill = imfill(dilateImg,'holes');%fill the holes of the image
figure;imshow(fill);title('fill processing image')
cannyImg = edge(fill,'Canny');%show the edge of the image
figure;imshow(cannyImg);title('cannyImg image')
cannyImg=bwmorph(cannyImg,'thin');%thin the edge
figure;imshow(cannyImg);title('thin cannyImg image')

%fill the image's background with white
[B1,L1] = bwboundaries(cannyImg,'holes');
final=label2rgb(~L1, @white, [0,0,0]);
figure;imshow(final);title('white background image')
 final1= rgb2gray(final);%change the final image into gray from the remove the noise again
 final2= medfilt2(final1,[4,4]);
 figure;imshow(final2);title('final image after the medfilt processing ')
 final3 = medfilt2(final2,[4,4]);%remove the noise from the final image by medfilter
figure;imshow(final3);title('after the medfilt process final image')
 
 %mark the black balls with white spots(i can't show the marked image and
 %don't konw why)
 %the mathod is find the cenntre of the circle and mark it
[L2,n]=bwlabel(~final3,4);
 for k=1:n
     [x,y]=find(L2==k);
     p(k,:)=vpa([mean(x),mean(y)],4);
     b(k,:)=round([mean(x),mean(y)]);
    final3(b(k,1),b(k,2),:)=255;
 end
 p;
 imtool(final3)

