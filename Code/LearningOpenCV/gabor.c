


float GaborFilte(int x, int y, float lambda, float theta, float phi, float sigma, float gamma) {
    
	float xx =  x * cos(theta) + y * sin(theta);
    	float yy = -x * sin(theta) + y * cos(theta);
    	float envelopeVal = exp( - ( (xx*xx + gamma*gamma* yy*yy) / (2.0f * sigma*sigma) ) );
 	float carrierVal = cos( 2.0f * (float)M_PI * xx / lambda + phi);
	float g = envelopeVal * carrierVal;
 
	return g;
}
