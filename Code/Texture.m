
function T = Texture(Image)
% Image = imread('E:\1\3.tiff');
% [M,N,O] = size(Image);
M = 256; 
N = 256;


for i = 1:M
    for j = 1:N
        for n = 1:256/16
            if (n-1)*16<=Gray(i,j)&Gray(i,j)<=(n-1)*16+15
                Gray(i,j) = n-1;
            end
        end
    end
end


P = zeros(16,16,4);
for m = 1:16
    for n = 1:16
        for i = 1:M
            for j = 1:N
                if j<N&Gray(i,j)==m-1&Gray(i,j+1)==n-1
                    P(m,n,1) = P(m,n,1)+1;
                    P(n,m,1) = P(m,n,1);
                end
                if i>1&j<N&Gray(i,j)==m-1&Gray(i-1,j+1)==n-1
                    P(m,n,2) = P(m,n,2)+1;
                    P(n,m,2) = P(m,n,2);
                end
                if i<M&Gray(i,j)==m-1&Gray(i+1,j)==n-1
                    P(m,n,3) = P(m,n,3)+1;
                    P(n,m,3) = P(m,n,3);
                end
                if i<M&j<N&Gray(i,j)==m-1&Gray(i+1,j+1)==n-1
                    P(m,n,4) = P(m,n,4)+1;
                    P(n,m,4) = P(m,n,4);
                end
            end
        end
        if m==n
            P(m,n,:) = P(m,n,:)*2;
        end
    end
end


for n = 1:4
    P(:,:,n) = P(:,:,n)/sum(sum(P(:,:,n)));
end


H = zeros(1,4);
I = H;
Ux = H;      Uy = H;
deltaX= H;  deltaY = H;
C =H;
for n = 1:4
    E(n) = sum(sum(P(:,:,n).^2)); 
    for i = 1:16
        for j = 1:16
            if P(i,j,n)~=0
                H(n) = -P(i,j,n)*log(P(i,j,n))+H(n); 
            end
            I(n) = (i-j)^2*P(i,j,n)+I(n); 
           
            Ux(n) = i*P(i,j,n)+Ux(n); 
            Uy(n) = j*P(i,j,n)+Uy(n); 
        end
    end
end
for n = 1:4
    for i = 1:16
        for j = 1:16
            deltaX(n) = (i-Ux(n))^2*P(i,j,n)+deltaX(n); 
            deltaY(n) = (j-Uy(n))^2*P(i,j,n)+deltaY(n); 
            C(n) = i*j*P(i,j,n)+C(n);             
        end
    end
    C(n) = (C(n)-Ux(n)*Uy(n))/deltaX(n)/deltaY(n);   
end


T(1) = mean(E);   T(2) = sqrt(cov(E));
T(3) = mean(H);   T(4) = sqrt(cov(H));
T(5) = mean(I);   T(6) = sqrt(cov(I));
T(7) = mean(C);   T(8) = sqrt(cov(C));

