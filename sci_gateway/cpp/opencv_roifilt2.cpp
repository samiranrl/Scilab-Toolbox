/*
 * roifilt2
 *
 * roifilt2 in scilab
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of integralImage method of matlab
// Usage:
// roifilt2(filter,image,roi)

// roifilt filters the input image with the given two-dimensional filter
// roi is a binary image the same size as I that defines an ROI used as a mask
// for filtering.
// roifilt2 returns an image that consists of filtered values for pixels in
// locations where roi contains 1's,
// and unfiltered values for the rest of the pixels

// Known Changes from Matlab:
/*
 * 1) Not using parameterizing functions implementation
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

int opencv_roifilt2(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;

  int *piAddr1 = NULL;

  int error;

  // Checking input argument
  CheckInputArgument(pvApiCtx, 3, 3);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Get filter

  Mat filter;
  retrieveImage(filter, 1);

  // Get input image

  Mat image;
  retrieveImage(image, 2);

  // Get roi

  Mat roi;
  retrieveImage(roi, 3);

  //  for (int i = 0; i < image.rows; i++) {
  //    for (int j = 0; j < image.cols; j++) {
  //      sciprint("%f ", image.at<double>(i,j));

  //    }

  // sciprint("\n");
  //  }

  // Error Checks

  if (image.channels() > 1) {
    sciprint("The image must be a two dimentional array.");
    return 0;
  }

  if (roi.channels() > 1) {
    sciprint("The region of interest must be a two dimentional array.");
    return 0;
  }

  if (filter.channels() > 1) {
    sciprint("The filter must be a two dimentional array.");
    return 0;
  }

    if ((image.rows!=roi.rows)||(image.cols!=roi.cols)) {
    sciprint("The roi matrix must be the same size as the image.");
    return 0;
  }

 Mat new_image;

  Point anchor = Point(-1, -1);
  double delta = 0;
  int ddepth = -1;

  /// Apply filter
  filter2D(image, new_image, ddepth, filter, anchor, delta, BORDER_DEFAULT);

  // Filter again using roi
  for (int i = 0; i < roi.rows; i++) {
    for (int j = 0; j < roi.cols; j++) {
      if (roi.at<double>(i, j) == 0) {
        new_image.at<double>(i, j) = image.at<double>(i, j);
      }
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
