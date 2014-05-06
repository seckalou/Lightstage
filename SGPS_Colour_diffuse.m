function [ normals ] = SGPS_Colour_diffuse( X_d,Y_d,Z_d,X2_d,Y2_d,Z2_d,C_d )
%SGPS_COLOUR_DIFFUSE Spherical gradient photometric stereo (colour,
%diffuse)
%   Computes a single normal map by taking a weighted average of the normal
%   maps from each colour channel. Weights are intensities with the goal of
%   improving signal to noise ratio

normals = zeros(size(X_d,1),size(X_d,2),3);
for chan=1:3
    normals = normals+repmat(C_d(:,:,chan),[1 1 3]).*SGPS_Grayscale_diffuse( X_d(:,:,chan),Y_d(:,:,chan),Z_d(:,:,chan),X2_d(:,:,chan),Y2_d(:,:,chan),Z2_d(:,:,chan) );
end

normals = EWnorm(normals);

end

