function [ normals ] = SGPS_Hybrid_diffuse( X_d,Y_d,Z_d,X2_d,Y2_d,Z2_d )
%SGPS_HYBRID_DIFFUSE Spherical gradient photometric stereo, one normal map
%per colour channel
%   normals: Returns 3 different normal maps, one for each colour channel

for chan=1:3
    normals(:,:,:,chan)=SGPS_Grayscale_diffuse( X_d(:,:,chan),Y_d(:,:,chan),Z_d(:,:,chan),X2_d(:,:,chan),Y2_d(:,:,chan),Z2_d(:,:,chan) );
end

end

