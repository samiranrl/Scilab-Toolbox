/*
 * imboxfilt
 *        
 * k_size*k_size box filter convolution in scilab
 *     
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of imboxfilt method in matlab
// Usage: 
// imboxfilt(I,k_size)
// imboxfilt(I)
// imboxfilt(I,name_value)

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
  
  int opencv_imboxfilt(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
    int i,j,k;

    

    Point anchor;
    double delta;
    int ddepth;
    int k_size=3;
    Mat kernel;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 6);
    CheckOutputArgument(pvApiCtx, 1, 1) ;


 // Get the number of input arguments
    int inputarg=*getNbInputArgument(pvApiCtx);



    //get input matrix

    Mat image;
    retrieveImage(image, 1);





    //for size of box filter

if (inputarg==2)
{
    sciErr = getVarAddressFromPosition(pvApiCtx,2,&piAddr2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    intErr = getScalarDouble(pvApiCtx, piAddr2, &k_size);
    if(sciErr.iErr)
    {
        return intErr;
    }   
    
  if(k_size<=0 || static_cast<int>(k_size) % 2 == 0)
    {
        sciprint("Invalid size of filter - should be an odd, positive integer\n");
        return 0;
    }
 

}











    Mat new_image;
  

  
    
    //initializing the box filter kernel used for convolution

    anchor = Point( -1, -1 );  //The center is unchanged
    delta = 0; //No value is added to output
    ddepth = -1; //The dimentions of input and output images are the same
    kernel = Mat::ones( k_size, k_size, CV_32F )/ (float)(k_size*k_size);

    //performing convolution
    filter2D(image, new_image, ddepth , kernel, anchor, delta, BORDER_DEFAULT );

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
