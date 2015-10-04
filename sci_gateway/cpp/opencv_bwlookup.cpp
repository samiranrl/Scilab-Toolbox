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

int opencv_bwlookup(char *fname, unsigned long fname_len) {
	SciErr sciErr;
	int intErr = 0;
	int iRows = 0, iCols = 0;
	int *piAddr = NULL;
	int *piAddrNew = NULL;
	int *piAddr2 = NULL;
	int *piAddr3 = NULL;



	//checking input argument
	CheckInputArgument(pvApiCtx, 1, 2);
	CheckOutputArgument(pvApiCtx, 1, 1);

	//get input matrix

	Mat image;
	retrieveImage(image, 1);

	//Retrieve lookup table

	Mat lut;
	retrieveImage(lut, 2);

	// Error Checking

	if (!((lut.cols == 16) || (lut.cols == 512))) {
		sciprint("Expected LUT (argument 2) to have 16 or 512 elements.\n");

		return 0;
	}

	if (lut.rows != 1) {
		sciprint("Expected input number 2, LUT, to be a vector.\n");
		return 0;
	}

	if (image.channels() != 1) {
		sciprint("Expected input number 1, A, to be two-dimensional.\n");
		return 0;
	}

	// temporary copy of image to perform computation
	Mat tempimg = Mat::zeros(image.size(), image.type());

	for (int i = 0; i < image.rows; i++) {

		for (int j = 0; j < image.cols; j++) {
			if (image.at<double>(i, j) != 0)
				tempimg.at<double>(i, j) = 1;

		}

	}

	//pad the temporary copy
	copyMakeBorder(tempimg, tempimg, 1, 1, 1, 1, BORDER_CONSTANT, 0);


        //output image
	Mat new_image = Mat::zeros(image.size(), image.type());

        //temporary variables
	int ii, jj;
	int index;

	//2*2 filtering
	if (lut.cols == 16) {

		for (int i = 0; i < image.rows; i++) {

			for (int j = 0; j < image.cols; j++) {
				ii = i + 1;
				jj = j + 1;

				index = tempimg.at<double>(ii, jj) * 1
						+ tempimg.at<double>(ii + 1, jj) * 2
						+ tempimg.at<double>(ii, jj + 1) * 4
						+ tempimg.at<double>(ii + 1, jj + 1) * 8;

				new_image.at<double>(j, i) = lut.at<double>(0, index);

			}

		}
	}

	//3*3 filtering
	if (lut.cols == 512) {

		for (int i = 0; i < image.rows; i++) {

			for (int j = 0; j < image.cols; j++) {
				ii = i + 1;
				jj = j + 1;

				index = tempimg.at<double>(ii - 1, jj - 1) * 1
						+ tempimg.at<double>(ii, jj - 1) * 2
						+ tempimg.at<double>(ii + 1, jj - 1) * 4
						+ tempimg.at<double>(ii - 1, jj) * 8
						+ tempimg.at<double>(ii, jj) * 16
						+ tempimg.at<double>(ii + 1, jj) * 32
						+ tempimg.at<double>(ii - 1, jj + 1) * 64
						+ tempimg.at<double>(ii, jj + 1) * 128
						+ tempimg.at<double>(ii + 1, jj + 1) * 256;

				new_image.at<double>(j, i) = lut.at<double>(0, index);

			}
	
		}
	}

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

