/*
 * cornermetric
 *
 * cornermetric in scilab
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of cornermetric method of matlab
// Usage:
// cornermetric(I,'harris')
// cornermetric(I,'minimimeigenvalue')
// cornermetric(I,'harris',k)
//
// Two methods are provided - Harris Corner Detection, and Shi and Tomasi's
// minimum eigenvalue method
//
// The input image I, must be grayscale
//
// k is the sensitivity factor, to be used with the harris corner detection
// method. By default, it is 0.04. It accepts a value between 0 and 0.25.
//
// Known Changes from Matlab:
/*
 * 1) normalizes the measure for easy visulization and thresholding, does not
 * use smoothing - this option is left upto the user
 */

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

int opencv_cornermetric(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;

  int *piAddr1 = NULL;
  int *piAddr3 = NULL;

  int error;

  // String holding the second argument
  int iRet = 0;
  char *pstData = NULL;

  // Checking input argument
  CheckInputArgument(pvApiCtx, 2, 3);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Get input image

  Mat image;
  retrieveImage(image, 1);

  //  for (int i = 0; i < image.rows; i++) {
  //    for (int j = 0; j < image.cols; j++) {
  //      sciprint("%f ", image.at<double>(i,j));

  //    }

  // sciprint("\n");
  //  }

  // Error Checks

  if (image.channels() > 1) {
    sciprint("The image must be grayscale.");
    return 0;
  }

  Mat new_image;

  // Get the number of input arguments
  int inputarg = *getNbInputArgument(pvApiCtx);

  if (inputarg >= 2)

  {
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr1);

    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }

    if (isStringType(pvApiCtx, piAddr1)) {
      if (isScalar(pvApiCtx, piAddr1)) {
        iRet = getAllocatedSingleString(pvApiCtx, piAddr1, &pstData);
      }
    }
    if (strcmp(pstData, "harris") == 0) {
      // sensitivity factor for harris corner detection
      double k = 0.04;

      if (inputarg == 3) {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);

        if (sciErr.iErr) {
          printError(&sciErr, 0);
          return 0;
        }

        intErr = getScalarDouble(pvApiCtx, piAddr3, &k);
        if (sciErr.iErr) {
          printError(&sciErr, 0);
          return intErr;
        }

        if ((k <= 0) || (k >= 0.25)) {
          sciprint(
              "Sensitivity factor for harris corner detection must be in the "
              "range [0,0.25]");
          return 0;
        }
      }

      cornerHarris(image, new_image, 2, 3, k, BORDER_DEFAULT);
      normalize(new_image, new_image, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
      convertScaleAbs(new_image, new_image);

    } else if (strcmp(pstData, "minimumeigenvalue") == 0) {
      cornerMinEigenVal(image, new_image, 2, 3, BORDER_DEFAULT);

      normalize(new_image, new_image, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
      convertScaleAbs(new_image, new_image);

      if (inputarg > 2) {
        sciprint("Invalid number of arguments to function\n", pstData);
        return 0;
      }
    }

    else {
      sciprint("Unknown Parameter Name:%s\n", pstData);
      return 0;
    }
  }

  // sciprint("\n");

  //  for (int i = 0; i < new_image.rows; i++) {
  //    for (int j = 0; j < new_image.cols; j++) {
  //      sciprint("%f ", new_image.at<double>(i,j));

  //    }

  // sciprint("\n");
  //  }

  // new_image is sent to scilab as output

  int temp = nbInputArgument(pvApiCtx) + 1;
  string tempstring = type2str(new_image.type());
  char *checker;
  checker = (char *)malloc(tempstring.size() + 1);
  memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
  returnImage(checker, new_image, 1);
  free(checker);

  // Assigning the list as the Output Variable
  AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
  // Returning the Output Variables as arguments to the Scilab environment
  ReturnArguments(pvApiCtx);
  return 0;
}
/* ==================================================================== */
}
