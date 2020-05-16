%%%
%%% Nuno Gonçalves, April 2013
%%%

% Algorithm to compute the Chi-Squared score in two classes

% Step 1 - find the mean value for the class 1 (ugly pages) and the mean
% value for the class 2 (nice pages).
% Step 2 - Compute the mean value of both classes mean values - val1
% Step 3 - Compute the expected and observed frequencies:
%    - Expected frequency is 15 in each class since we are using equal
%    sized group classes - E11=15 and E22=15
%    - Observed frequency is the number of items of class 1 (ugly pages) in
%    the bin 1 (whose value is below or equal than val1) - O11, and the
%    number of items of class 2 (nice pages) in bin 2 (whose value is
%    greater than val1)
% Step 4 - Compute the chi-squared value for this feature
%
% Step 5 - Sort the chi-squared value for all features and choose the M
% features with the smallest value.


% Good feature because the two clusters are not overlaped by each other
feature1=[random('unif',1,3,15,1);random('unif',5,7,15,1)];

% Good feature because the two clusters are not overlaped by each other
feature2=[random('unif',1,5,15,1);random('unif',5,8,15,1)];

% Bad feature because the two clusters are partially overlaped by each other
feature3=[random('unif',1,5,15,1);random('unif',3,7,15,1)];

% Bad feature because the two clusters are totally overlaped by each other
feature4=[random('unif',1,5,15,1);random('unif',1,5,15,1)];

%Step 1
mean1=[mean(feature1(1:15));mean(feature1(16:30))];
mean2=[mean(feature2(1:15));mean(feature2(16:30))];
mean3=[mean(feature3(1:15));mean(feature3(16:30))];
mean4=[mean(feature4(1:15));mean(feature4(16:30))];

%Step 2
val1=(mean1(1)+mean1(2))/2;
val2=(mean2(1)+mean2(2))/2;
val3=(mean3(1)+mean3(2))/2;
val4=(mean4(1)+mean4(2))/2;

%Feature 1
%Step 3
O11=numel(find(feature1(1:15)<=val1));
E11=15;
O22=numel(find(feature1(16:30)>val1));
E22=15;

%Step 4
chisq1=(O11-E11)^2/E11+(O22-E22)^2/E22;

%Feature 2
%Step 3
O11=numel(find(feature2(1:15)<=val2));
E11=15;
O22=numel(find(feature2(16:30)>val2));
E22=15;

%Step 4
chisq2=(O11-E11)^2/E11+(O22-E22)^2/E22;

%Feature 3
%Step 3
O11=numel(find(feature3(1:15)<=val3));
E11=15;
O22=numel(find(feature3(16:30)>val3));
E22=15;

%Step 4
chisq3=(O11-E11)^2/E11+(O22-E22)^2/E22;

%Feature 4
%Step 3
O11=numel(find(feature4(1:15)<=val4));
E11=15;
O22=numel(find(feature4(16:30)>val4));
E22=15;

%Step 4
chisq4=(O11-E11)^2/E11+(O22-E22)^2/E22;


res=[chisq1;chisq2;chisq3;chisq4];

%We can see that features 1 and 2 are good features and feature 3 and 4 are
%bad
