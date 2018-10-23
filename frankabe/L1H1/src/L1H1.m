i=imread('1.jpg');%read the picture in the exercise1.
binaryImg=rgb2gray(i);%{ change the rbg form figure1 to the gray form
                      % and then change it into binary form,which is used  
                      % to get the binary value of the figure1.%}
[row,clos]=size(binaryImg);%{set the row and equal to the line and rank 
                           %value of the matrix of the figure1.%}
for x=1:row       %{traverse the matrix and change  the value 2 into 0 
 for   y=1:clos   %and others into 255,which can change the background of  
    if i(x,y)==2  %figure1 into black and the effecive part into white.%}
        binaryImg(x,y)=255;%{ps:we can know the background value by the matlab%}
    else                   
        binaryImg(x,y)=0;
    end
end
end

dilateImg=imdilate(binaryImg,strel('square',50));%{use the dilate function to separate each objects of the figure1%}
cannyImg=edge(dilateImg,'canny');%{use the canny edge processing to reduce the noise of the figure%}
[B,L]=bwboundaries(cannyImg,'noholes');%{use the bwboundaries function to find the edges except the holes, which can find how many objects in the figure.                            
imshow(label2rgb(L,@jet,[.5 .5 .5]))%change the parts except the holes into gray, which can find the objects hidden in the figure.
hold on
for k=1:length(B)
    boundary = B{k};
    plot(boundary(:,2), boundary(:,1), 'w', 'LineWidth', 2)%change the color of edges into white to change the objects'color hidden in the figure.
end
disp(k);%get the quantity of the objects by get the quantity of the edges.

        
        
                           