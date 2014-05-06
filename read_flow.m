function [u, v ] = read_flow( u_file, v_file )
%READ_FLOW Summary of this function goes here
%   Detailed explanation goes here

fidu = fopen(u_file,'r');

header = textscan(fidu,'%d %d %f %f\n',1);
data = textscan(fidu,'%f\n');
data = data{1};

width = header{1};
height = header{2};

u = zeros(height,width);

for i = 1:width
    for j = 1:height
        index = width * (j - 1) + i;
        u(j,i) = data(index);
    end
end

fidv = fopen(v_file, 'r');
header = textscan(fidv,'%d %d %f %f\n',1);
data = textscan(fidv,'%f\n');
data = data{1};

v = zeros(height,width);

for i = 1:width
    for j = 1:height
        index = width * (j - 1) + i;
        v(j,i) = data(index);
    end
end


end

