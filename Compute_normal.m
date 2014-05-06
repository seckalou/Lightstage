function [N_diff_r N_diff_g N_diff_b] = Compute_normal(img_dir)
%COMPUTE_NORMAL Summary of this function goes here
%   Detailed explanation goes here

%-----------------Reading photometric images-----------------

disp('Reading photometric images....')

img_dir = strcat(img_dir, filesep);

X_d = double(imread(strcat(img_dir,'01_X_d.tiff')));
Y_d = double(imread(strcat(img_dir,'02_Y_d.tiff')));
Z_d = double(imread(strcat(img_dir,'03_Z_d.tiff')));

X2_d = double(imread(strcat(img_dir,'04_X2_d.tiff')));
Y2_d = double(imread(strcat(img_dir,'05_Y2_d.tiff')));
Z2_d = double(imread(strcat(img_dir,'06_Z2_d.tiff')));

C_d = double(imread(strcat(img_dir,'07_C_d.tiff')));

% C_s = double(imread(strcat(img_dir,'08_C_s.tiff')));
% 
% X_s = double(imread(strcat(img_dir,'09_X_s.tiff')));
% Y_s = double(imread(strcat(img_dir,'10_Y_s.tiff')));
% Z_s = double(imread(strcat(img_dir,'11_Z_s.tiff')));
% 
% X2_s = double(imread(strcat(img_dir,'12_X2_s.tiff')));
% Y2_s = double(imread(strcat(img_dir,'13_Y2_s.tiff')));
% Z2_s = double(imread(strcat(img_dir,'14_Z2_s.tiff')));


%--------------------Aligning images to Ref Cd-----------------
disp('Images alignment.....');

X_d = align_images(X_d,C_d); imwrite(X_d,strcat(img_dir,'01_X_d_aligned.tiff'));
Y_d = align_images(Y_d,C_d); imwrite(Y_d,strcat(img_dir,'02_Y_d_aligned.tiff'));
Z_d = align_images(Z_d,C_d); imwrite(Z_d,strcat(img_dir,'03_Z_d_aligned.tiff'));

X2_d = align_images(X2_d,C_d); imwrite(X2_d,strcat(img_dir,'04_X2_d_aligned.tiff'));
Y2_d = align_images(Y2_d,C_d); imwrite(Y2_d,strcat(img_dir,'05_Y2_d_aligned.tiff'));
Z2_d = align_images(Z2_d,C_d); imwrite(Z2_d,strcat(img_dir,'06_Z2_d_aligned.tiff'));


disp('=========================Normal Calculation===================================')
N_diff_r = SGPS_Grayscale_diffuse(X_d(:,:,1),Y_d(:,:,1),Z_d(:,:,1),X2_d(:,:,1),Y2_d(:,:,1),Z2_d(:,:,1));
N_diff_g = SGPS_Grayscale_diffuse(X_d(:,:,2),Y_d(:,:,2),Z_d(:,:,2),X2_d(:,:,2),Y2_d(:,:,2),Z2_d(:,:,2));
N_diff_b = SGPS_Grayscale_diffuse(X_d(:,:,3),Y_d(:,:,3),Z_d(:,:,3),X2_d(:,:,3),Y2_d(:,:,3),Z2_d(:,:,3));

end

