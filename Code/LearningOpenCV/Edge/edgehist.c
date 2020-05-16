/*
	File:   edgehist.c
 	Author: António Videira
	Created on 25 February, 2013
	Function that extract the Edge Histogram from an image.
	Compute 4 directional edges and 1 non-directional edge
	The image is split into 4x4 non-overlapping rectangular regions.
	In each region, a 1x5 edge orientation histogram is computed (horizontal, 
	vertical, 2 diagonals and 1 non-directional).
	
*/

#define	NoEdge				0
#define	vertical_edge			1
#define	horizontal_edge			2
#define	non_directional_edge		3
#define	diagonal_45_degree_edge		4
#define	diagonal_135_degree_edge	5

IplImage *imageRGB;
IplImage *gray;
IplImage *img;

CvMat *ver, *hor, *dia45, *dia135, *nond;
int GetBlockSize(int width, int height);
int GetEdgeFeature(int i, int j, unsigned long block_size);
double *EdgeCalculation(unsigned long image_width, unsigned long image_height, unsigned long block_size);
double FirstBlock(int i, int j, int block_size);
double SecondBlock(int i, int j, int block_size);
double ThirdBlock(int i, int j, int block_size);
double FourthBlock(int i, int j, int block_size);
double Local_Edge_Histogram[80];

int main(int argc, char** argv)
{
	int i;
	unsigned long block_size;
	double *EDHTable;
	

	imageRGB  = cvLoadImage(argv[1], 1);
	gray = cvCreateImage(cvGetSize(imageRGB), 8, 1);
	cvCvtColor(imageRGB, gray, CV_BGR2GRAY );
	cvNamedWindow( "gray IMAGE", 1 );
	cvShowImage( "gray IMAGE", gray);

	unsigned int h = gray->height;
	unsigned int w = gray->width;
	printf("height = '%d' , width = '%d'", h, w);

	block_size = GetBlockSize(w, h);
	printf("blocksize = '%lu'\n", block_size);

/* 	int img_size = gray->width * gray->height * gray->nChannels;
	unsigned char* data = (unsigned char*) malloc(img_size);
	memcpy(data, gray->imageData, img_size); */
	
	EDHTable = EdgeCalculation(w, h, block_size);
	
	int k;
    for (k = 0; k < 80; k++) {
        printf(" %f \n" , EDHTable[k]);
    }
	
	cvWaitKey(0);
	free(EDHTable);
	cvDestroyWindow( "imageGray" );
}


double *EdgeCalculation(unsigned long image_width, unsigned long image_height, unsigned long block_size,  double *){
	int Offset, EdgeTypeOfBlock;
	int i, j;
	int Count_Local[16],local_index;
	double *Local_Edge = malloc(sizeof(double)*80);
	
	// Clear
	memset(Count_Local, 0, 16*sizeof(int));
	
	for(i=0; i<=image_height-block_size; i+=block_size){
		for(j=0; j<=image_width-block_size; j+=block_size){
		
		local_index = (int)(j*4/image_width)+(int)(i*4/image_height)*4;
	    Count_Local[local_index]++;	
		EdgeTypeOfBlock=GetEdgeFeature(i,j,block_size);	
		
		switch(EdgeTypeOfBlock) {
	    case NoEdge:
	      break;
	    case vertical_edge:
	      Local_Edge[local_index*5]++;
	      break;
	    case horizontal_edge:
	      Local_Edge[local_index*5+1]++;
	      break;
	    case diagonal_45_degree_edge:
	      Local_Edge[local_index*5+2]++;
	      break;
	    case diagonal_135_degree_edge:
	      Local_Edge[local_index*5+3]++;
	      break;
	    case non_directional_edge:
	      Local_Edge[local_index*5+4]++;
	      break;
	    } 
	}
	}
	int k;
    for (k = 0; k < 80; k++) {
        Local_Edge[k] /= Count_Local[(int)k / 5];
    }
    return (Local_Edge);
}

int GetEdgeFeature(int i, int j, unsigned long block_size){
	int z,h,k,e;
	int e_index;
	double e_h, e_v, e_45, e_135, e_m, e_max;
	double strengths[5];
	double EdgeFilters[5][4] = {{1.0, -1.0, 1.0, -1.0},
								{1.0, 1.0, -1.0, -1.0},
								{sqrt(2), 0.0, 0.0, -sqrt(2)},
								{0.0, sqrt(2), -sqrt(2), 0.0},
								{2.0, -2.0, -2.0, 2.0}};
								
/* 	for(z = j+1; z <= block_size+j - 1; z++){
		for(h=i+1; h <= block_size+i - 1; h++){
		if(z <= ((block_size/2) + i)){
			if(h <= ((block_size/2) + j)){ 
				s = cvGet2D(gray,z,h);
				d1 += s.val[0];
			}else{
				s = cvGet2D(gray,z,h);
				d2 += s.val[0];
				}
		}else{
			if(h <= ((block_size/2)+j)){
				s = cvGet2D(gray,z,h);
				d3 += s.val[0];
			}else{
				s = cvGet2D(gray,z,h);
				d4 += s.val[0];
			}
		}
	}
	} */
	
	//Calculate the average of gray levels in each sub-block of the (i,j)th imageblock
	double average[4] = {FirstBlock(i,j,block_size), SecondBlock(i,j,block_size), ThirdBlock(i,j,block_size), FourthBlock(i,j,block_size)};
	printf("average: '%f', '%f', '%f', '%f' \n\n",average[0],average[1],average[2],average[3] );

	for (e = 0; e < 5; e++){
        for (k = 0; k < 4; k++){
            strengths[e] += average[k] * EdgeFilters[e][k];
        }
        strengths[e] = fabs(strengths[e]);
    }
		
	e_v = strengths[0];
	e_h = strengths[1];
	e_45 = strengths[2];
	e_135 = strengths[3];
	e_m = strengths[4];
	//printf("edge values: '%f', '%f', '%f', '%f', '%f' \n", e_h, e_v, e_45, e_135, e_m);
	
	e_max = e_v;
	e_index = vertical_edge;
	if(e_h > e_max){
		e_max = e_h;
		e_index = horizontal_edge;
	}
	if(e_45 > e_max){
		e_max = e_45;
		e_index = diagonal_45_degree_edge;
	}
	if(e_135 > e_max){
		e_max = e_135;
		e_index = diagonal_135_degree_edge;
	}
	if(e_m > e_max){
		e_max = e_m;
		e_index = non_directional_edge;
	}
	if(e_max < 1)
		e_index = NoEdge;
	return(e_index);
}

double FirstBlock(int i, int j, int block_size) {
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = i; m < (block_size/2)+i; m++) {
        for (n = j; n < (block_size/2)+j; n++) {
            s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
        }
    }
      
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
double SecondBlock(int i, int j, int block_size) {
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = i; m < (block_size/2)+i; m++) {
        for (n = (block_size/2)+j; n < block_size+j; n++) {
 			s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
		}
    }
      
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
double ThirdBlock(int i, int j, int block_size) {
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = (block_size/2)+i; m < (block_size+i); m++) {
        for (n = j; n < (block_size/2)+j; n++) {
 			s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
		}
    }
      
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
double FourthBlock(int i, int j, int block_size) {
    int m,n;
	double average_brightness = 0;
	CvScalar s;
	for (m = (block_size/2)+i; m < block_size+i; m++) {
        for (n = (block_size/2)+j; n < block_size+j; n++) {
 			s = cvGet2D(gray,m,n);
            average_brightness = average_brightness + s.val[0];
		}
    }
      block_size*block_size/4.0
    double bs = block_size * block_size;
    double div = 4 / bs;
    average_brightness = average_brightness * div;
    return average_brightness;
 }
/*
	height e width representam o tamanho horizontal
	e vertical da imagem, respectivamente.
*/
int GetBlockSize(int width, int height){
	unsigned long x, block_size;
	int num_block = 16; //numero de blocos desejados
	
	x = sqrt((width*height)/num_block);
	block_size = abs(x/2)*2;
	
	if(block_size<2) block_size = 2;
	
	return block_size;
}

