function animate_imgs( img1, img2, scale, hsv, max_intensity, fps )
%ANIMATE Summary of this function goes here
%   Detailed explanation goes here

img1=imresize(img1,scale);
img2=imresize(img2,scale);

if(hsv)
    hsv1=rgb2hsv(img1);
    hsv2=rgb2hsv(img2);
    tmp=hsv1(:,:,1);
    I1(:,:,1)=tmp;
    I1(:,:,2)=tmp;
    I1(:,:,3)=tmp;
    tmp=hsv2(:,:,1);
    I2(:,:,1)=tmp;
    I2(:,:,2)=tmp;
    I2(:,:,3)=tmp;
else
    I1=double(img1)/max_intensity;
    I2=double(img2)/max_intensity;
end

m(:,1)=im2frame(I1);
m(:,2)=im2frame(I2);

h=gcf;
scrsz = get(0,'ScreenSize');
set(h,'OuterPosition',[scrsz(1),scrsz(2),size(I1,2), size(I1,1)]);

movie(h,m,20000,fps,[0 -size(I1,1)/2 0 0]);

end

