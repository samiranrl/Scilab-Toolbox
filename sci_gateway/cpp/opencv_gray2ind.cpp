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
    
	double temp = 0;	
    double n;

    //checking input argument
    CheckInputArgument(pvApiCtx, 2, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    Mat image;
    retrieveImage(image, 1);

    //for value of the scale factor
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &n);
    if(intErr)
    {
        return intErr;
    }   

    // The error checks for the function

    // The main function
    double scl_factor = n/255.0;

    Mat dst = Mat::zeros(image.size(),image.type());
    if(image.channels() ==3 ) {
		
		for(int i=0;i<image.rows;i++){
			for(int j=0;j<image.cols;j++){
				for(int c=0;c<3;c++){
					temp = ((double)image.at<Vec3b>(i,j)[c])*scl_factor;
					dst.at<Vec3b>(i,j)[c] = temp;
				}
			}
		}
		
	}
	else{
		
		double scl_factor = (double)n/255.0;
		
		for(int i=0;i<image.rows;i++){
			for(int j=0;j<image.cols;j++){
				temp = ((double)image.at<uchar>(i,j))*scl_factor;
				dst.at<uchar>(i,j) = temp;
			}
		}
		
	}

	string tempstring = type2str(dst.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,dst,1);
    free(checker); 
    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

}
}
