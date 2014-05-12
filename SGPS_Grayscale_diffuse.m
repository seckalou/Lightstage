function [ normals ] = SGPS_Grayscale_diffuse( X_d,Y_d,Z_d,X2_d,Y2_d,Z2_d )
%SGPS_GRAYSCALE Spherical Gradient Photometric Stereo (Grayscale, diffuse)


normals(:,:,1)=X_d-X2_d;
normals(:,:,2)=Y_d-Y2_d;
normals(:,:,3)=Z_d-Z2_d;

normals = EWnorm(normals);

end

