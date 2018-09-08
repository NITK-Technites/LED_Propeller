% Create an image of circles
radii = linspace(0, 40, 10);

rows = 100;
cols = 100;
img = zeros(rows, cols);

for k = 1:numel(radii)
    t = linspace(0, 2*pi, 1000);
    xx = round((cos(t) * radii(k)) + (cols / 2));
    yy = round((sin(t) * radii(k)) + (rows / 2));

    toremove = xx > cols | xx < 1 | yy > rows | yy < 1;

    inds = sub2ind(size(img), xx(~toremove), yy(~toremove));

    img(inds) = 1;
end
%img = imread('Triangle.png');
[r,c] = size(img);
center_row = r / 2;
center_col = c / 2;

[X,Y] = meshgrid((1:c) - center_col, (1:r) - center_row);

[theta, rho] = cart2pol(X, Y);

rhoRange = linspace(0, max(rho(:)), 1000);
thetaRange = linspace(-pi, pi, 1000);

[T, R] = meshgrid(thetaRange, rhoRange);

theta_rho_image = griddata(theta, rho, double(img), T, R);

figure
subplot(1,2,1);
imshow(img);
title('Original Image')

subplot(1,2,2);
imshow(theta_rho_image);
title('Polar Image')