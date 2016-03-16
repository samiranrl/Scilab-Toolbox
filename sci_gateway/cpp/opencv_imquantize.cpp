/*
 * imquantize
 *
 * imquantize in scilab
 *
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of inquantize method of matlab
// Usage:
// imquantize(I) : Quantizes the image according to an input array of increasing quantization levels

// Known Changes from Matlab:
/*
 * 1) None, as of now
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

int opencv_imquantize(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;
  int iRows = 0, iCols = 0;    // for levels
  int iRows2 = 0, iCols2 = 0;  // for values
  int *piAddr1 = NULL;
  int *piAddr2 = NULL;
  int *piAddr3 = NULL;

  int check;
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

  image.convertTo(image, CV_64FC1, 1, 0);

  //  for (int i = 0; i < image.rows; i++) {
  //    for (int j = 0; j < image.cols; j++) {
  //      sciprint("%f ", image.at<double>(i,j));

  //    }

  // sciprint("\n");
  //  }

  Mat new_image;

  // Get the number of input arguments
  int inputarg = *getNbInputArgument(pvApiCtx);

  double *values;
  double *levels;

  if (inputarg >= 2)

  {
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr2, &iRows, &iCols, &levels);
    if (sciErr.iErr) {
      sciprint(
          "Error: Levels must be a strictly increasing 1*n matrix of scalars "
          "\n");
      return -1;
    }

    if (iRows != 1) {
      sciprint(
          "Error: Levels must be a strictly increasing 1*n matrix of scalars "
          "\n");

      return -1;
    }

    for (int i = 0; i < iCols - 1; i++) {
      if (levels[ 0, i ] >= levels[ 0, i + 1 ])

      {
        sciprint(
            "Error: Levels must be a strictly increasing 1*n matrix of scalars "
            "\n");

        return -1;
      }
    }
  }

  if (inputarg == 3)

  {
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);
    if (sciErr.iErr) {
      printError(&sciErr, 0);
      return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddr3, &iRows2, &iCols2, &values); mn
    if (sciErr.iErr) {
      sciprint(
          "Error: Values 1*(n+1) matrix of scalars, where n is the number of "
          "quantization levels "
          "\n");
      return -1;
    }

    if (iRows2 != 1) {
      sciprint(
          "Error: Values 1*(n+1) matrix of scalars, where n is the number of "
          "quantization levels "
          "\n");

      return -1;
    }

    if (iCols != iCols2 - 1) {
      sciprint(
          "Error: Values 1*(n+1) matrix of scalars, where n is the number of "
          "quantization levels "
          "\n");
      return -1;
    }

    // for (int i = 0; i < iCols2; i++) {
    //   sciprint("%f ", values[ 0, i ]);
    // }

  }

  else {
    values = (double *)malloc(sizeof(double) * (iCols + 1));

    for (int i = 0; i < iCols + 1; i++) {
      values[i] = i + 1;
    }
  }

  // for (int i = 0; i < iCols; i++) {
  //   sciprint("%f ", levels[ 0, i ]);
  // }
  if (image.channels() == 1) {
    new_image = Mat::zeros(image.size(), CV_64FC1);

    for (int i = 0; i < image.rows; i++) {
      for (int j = 0; j < image.cols; j++) {
        if ((image.at<double>(i, j) > 0) &&
            (image.at<double>(i, j) <= levels[ 0, 0 ])) {
          new_image.at<double>(i, j) = values[ 0, 0 ];
        }

        else {
          check = 0;
          for (int k = 0; k < iCols - 1; k++) {
            if ((image.at<double>(i, j) > levels[ 0, k ]) &&
                (image.at<double>(i, j) <= levels[ 0, k + 1 ])) {
              new_image.at<double>(i, j) = values[ 0, k + 1 ];
              check = 1;
            }
            if (check == 0) {
              new_image.at<double>(i, j) = values[ 0, iCols ];
            }
          }
        }
      }
    }
    transpose(new_image, new_image);
  }

  else if (image.channels() == 3) {
    Mat rgb[3];

    split(image, rgb);

    for (int i = 0; i < rgb[0].rows; i++) {
      for (int j = 0; j < rgb[0].cols; j++) {
        if ((rgb[0].at<double>(i, j) > 0) &&
            (rgb[0].at<double>(i, j) <= levels[ 0, 0 ])) {
          rgb[0].at<double>(i, j) = values[ 0, 0 ];
        }

        else {
          check = 0;
          for (int k = 0; k < iCols - 1; k++) {
            if ((rgb[0].at<double>(i, j) > levels[ 0, k ]) &&
                (rgb[0].at<double>(i, j) <= levels[ 0, k + 1 ])) {
              rgb[0].at<double>(i, j) = values[ 0, k + 1 ];
              check = 1;
            }
            if (check == 0) {
              rgb[0].at<double>(i, j) = values[ 0, iCols ];
            }
          }
        }
      }
    }

    for (int i = 0; i < rgb[1].rows; i++) {
      for (int j = 0; j < rgb[1].cols; j++) {
        if ((rgb[1].at<double>(i, j) > 0) &&
            (rgb[1].at<double>(i, j) <= levels[ 0, 0 ])) {
          rgb[1].at<double>(i, j) = values[ 0, 0 ];
        }

        else {
          check = 0;
          for (int k = 0; k < iCols - 1; k++) {
            if ((rgb[1].at<double>(i, j) > levels[ 0, k ]) &&
                (rgb[1].at<double>(i, j) <= levels[ 0, k + 1 ])) {
              rgb[1].at<double>(i, j) = values[ 0, k + 1 ];
              check = 1;
            }
            if (check == 0) {
              rgb[1].at<double>(i, j) = values[ 0, iCols ];
            }
          }
        }
      }
    }

    for (int i = 0; i < rgb[2].rows; i++) {
      for (int j = 0; j < rgb[2].cols; j++) {
        if ((rgb[2].at<double>(i, j) > 0) &&
            (rgb[2].at<double>(i, j) <= levels[ 0, 0 ])) {
          rgb[2].at<double>(i, j) = values[ 0, 0 ];
        }

        else {
          check = 0;
          for (int k = 0; k < iCols - 1; k++) {
            if ((rgb[2].at<double>(i, j) > levels[ 0, k ]) &&
                (rgb[2].at<double>(i, j) <= levels[ 0, k + 1 ])) {
              rgb[2].at<double>(i, j) = values[ 0, k + 1 ];
              check = 1;
            }
            if (check == 0) {
              rgb[2].at<double>(i, j) = values[ 0, iCols ];
            }
          }
        }
      }
    }

    transpose(rgb[0], rgb[0]);
    transpose(rgb[1], rgb[1]);
    transpose(rgb[2], rgb[2]);

    merge(rgb, 3, new_image);
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
