function [R, U, V] = align_images( img1, img2 )
%ALIGN_IMAGES Align img1 to img2 using optical flow and put the result in R

addpath(genpath('black_Anandan'));

s = size(img1);

H1=rgb2hsv(double(img1));
H2=rgb2hsv(double(img2));

imwrite(H1(:,:,1), 'data/yos11.pgm');
imwrite(H2(:,:,1), 'data/yos12.pgm');

% H1=EWnorm(double(img1));
% H2=EWnorm(double(img2));
% imwrite(rgb2gray(H1), 'data/yos11.pgm');
% imwrite(rgb2gray(H2), 'data/yos12.pgm');

cmd_fmt = './gnc 11 12 4 1 data/yos results/flow- -l1 10.0 -l2 1.0 -S1 10.0 -S2 1.0 -s1 10.0 -s2 0.05 -stages 5 -nx %d -ny %d -f 0.5 -F 0 -w 1.995 -iters 60 -end ".pgm" -skip 15'
cmd = sprintf(cmd_fmt, s(2), s(1));

system(cmd);

[U,V] = read_flow('results/flow-u-ascii.txt','results/flow-v-ascii.txt');

clear('H1','H2');

for i = 1:s(3)
    R(:,:,i)= uint16(imwarp(double(img1(:,:,i)),-U,-V,true));
end


end

