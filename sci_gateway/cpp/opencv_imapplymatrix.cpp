/********************************************************
Author: Samiran Roy
********************************************************/


//M = Multipler Matrix : Matrix that contains the coefficients of the weighting matrix. If X is m-by-n-by-p, M must be q-by-p, where q is in the range [1,p].
//This is incorrectly described in Matlab, as imapplymatrix([],[]) gives []

#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <typeinfo>

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
  
  int opencv_imapplymatrix(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr = 0;
  

// the multiplier matrix
 //double *M = NULL;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;


// Check the number of output arguments
CheckOutputArgument(pvApiCtx, 1, 1) ;
// Check the number of input arguments
CheckInputArgument(pvApiCtx, 2, 4);



    // Get the number of input arguments
    int inputarg=*getNbInputArgument(pvApiCtx);

// Get the image
 Mat image;
 retrieveImage(image, 2);

// Get number of channels

   int num_channels=image.channels();






  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      sciprint("%f ", image.at<double>(i,j));

    }
    
 sciprint("\n");  
  }






// getting the coefficient matrix

 double* M;

  int iRows = 0, iCols = 0;

  sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return -1;
  }
  sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &M);
  if (sciErr.iErr) {
    printError(&sciErr, 0);
    return -1;
  }



// Initial checks 

if (iCols!=num_channels)
{
sciprint("The number of columns in the multiplier must match the size of the last dimension in the image.\n");
return -1;
}

if (iRows>num_channels)
{
sciprint("The number of rows in the multiplier must be less than or equal to the number of columns.\n");
return 0;
}


Mat tempimg;
image.copyTo(tempimg);
tempimg.convertTo(tempimg,CV_32F);
// temporary variable to do manipulation on the input image

// Creating the output image

    Mat new_image;


// Channels of the input image
Mat channels[num_channels];
split(tempimg, channels);


// Channels of the output image
Mat outplanes[iRows];


switch (inputarg)

{
case(1):

sciprint("Needs atleast two arguments.\n");
return 0;

break;
case(2):

Mat temp1,temp2;
for (int i=0;i<iRows;i++)

{

outplanes[i] = Mat::zeros(Size(image.rows, image.cols), channels[0].type());

// Iterate over the multipler matrixs
for (int j=0;j<iCols;j++)
{
outplanes[i]=outplanes[i]+(channels[j]*M[i,j]);
break;
}


}

break;




}





merge(outplanes,iRows,new_image);







 sciprint("\n");  


  for (int i = 0; i < new_image.rows; i++) {
    for (int j = 0; j < new_image.cols; j++) {
      sciprint("%f ", new_image.at<double>(i,j));

    }
    
 sciprint("\n");  
  }




    int temp = nbInputArgument(pvApiCtx) + 1;
    string tempstring = type2str(new_image.type());
    char *checker;
    checker = (char *)malloc(tempstring.size() + 1);
    memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
    returnImage(checker,new_image,1);
    free(checker); 

    //Assigning the list as the Output Variable
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    //Returning the Output Variables as arguments to the Scilab environment
    ReturnArguments(pvApiCtx);
    return 0;

  }
/* ==================================================================== */
}
