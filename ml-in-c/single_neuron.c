#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#define ROW_CNT 5
float data[ROW_CNT][2] = {
//	 x  y
//-----------------------
	{1, 5},
	{2, 10},
	{3, 15},
	{4, 20},
	{5, 25}
};

// MODEL: y = x * w
// w is a parameter of the model
// x is the input data
// y is the predicted output


// cost function: Gives how erroneous predictions are
float cost(float w){
	float summed_error = 0;
	for(int i=0;i<=ROW_CNT;i++){
		float x = data[i][0];
		float y = data[i][1];
		float prediction = x * w;
		float error = y - prediction;
		float squared_error = error * error;
		summed_error += squared_error;
	}
	summed_error /= ROW_CNT;
	return summed_error;
}

// Gives the direction and intensity of slope
// slope is the tendency of the function's growth
// High values mean high slopes
// Slope of 0 means no slope at all
// Positive slope means function increases with independent variable
// Negative slope means function decreases with independent variable
float derivative(float w){
	float eps = 1.0e-2;
	float der = (cost(w+eps) - cost(w))/eps;
	return der;
}

int main(void){
	srand(time(0));
	//float w = ((float)rand()/(float)RAND_MAX) * 100;
	float w = -90;
	float learning_rate = 1.0e-3;
	printf("Randomized Parameter: %f\n",w);
	printf("Parameter: %.2f, Summed Error: %.2f, Der_Cost_At_w: %.2f\n",w,cost(w),derivative(w));
	
	float threshold = 1.0e-3;
	//optimization loop (finding parameter that minimizes cost function)
	while((fabs(derivative(w))) >= threshold){
		w -= learning_rate * derivative(w);
		printf("Parameter: %.2f, Der_Cost_At_w: %f\n",w,derivative(w));
	}	
	return 0;
}
