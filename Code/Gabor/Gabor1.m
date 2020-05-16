%4个方向的Gabo滤波器通过图像显示
clear;
x = 0;
theta = pi*3/4;%用弧度0,pi/4,pi/2,pi*3/4
f0 = 0.2; 
for i = linspace(-15,15,50)
    x = x + 1;
    y = 0;
    for j = linspace(-15,15,50)
        y = y + 1;
        z(y,x)=compute(i,j,f0,theta);
    end
end
z_real = real(z);
m = min(z_real(:));
z_real = z_real+abs(m);
M = max(z_real(:));
imshow(1/M*z_real);
figure(2)
z_imag = imag(z);
m = min(z_imag(:));
z_imag = z_imag+abs(m);
M = max(z_imag(:));
imshow(1/M*z_imag);

