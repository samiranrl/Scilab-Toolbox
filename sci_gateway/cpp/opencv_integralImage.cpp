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
extern "C"
{
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "BOOL.h"
  #include <localization.h>
  #include "sciprint.h"
  #include "../common.h"
  
  int opencv_integralImage(char *fname, unsigned long fname_len)
  {
    SciErr sciErr;
    int intErr = 0;
    int iRows=0,iCols=0;
    int *piAddr = NULL;
  int *piAddr1 = NULL;
    int *piAddrNew = NULL;
    int *piAddr2  = NULL;
    int *piAddr3  = NULL;
  
  int error;
        int iRet    = 0;
        char* pstData = NULL;


 
    
    Point anchor;
    double delta;
    int ddepth;
    int kernel_size;
    Mat kernel;

    //checking input argument
    CheckInputArgument(pvApiCtx, 1, 2);
    CheckOutputArgument(pvApiCtx, 1, 1) ;

    //get input matrix

    Mat image;
    retrieveImage(image, 1);





 // error Checks


if (image.channels()>1)
{
sciprint("The image must be grayscale."); 
return 0;
}
 


    Mat new_image,integralimage,squaredimage,rotatedimage;


integral(image,integralimage,squaredimage,rotatedimage,-1);



  // Get the number of input arguments
    int inputarg=*getNbInputArgument(pvApiCtx);

if (inputarg==1)
integralimage.copyTo(new_image);

if (inputarg==2)

{
 sciErr = getVarAddressFromPosition(pvApiCtx, 2,&piAddr1);

if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	if(isStringType(pvApiCtx, piAddr1))
	{
		if(isScalar(pvApiCtx, piAddr1))
		{
			
                        iRet = getAllocatedSingleString(pvApiCtx, piAddr1, &pstData);
                 }
        }
   if(strcmp(pstData,"rotated")==0)
       {
          rotatedimage.copyTo(new_image);
       }
else if (strcmp(pstData,"upright")==0)
{
integralimage.copyTo(new_image);
}

else if (strcmp(pstData,"upright")==0)
{
integralimage.copyTo(new_image);
}






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
