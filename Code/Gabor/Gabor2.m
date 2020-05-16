%4�������Gabor�˲�����lena�����˲�
clear;
I = imread('.\pic\lena.bmp');
f0 = 0.2; 
count = 0;
for theta = [0,pi/4,pi/2,pi*3/4];%�û���0,pi/4,pi/2,pi*3/4
    count = count + 1;
    x = 0;
    for i = linspace(-8,8,11)
        x = x + 1;
        y = 0;
        for j = linspace(-8,8,11)
            y = y + 1;
            z(y,x)=compute(i,j,f0,theta);
        end
    end
    figure(count);
    filtered = filter2(z,I,'valid');
    f = abs(filtered);
    imshow(f/max(f(:)))
end