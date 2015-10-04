/*
 * imboxfilt
 *        
 * k_size*k_size box filter convolution in scilab
 *     
 */

// Created by Samiran Roy, mail: samiranroy@cse.iitb.ac.in
// An implementation of imboxfilt method in matlab
// Usage: 
// imboxfilt(I) : Convolve image I with 3*3 box kernel
// imboxfilt(I,k_size) : Convolve image I with k_size*k_size box kernel
// imboxfilt(I,ksize,name,value)
// imboxfilt(I,ksize,name,value,name,value)

// name - value pair details:

// name : 'NormalizationFactor'
// value: any scalar: this is multiplied to the image after convolving with kernel = ones(k_size,k_size)
// example: imboxfilt(I,3,'NormalizationFactor',1) takes area sums over a 3*3 neighborhood

// name : 'padding' : how to handle the borders
// value: 'replicate' : take the value of the border pixel to be the closest image border pixel
// value: 'symmetric' : take the border values to be a reflection of the image
// value: 'circular' : take the border values assuming that the image is periodic
// value: 'constant' : pad the image with the provided scalar double

// Known Changes from Matlab:
/*
 * 1) No infinite name-value pairs, limited it to two pairs - or 6 arguments in total
 * 2) Kernel size must be input manually when using name-value pairs
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
	int iRet = 0;
	int *piAddr2 = NULL;
	int *piAddr3 = NULL;
	int *piAddr4 = NULL;
	int *piAddr5 = NULL;
	int *piAddr6 = NULL;

// Setting default border handling to to replicate
	int borderType = BORDER_REPLICATE;
	double borderValue = BORDER_REPLICATE;

// Error flags - These are set to 1 if i need to handle the situation manually (it means: OpenCV does not have this explicit implementation)
	int handle_constant_border = 0;
	int handle_circular_border = 0;

// The type of padding - entered by user
	char* padding;

// Holds the names of name-value pairs entered by user
	char* Name1 = NULL;
	char* Name2 = NULL;

// The kernel used for convolution - initilalized later according to input parameters
	Mat kernel;

// Default kernel size, the result is normalized according to this kernel
	double k_size = 3;
	double NormalizationFactor = 1 / ((k_size) * (k_size));

// Parameters for convolution - Leave these alone

	/* ********************************************************************************** */

	Point anchor;
	double delta;
	int ddepth;

	anchor = Point(-1, -1);  // The center is unchanged
	delta = 0; // No value is added to output
	ddepth = -1; // The dimentions of input and output images are the same

	/* ********************************************************************************** */

	//checking input argument
	CheckInputArgument(pvApiCtx, 1, 6);
	CheckOutputArgument(pvApiCtx, 1, 1);

	// Get the number of input arguments
	int inputarg = *getNbInputArgument(pvApiCtx);

	//get input image
	Mat image;
	retrieveImage(image, 1);

	if (inputarg >= 2) {
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddr2);
		if (sciErr.iErr) {
			printError(&sciErr, 0);
			return 0;
		}
// Get size of box filter
		intErr = getScalarDouble(pvApiCtx, piAddr2, &k_size);
		if (sciErr.iErr) {
			return intErr;
		}

		if (k_size <= 0 || static_cast<int>(k_size) % 2 == 0) {
			sciprint(
					"Invalid size of filter - should be an odd, positive integer\n");
			return 0;
		}
// Update normalization factor according to the new size
		NormalizationFactor = 1 / ((k_size) * (k_size));

	}

	if (inputarg == 3)

	{

		sciprint("Invalid number of arguments\n");
		return 0;

	}

	if (inputarg >= 4)

	{

		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddr3);

		if (sciErr.iErr) {
			printError(&sciErr, 0);
			return 0;
		}

		if (isStringType(pvApiCtx, piAddr3)) {
			if (isScalar(pvApiCtx, piAddr3)) {

				iRet = getAllocatedSingleString(pvApiCtx, piAddr3, &Name1);
			}
		}
// Checks the first name in the name - value pairs
		if (strcmp(Name1, "NormalizationFactor") == 0) {

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

		else if (strcmp(Name1, "padding") == 0) {

			sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddr4);

			if (sciErr.iErr) {
				printError(&sciErr, 0);
				return 0;
			}

			if (isStringType(pvApiCtx, piAddr4)) {
				if (isScalar(pvApiCtx, piAddr4)) {

					iRet = getAllocatedSingleString(pvApiCtx, piAddr4,
							&padding);
				}

				if (strcmp(padding, "replicate") == 0) {
					borderType = BORDER_REPLICATE;

				}

				else if (strcmp(padding, "circular") == 0) {
// I have to handle this case manually since opencv does not allow convolution with this border padding type
					handle_circular_border = 1;

				}

				else if (strcmp(padding, "symmetric") == 0) {
					borderType = BORDER_REFLECT;

				}

				else {

					sciprint(
							"Invalid border argument: Accepts 'replicate', 'symmetric', 'circular' or a scalar double\n");
					return 0;

				}

			} else
// User enters a constant scalar double as border padding type 

			{

				intErr = getScalarDouble(pvApiCtx, piAddr4, &borderValue);
				if (sciErr.iErr) {

					printError(&sciErr, 0);
					return intErr;
				}
// I have to handle this case manually since opencv does not allow convolution with this border padding type
				handle_constant_border = 1;

			}

		} else {

// User entered an incorrect argument for the name in the name - value pair
			sciprint("Unknown Parameter Name:%s\n", Name1);
			return 0;
		}

	}

	if (inputarg == 5)

	{

		sciprint("Invalid number of arguments\n");
		sciprint("Follows the format:\n");
		return 0;

	}

	if (inputarg == 6)

	{

		sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddr5);

		if (sciErr.iErr) {
			printError(&sciErr, 0);
			return 0;
		}

		if (isStringType(pvApiCtx, piAddr5)) {
			if (isScalar(pvApiCtx, piAddr5)) {

				iRet = getAllocatedSingleString(pvApiCtx, piAddr5, &Name2);
			}
		}

// Checks the second name in the name - value pairs
		if (strcmp(Name2, "NormalizationFactor") == 0) {

			sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);
			if (sciErr.iErr) {
				printError(&sciErr, 0);
				return 0;
			}
// Get the Normalization Factor from user
			intErr = getScalarDouble(pvApiCtx, piAddr6, &NormalizationFactor);
			if (sciErr.iErr) {
				return intErr;
			}

		}

		else if (strcmp(Name2, "padding") == 0) {

			sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddr6);

			if (sciErr.iErr) {
				printError(&sciErr, 0);
				return 0;
			}

			if (isStringType(pvApiCtx, piAddr6)) {
				if (isScalar(pvApiCtx, piAddr6)) {

					iRet = getAllocatedSingleString(pvApiCtx, piAddr6,
							&padding);
				}

				if (strcmp(padding, "replicate") == 0) {
					borderType = BORDER_REPLICATE;

				}

				else if (strcmp(padding, "circular") == 0) {
// I have to handle this case manually since opencv does not allow convolution with this border padding type
					handle_circular_border = 2;

				}

				else if (strcmp(padding, "symmetric") == 0) {
					borderType = BORDER_REFLECT;

				}

				else {

					sciprint(
							"Invalid border argument: Accepts 'replicate', 'symmetric', 'circular' or a scalar double\n");
					return 0;

				}

			} else
// User enters a constant scalar double as border padding type 
			{

				intErr = getScalarDouble(pvApiCtx, piAddr6, &borderValue);
				if (sciErr.iErr) {

					printError(&sciErr, 0);
					return intErr;
				}

				handle_constant_border = 2;
// I have to handle this case manually since opencv does not allow convolution with this border padding type
			}

		}

		else {

// User entered an incorrect argument for the name in the name - value pair
			sciprint("Unknown Parameter Name:%s\n", Name2);
			return 0;

		}

	}

//initializing the box filter kernel used for convolution

	kernel = Mat::ones(k_size, k_size, CV_32F) * (NormalizationFactor);

	Mat new_image;
	Mat tempimg;

// These are my implementations for flagged cases - circular border padding and constant border padding
// I manually pad the image, perform convolution and extract the required area
// There are four if condition to handle to give priority to the second name - value pair
// eg: imboxfilt(list(eye(3,3)),3,'padding',1,'padding',0) will pad the border with zeroes

	/* ************************************************************************************************ */

	int pad_size = static_cast<int>(k_size / 2);

	if (handle_circular_border == 1) {

		copyMakeBorder(image, tempimg, pad_size, pad_size, pad_size, pad_size,
				BORDER_WRAP);

		filter2D(tempimg, tempimg, ddepth, kernel, anchor, delta,
				BORDER_CONSTANT);

		new_image = tempimg(Range(pad_size, tempimg.rows - pad_size),
				Range(pad_size, tempimg.cols - pad_size));

	} else if (handle_constant_border == 1)

	{

		copyMakeBorder(image, tempimg, pad_size, pad_size, pad_size, pad_size,
				BORDER_CONSTANT, borderValue);

		filter2D(tempimg, tempimg, ddepth, kernel, anchor, delta, borderType);

		new_image = tempimg(Range(pad_size, tempimg.rows - pad_size),
				Range(pad_size, tempimg.cols - pad_size));

	}
	if (handle_circular_border == 2) {

		copyMakeBorder(image, tempimg, pad_size, pad_size, pad_size, pad_size,
				BORDER_WRAP);

		filter2D(tempimg, tempimg, ddepth, kernel, anchor, delta,
				BORDER_CONSTANT);

		new_image = tempimg(Range(pad_size, tempimg.rows - pad_size),
				Range(pad_size, tempimg.cols - pad_size));

	}

	else if (handle_constant_border == 2)

	{

		copyMakeBorder(image, tempimg, pad_size, pad_size, pad_size, pad_size,
				BORDER_CONSTANT, borderValue);

		filter2D(tempimg, tempimg, ddepth, kernel, anchor, delta, borderType);

		new_image = tempimg(Range(pad_size, tempimg.rows - pad_size),
				Range(pad_size, tempimg.cols - pad_size));

	}

	/* ************************************************************************************************ */

	else

	{

//performing convolution
		filter2D(image, new_image, ddepth, kernel, anchor, delta, borderType);
	}

// new_image is sent to scilab as output
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

