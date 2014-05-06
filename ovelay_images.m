function ovelay_images( img1, img2 )
%OVELAY_IMAGES Summary of this function goes here
%   Detailed explanation goes here
imshow(img1);
hold on
h = imshow(img2);
set(h, 'AlphaData', 0.5);
hold off

end

