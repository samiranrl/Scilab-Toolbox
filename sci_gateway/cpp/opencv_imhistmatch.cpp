/********************************************************
 Author: Samiran Roy
 ********************************************************/

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
extern "C" {
#include "api_scilab.h"
#include "Scierror.h"
#include "BOOL.h"
#include <localization.h>
#include "sciprint.h"
#include "../common.h"






int opencv_imhistmatch(char *fname, unsigned long fname_len) {
	SciErr sciErr;
	int intErr = 0;
	
	int *piAddr = NULL;
	int *piAddrNew = NULL;
	int *piAddr2 = NULL;
	int *piAddr3 = NULL;

double num_bins= 256; // default number of bins for histogram calculation

	//checking input argument
	CheckInputArgument(pvApiCtx, 2, 3);
	CheckOutputArgument(pvApiCtx, 1, 1);


// Get the number of input arguments
	int inputarg = *getNbInputArgument(pvApiCtx);

	//get input matrix

	Mat image;
	retrieveImage(image, 1);


Mat ref;
	retrieveImage(ref, 2);

int case1=0;
if (image.channels()==1)

{
if (ref.channels()!=1)
{
sciprint("If A is a grayscale image, the ref inage must also be grayscale\n");
return 0;

}
else
{
case1=1;

}



}


if (image.channels()==3)

{
if (ref.channels()==1)
{
case1=2;
}
if (ref.channels()==3)
{
case1=3;
}


}


if (!((image.channels()==3) || (image.channels()==1)))
{
sciprint("Invalid Image\n");
return 0;


}




if (!((ref.channels()==3) || (ref.channels()==1)))
{
sciprint("Invalid Reference Image\n");
return 0;


}


if (inputarg==3)
{

// Get the number of bins for histogram calculation
sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
		if (sciErr.iErr) {
			printError(&sciErr, 0);
			return 0;
		}

intErr = getScalarDouble(pvApiCtx, piAddr3, &num_bins);
		if (sciErr.iErr) {
			return intErr;
		}

		if (num_bins<1) {
			sciprint(
					"Invalid number of histogram bins\n");
			return 0;
		}











}


int bins=(int)num_bins;




 Mat dst, hist;
 Mat new_image;
Mat refhist;

Mat cdf=Mat(bins,1, CV_8U, cvScalar(0));

Mat refcdf=Mat(bins,1, CV_8U, cvScalar(0));


switch(case1){
    case 1  :







// Calculating histogram of image
  image.convertTo(dst,CV_8U , 1, 0);
 calcHist(&dst, 1, 0, Mat(),hist, 1, &bins, 0);
		


// Calculating histogram of reference image

  ref.convertTo(dst, CV_8U , 1, 0);
 calcHist(&dst, 1, 0, Mat(),refhist, 1, &bins, 0);


hist.copyTo(cdf);
refhist.copyTo(refcdf);


// calculate cdf 
 for( int h = 1; h < bins; h++ )
         {
            float binVal = hist.at<float>(h,0);
         
cdf.at<float>(h,0)=cdf.at<float>(h,0)+cdf.at<float>(h-1,0);
         }

// normalize histogram
 for( int h = 0; h < bins; h++ )
         {
           
   
cdf.at<float>(h,0)=cdf.at<float>(h,0)/cdf.at<float>(bins-1,0);
         }

 
//for reference image



// calculate cdf 
 for( int h = 1; h < bins; h++ )
         {
            float binVal = refhist.at<float>(h,0);
         
refcdf.at<float>(h,0)=refcdf.at<float>(h,0)+refcdf.at<float>(h-1,0);
         }

// normalize histogram
 for( int h = 0; h < bins; h++ )
         {
           
   
refcdf.at<float>(h,0)=refcdf.at<float>(h,0)/refcdf.at<float>(bins-1,0);
         }


sciprint("%i\n", cdf.type());

 
image.copyTo(new_image);


for(int i = 0; i < new_image.rows; i++)
        for(int j=0; j < new_image.cols; j++){


            new_image.at<double>(i,j)=1;


}}

	int temp = nbInputArgument(pvApiCtx) + 1;
	string tempstring = type2str(new_image.type());
	char *checker;
	checker = (char *) malloc(tempstring.size() + 1);
	memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
	returnImage(checker, new_image, 1);
	free(checker);

	//Assigning the list as the Output Variable
	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
	//Returning the Output Variables as arguments to the Scilab environment
	ReturnArguments (pvApiCtx);
	return 0;

}
/* ==================================================================== */
}

