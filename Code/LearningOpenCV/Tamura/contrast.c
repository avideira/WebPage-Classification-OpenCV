double Contrast(Matrix image)
{
  double var, norm4, mean, diff;
  int y,x;

  mean = MatrixMean(image);
  var = 0;
  norm4 = 0;
  for(y=0;y<image->height;y++) {
    for(x=0;x<image->width;x++) {
      diff = image->data[y][x] - mean;
      var += diff*diff;
      norm4 += diff*diff*diff*diff;
    }
  }
  var /= (image->width * image->height);
  norm4 /= (image->width * image->height);
  if(norm4 == 0) return 0;
  return var / pow(norm4,0.25) / 128; /* normalize by maximum value */

}
