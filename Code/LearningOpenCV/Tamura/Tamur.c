#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include "matrix.h"

/* Coarseness */
Matrix nbr_avg(Matrix image, int factor);
Matrix nbr_diff(Matrix image, int factor);
double Coarseness(Matrix image, int max_scale, double weight);

/* Contrast */
double Contrast(Matrix image);

/* Directionality */
void Directionality(Matrix image, int histo_bins, int histo_thres, 
		    double peak_thres,
		    int *orientations, double *dir);
double *dir_histogram(Matrix image, int histo_bins, int threshold);
double *new_dir_histogram(Matrix image, int histo_bins);

void main(int argc, char *argv[])
{
  Matrix image;
  int orientations;
  double crs, con, dir;
  char *p,*q;
  FILE *fp;

  if(argc < 2) {
    printf("Usage:\n%s <image_file>\n", argv[0]);
    printf("To compute tamura features for a 128x128 character array\n");
    printf("(coarseness, contrast, directionality, #orientations)\n");
    exit(1);
  }

  image = MatrixCreate(128, 128);
  fp = fopen(argv[1], "r");
  if(!fp) { perror(""); exit(1); }
  MatrixReadImage(image, fp);
  fclose(fp);

  crs = Coarseness(image, 5, 1);
  con = Contrast(image);
  Directionality(image, 16, 12, 2, &orientations, &dir);
  
  free(image);

  for(p = q = argv[1];*q;q++) if(*q == '/') p=q+1;
  printf("%s %lf %lf %lf %d\n", p, crs, con, dir, orientations);
}

#define CHECK_EXTREMUM(a,b)            \
  if((diff[a] * diff[b] < 0) ||        \
     ((diff[a] * diff[b] == 0) &&      \
      (diff[a] + diff[b] != 0))) {     \
    extrema[n_extrema] = b;            \
    is_peak[n_extrema] = diff[b] < 0;  \
    n_extrema++;                       \
  }

int valid_peak(int *peak, double *histogram, double peak_thres)
{
  double max_valley, sharpness;

  max_valley = histogram[peak[0]];
  if(histogram[peak[2]] > max_valley) 
    max_valley = histogram[peak[2]];
  sharpness = histogram[peak[1]] / max_valley;
  max_valley = fabs(sharpness - peak_thres);
/*
  if(max_valley < 0.1) printf("close call\n");
*/
  return(sharpness > peak_thres);
}

double sd_histo(double *histogram, int histo_bins, int start, int stop)
{
  double *interval;
  double sum;
  int i, length, mp;

  length = stop - start;
  if(stop <= start) length += histo_bins;
  interval = Allocate(length, double);

  /* copy the sub-histogram into the interval */
  if(stop <= start) {
    for(i=start;i < histo_bins;i++) interval[i-start] = histogram[i];
    for(i=0;i<stop;i++) interval[histo_bins-start+i] = histogram[i];
  }
  else {
    for(i=start;i<stop;i++) interval[i-start] = histogram[i];
  }

  /* normalize to make a distribution */
  sum = 0.0;
  for(i=0;i<length;i++) sum += interval[i];
  for(i=0;i<length;i++) interval[i] /= sum;

  /* find mean */
  sum = 0.0;
  for(i=0;i<length;i++) {
    sum += (i+1)*interval[i];
  }
  mp = (int)(sum-0.5);

  /* compute variance */
  sum = 0.0;
  for(i=0;i<length;i++) {
    sum += (double)(i - mp) * (i - mp) * interval[i];
  }

  free(interval);
  return sqrt(sum);
}
  
void Directionality(Matrix image, int histo_bins, int histo_thres, 
		    double peak_thres,
		    int *orientations, double *dir)
{
  double *histogram, *diff, dist;
  int *extrema, n_extrema, *is_peak;
  int num_peaks, i, j, k;
  int **pv;

  /* compute direction histogram */
/*
  histogram = new_dir_histogram(image, histo_bins);
*/
  histogram = dir_histogram(image, histo_bins, histo_thres);

/*
  for(i=0;i<histo_bins;i++) printf("%lf\n", histogram[i]);
*/

  /* find extrema of histogram */
  extrema = Allocate(histo_bins,int);
  is_peak = Allocate(histo_bins,int);
  diff = Allocate(histo_bins,double);
  n_extrema = 0;

  /* compute difference vector */
  for(i=0;i < histo_bins-1;i++) 
    diff[i] = histogram[i+1] - histogram[i];
  diff[histo_bins-1] = histogram[0] - histogram[histo_bins-1];
  CHECK_EXTREMUM(histo_bins-1, 0);
  for(i=0;i < histo_bins-1;i++) {
    CHECK_EXTREMUM(i, i+1);
  }
  free(diff);
/*
  for(i=0;i<n_extrema;i++) printf("%d\n", extrema[i]);
*/

  /* extract salient peaks */
  pv = Allocate(n_extrema, int*);
  pv[0] = Allocate(3*n_extrema, int);
  for(i=1;i < n_extrema;i++) pv[i] = &pv[0][i * 3];
  
  j = 0;
  num_peaks = 0;
  for(i=0;i < n_extrema;i++) {
    if(is_peak[i]) {
      if(j == 0) {
	/* find valley to fill first slot */
        for(k=n_extrema-1;is_peak[k];k--);
	pv[num_peaks][0] = extrema[k];
	j++;
      }
      if(j == 1) {
	pv[num_peaks][1] = extrema[i];
	j++;
      }
    }
    else {
      if(j < 2) {
	pv[num_peaks][0] = extrema[i];
	j = 1;
      }
      else {
	pv[num_peaks][2] = extrema[i];
	if(valid_peak(pv[num_peaks], histogram, peak_thres)) {
	  num_peaks++;
	}
	pv[num_peaks][0] = extrema[i];
	j = 1;
      }
    }
  }
  if(j == 2) {
    for(k=0;is_peak[k];k++);
    pv[num_peaks][2] = extrema[k];
  }
  if((j < 2) || !valid_peak(pv[num_peaks], histogram, peak_thres)) 
    num_peaks--;
  num_peaks++;
/*
  for(i=0;i<num_peaks;i++) 
    printf("%d %d %d\n", pv[i][0], pv[i][1], pv[i][2]);
*/
  *orientations = num_peaks;

  /* compute 2nd moment about peaks */
  if(num_peaks == 0) {
    *dir = 1;
    return;
  }
  *dir = 0;
  for(i=0;i<num_peaks;i++) {
    *dir += sd_histo(histogram, histo_bins, pv[i][0], pv[i][2]);
  }
  *dir /= 5; /* normalize by maximum value */
}

double *dir_histogram(Matrix image, int histo_bins, int threshold)
{
  int y,x, bin;
  double delh, delv;
  double delG, theta;
  double sum;
  double *histo;
  double dont_care;

  histo = Allocate(histo_bins, double);
  for(y=0;y<histo_bins;y++) histo[y]=0;
  sum = 0;
  dont_care = 0;
  for(y=1;y<image->height-1;y++) {
    for(x=1;x<image->width-1;x++) {
      delh = (image->data[y-1][x+1] + 
	      image->data[y][x+1] + 
	      image->data[y+1][x+1]) -
	     (image->data[y-1][x-1] + 
	      image->data[y][x-1] + 
	      image->data[y+1][x-1]);
      delv = (image->data[y-1][x-1] + 
	      image->data[y-1][x] + 
	      image->data[y-1][x+1]) -
	     (image->data[y+1][x-1] + 
	      image->data[y+1][x] + 
	      image->data[y+1][x+1]);
      delG = (fabs(delh) + fabs(delv)) / 2;
      if(delG >= threshold) {
	theta = atan2(delv, delh);
	if(theta < 0) theta += M_PI;
	else if(theta >= M_PI) theta -= M_PI;
	bin = (int)(theta * histo_bins / M_PI + 0.5);
	if(bin == histo_bins) bin = 0;
	if(bin < 0 || bin >= histo_bins) {
	  printf("Bin error: %d\n", bin);
	  exit(1);
	}
	histo[bin]++;
	sum++;
      }
      else {
	dont_care++;
      }
    }
  }
  /* Equitz correction: add dont_care to every bin */
  sum += dont_care;
  dont_care /= histo_bins;
  if(sum) {
    for(y=0;y<histo_bins;y++) histo[y] = (histo[y] + dont_care) / sum;
  }
  return histo;
}

double *new_dir_histogram(Matrix image, int histo_bins)
{
  int y,x, bin;
  double delh, delv;
  double delG, theta;
  double sum;
  double *histo;

  histo = Allocate(histo_bins, double);
  for(y=0;y<histo_bins;y++) histo[y]=0;
  sum = 0;
  for(y=1;y<image->height-1;y++) {
    for(x=1;x<image->width-1;x++) {
      delh = (image->data[y-1][x+1] + 
	      image->data[y][x+1] + 
	      image->data[y+1][x+1]) -
	     (image->data[y-1][x-1] + 
	      image->data[y][x-1] + 
	      image->data[y+1][x-1]);
      delv = (image->data[y-1][x-1] + 
	      image->data[y-1][x] + 
	      image->data[y-1][x+1]) -
	     (image->data[y+1][x-1] + 
	      image->data[y+1][x] + 
	      image->data[y+1][x+1]);
      delG = (fabs(delh) + fabs(delv))/2;
      if(!delv && !delh) {
	theta = 0.0;
      }
      else {
	theta = atan2(delv, delh);
      }
      if(theta < 0) theta += M_PI;
      else if(theta >= M_PI) theta -= M_PI;
      bin = (int)(theta * histo_bins / M_PI + 0.5);
      if(bin == histo_bins) bin = 0;
      if(bin < 0 || bin >= histo_bins) {
	printf("Bin error: %d\n", bin);
	exit(1);
      }
      histo[bin] += delG;
      sum += delG;
    }
  }
  if(sum) {
    for(y=0;y<histo_bins;y++) histo[y] /= sum;
  }
  return histo;
}


