/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Created By: Riddhish Bhalodia
Date: 5th September 2015

Usage:

Mat dst =  gray2ind(Mat img,int n)

1)  img   : Input image, grayscale only
2)  n     : Index scale factor
3)  dst   :	Output Image


*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "sciprint.h"
  #include "../common.h"


int opencv_gray2ind(char *fname, unsigned long fname_len){

	SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int *piAddr4  = NULL;
    int *piAddr5  = NULL;
    int i,j,k;
    double x, y, width, height;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image, 1);

    //for value of top-left x-coordinate
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &x);
    if(intErr)
    {
        return intErr;
    }   


}
Mat opencv_gray2ind(Mat img,int n){
	// check that image is gray scale or not
	if(img.channels() ==3 ) {
		Mat dst = Mat::zeros(img.size(),img.type());
		double scl_factor = (double)n/255.0;
		double temp = 0;
		for(int i=0;i<img.rows;i++){
			for(int j=0;j<img.cols;j++){
				for(int c=0;c<3;c++){
					temp = ((double)img.at<Vec3b>(i,j)[c])*scl_factor;
					dst.at<Vec3b>(i,j)[c] = temp;
				}
			}
		}
		return dst;
	}
	else{
		Mat dst = Mat::zeros(img.size(),img.type());
		double scl_factor = (double)n/255.0;
		double temp = 0;
		for(int i=0;i<img.rows;i++){
			for(int j=0;j<img.cols;j++){
				temp = ((double)img.at<uchar>(i,j))*scl_factor;
				dst.at<uchar>(i,j) = temp;
			}
		}
		return dst;
	}
}

int main(){
	Mat in = imread("barbara.png");
	// cvtColor(in,in,CV_BGR2GRAY);
	Mat out;
	out = Mat::zeros(in.size(),in.type());
	out = opencv_gray2ind(in,64);
	imshow("SHIT",in);
	imshow("SHIT2",out);
	waitKey(0);
	return 0;
}