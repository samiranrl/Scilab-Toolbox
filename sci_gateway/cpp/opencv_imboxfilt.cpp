/*
 * imboxfilt
 *
 * m*n box filter convolution in scilab
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of imboxfilt method
// Usage:
// 1) imboxfilt(I) : Convolve image I with 3*3 box kernel
// 2) imboxfilt(I,k_size) : Convolve image I with k_size*k_size box kernel

// k_size is an odd positive scalar >=3, or a 1*2 vector of odd positive scalars
// >=3
// signifying the number of rows and columns of the kernels

// 3) imboxfilt(I,ksize,'padding')

// 'padding' : how to handle the borders
// value: 'default' : replicate the border padding, or take the value of the
// border pixel to be the closest image border pixel
// value: 'symmetric' : take the border values to be a reflection of the image
// value: 'circular' : take the border values assuming that the image is
// periodic
// value: 'constant' : pad the image with the provided scalar double

// 4) imboxfilt(I,ksize,'padding',NormalizationFactor)

// 'NormalizationFactor' :
// value: any scalar: this is multiplied to the image after convolving with
// kernel = ones(k_size,k_size)

// example: imboxfilt(I,3,0,1) takes area sums over a 3*3 neighborhood

// Known Changes from Matlab:
/*
 * None, as of now - except replacing Name - Value Pairs with an overloaded
 * function
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

int opencv_imboxfilt(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;
  int iRows = 0, iCols = 0;
  int *piAddr = NULL;

  int *piAddr2 = NULL;
  int *piAddr3 = NULL;
  int *piAddr4 = NULL;

  // Setting default border handling to to replicate
  int borderType = BORDER_REPLICATE;
  double borderValue = BORDER_REPLICATE;

  // Error flags - These are set to 1 if i need to handle the situation manually
  // (it means: OpenCV does not have this explicit implementation)
  int handle_constant_border = 0;
  int handle_circular_border = 0;

  // The type of padding - entered by user
  char *padding;
  // Used to get a string from user in argument 3
  int iRet = 0;

  // The kernel used for convolution - initilalized later according to input
  // parameters
  Mat kernel;

  // Default kernel size, the result is normalized according to this kernel
  double k_sizex = 3;
  double k_sizey = 3;
  double NormalizationFactor = 1 / ((k_sizex) * (k_sizey));

  // Parameters for convolution - Leave these alone

  /* **********************************************************************************
   */

  Point anchor;
  double delta;
  int ddepth;

  anchor = Point(-1, -1);  // The center is unchanged
  delta = 0;               // No value is added to output
  ddepth = -1;  // The dimentions of input and output images are the same

  /* **********************************************************************************
   */

  // checking input argument
  CheckInputArgument(pvApiCtx, 1, 4);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Get the number of input arguments
  int inputarg = *getNbInputArgument(pvApiCtx);

  // get input image
  Mat image;
  retrieveImage(image, 1);

  //  for (int i = 0; i < image.rows; i++) {
  //    for (int j = 0; j < image.cols; j++) {
  //      sciprint("%f ", image.at<double>(i,j));

  //    }

  // sciprint("\n");
  //  }

  // In case kernel is different in different dimentions
  double *ksize;

  if (inputarg >= 2) {
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }
    // Get size of box filter

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &ksize);
    if (sciErr.iErr) {
      sciprint(
          "Filter size must be an odd positive scalar or a 1*2 matrix of odd "
          "positive scalars\n");
      return -1;
    }

    if (iCols == 1 && iRows == 1) {
      intErr = getScalarDouble(pvApiCtx, piAddr2, &k_sizex);

      k_sizey = k_sizex;

      if (k_sizex <= 0 || static_cast<int>(k_sizex) % 2 == 0) {
        sciprint(
            "Filter size must be an odd positive scalar or a 1*2 matrix of odd "
            "positive scalars\n");
        return -1;
      }
    }

    else {
      if (!(iCols == 2)) {
        sciprint(
            "Filter size must be an odd positive scalar or a 1*2 matrix of odd "
            "positive scalars\n");

        return -1;
      }

      k_sizex = ksize[ 0, 0 ];
      k_sizey = ksize[ 0, 1 ];

      if (k_sizex <= 0 || static_cast<int>(k_sizex) % 2 == 0) {
        sciprint(
            "Filter size must be an odd positive scalar or a 1*2 matrix of odd "
            "positive scalars\n");
        return -1;
      }

      if (k_sizey <= 0 || static_cast<int>(k_sizey) % 2 == 0) {
        sciprint(
            "Filter size must be an odd positive scalar or a 1*2 matrix of odd "
            "positive scalars\n");
        return -1;
      }
    }

    // Update normalization factor according to the new size
    NormalizationFactor = 1 / ((k_sizex) * (k_sizey));
  }

  // Get the type of padding from user

  if (inputarg >= 3) {
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);

    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }

    if (isStringType(pvApiCtx, piAddr3)) {
      if (isScalar(pvApiCtx, piAddr3)) {
        iRet = getAllocatedSingleString(pvApiCtx, piAddr3, &padding);
      }

      // Checks the first name in the name - value pairs
      if (strcmp(padding, "default") == 0) {
        // Nothing to do here
      }

      else if (strcmp(padding, "circular") == 0) {
        // I have to handle this case manually since opencv does not allow
        // convolution with this border padding type
        handle_circular_border = 1;

      }

      else if (strcmp(padding, "symmetric") == 0) {
        borderType = BORDER_REFLECT;

      }

      else {
        sciprint(
            "Invalid border argument: Accepts 'replicate', 'symmetric', "
            "'circular' or a scalar double\n");
        return -1;
      }
    }

    else
    // User enters a constant scalar double as border padding type

    {
      intErr = getScalarDouble(pvApiCtx, piAddr3, &borderValue);
      if (sciErr.iErr) {
        printError(&sciErr, 0);
        return intErr;
      }
      // I have to handle this case manually since opencv does not allow
      // convolution with this border padding type
      handle_constant_border = 1;
    }
  }

  if (inputarg == 4) {
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);
    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }
    // Get the Normalization Factor from user
    intErr = getScalarDouble(pvApiCtx, piAddr4, &NormalizationFactor);
    if (sciErr.iErr) {
      return intErr;
    }
  }

  int pad_sizex = static_cast<int>(k_sizex / 2);
  int pad_sizey = static_cast<int>(k_sizey / 2);

  // initializing the box filter kernel used for convolution

  kernel = Mat::ones(k_sizex, k_sizey, CV_32F) * (NormalizationFactor);

  Mat new_image;
  Mat tempimg;

  // These are my implementations for flagged cases - circular border padding
  // and constant border padding
  // I manually pad the image, perform convolution and extract the required area

  if (handle_circular_border == 1) {
    copyMakeBorder(image, tempimg, pad_sizey, pad_sizey, pad_sizex, pad_sizex,
                   BORDER_WRAP);

    filter2D(tempimg, tempimg, ddepth, kernel, anchor, delta, BORDER_CONSTANT);

    new_image = tempimg(Range(pad_sizey, tempimg.rows - pad_sizey),
                        Range(pad_sizex, tempimg.cols - pad_sizex));

  } else if (handle_constant_border == 1)

  {
    copyMakeBorder(image, tempimg, pad_sizey, pad_sizey, pad_sizex, pad_sizex,
                   BORDER_CONSTANT, borderValue);

    filter2D(tempimg, tempimg, ddepth, kernel, anchor, delta, borderType);

    new_image = tempimg(Range(pad_sizey, tempimg.rows - pad_sizey),
                        Range(pad_sizex, tempimg.cols - pad_sizex));

  }

  else {
    // performing convolution
    filter2D(image, new_image, ddepth, kernel, anchor, delta, borderType);
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
}
