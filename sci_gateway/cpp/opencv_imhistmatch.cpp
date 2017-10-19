/*
 * imhistmatch
 *
 * histogram matching in scilab
 *
 */

// Shreyash Sharma
// An implementation of imhistmatch method
// Usage:
// 1) imhistmatch(image,referenceimage)
// 2) imhistmatch(image,referenceimage,outputbins)

// Known Changes from Matlab:
/*
 * Default number of bins is 256 - for both input and output
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

Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);

    int channels = I.channels();

    int nRows = I.rows;
    int nCols = I.cols * channels;

    if (I.isContinuous())
    {
        nCols *= nRows;
        nRows = 1;
    }

    int i,j;
    uchar* p;
    for( i = 0; i < nRows; ++i)
    {
        p = I.ptr<uchar>(i);
        for ( j = 0; j < nCols; ++j)
        {
            p[j] = table[p[j]];
        }
    }
    return I;
}

Mat cumsum (Mat & src) 
{
  Mat result = Mat::zeros(Size(src.cols,src.rows),CV_32FC1);      
  for (int i = 0; i < src.rows; ++i) 
  {
     for (int j = 0; j < src.cols; ++j)
     {
        if (i == 0)
        {
            result.at<float>(i, j) = src.at<float>(i, j);
         }
         else
         {
             result.at<float>(i, j) = src.at<float>(i, j) + result.at<float>(i-1, j);
         }               

       }

    }

return result;
}



int opencv_imhistmatch(char *fname, unsigned long fname_len) {
  SciErr sciErr;
  int intErr = 0;

  int *piAddr = NULL;
  int *piAddrNew = NULL;
  int *piAddr2 = NULL;
  int *piAddr3 = NULL;

  double num_bins = 256;  // default number of bins for histogram calculation

  // checking input argument
  CheckInputArgument(pvApiCtx, 2, 3);
  CheckOutputArgument(pvApiCtx, 1, 1);

  // Get the number of input arguments
  int inputarg = *getNbInputArgument(pvApiCtx);

  // get input matrix

  Mat image, new_image, r, g, b;
  retrieveImage(image, 1);
  image.convertTo(image,CV_8UC1);

  Mat ref;
  retrieveImage(ref, 2);
  ref.convertTo(ref,CV_8UC1);
  int case1 = 0;
  if (image.channels() == 1)

  {
    if (ref.channels() != 1) {
      sciprint(
          "If A is a grayscale image, the ref image must also be grayscale\n");
      return 0;

    } else {
      case1 = 1;
    }
  }

  if (inputarg == 3) {
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

    if (num_bins < 1) {
      sciprint("Invalid number of histogram bins\n");
      return 0;
    }
  }

  int bins = (int)num_bins;
  
    Mat Result;
    int L = 256;

  try
  {
  if (case1 == 1) 
  {
    
    //new_image = match(image, ref, bins);
    
    Mat G,S,F; //G is the reference CDF, S the CDF of the equlized given image, F is the map from S->G
    
   if(ref.cols>1)
    
   {
    Mat R_hist, Rp_hist; //R_hist the counts of pixels for each level, Rp_hist is the PDF of each level
    
    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true; 
    bool accumulate = false;  
    //sciprint("Hello");
    calcHist( &ref, 1, 0, Mat(), R_hist, 1, &L, &histRange, uniform, accumulate );
    //sciprint("Hello1");
    //Calc PDF of the image
    Rp_hist=R_hist/(image.rows*image.cols);
    
    //calc G=(L-1)*CDF(p)
    Mat CDF=cumsum(Rp_hist);
    
    G=(L-1)*CDF;
    for(int i=0;i<G.rows;i++)
        G.at<Point2f>(i,0).x=(float)cvRound(G.at<Point2f>(i,0).x);//round G
   }
  else
   {
    //else, the given R is the reference PDF
    Mat CDF=cumsum(ref);
    
    G=(L-1)*CDF;
   
    for(int i=0;i<G.rows;i++)
        G.at<Point2f>(i,0).x=(float)cvRound(G.at<Point2f>(i,0).x);//round G
   }
  
  /// Establish the number of bins

   Mat S_hist, Sp_hist; //S_hist the counts of pixels for each level, Sp_hist is the PDF of each level

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float* histRange = { range };

  bool uniform = true; bool accumulate = false;

  //sciprint("Hello2");
  calcHist( &image, 1, 0, Mat(), S_hist, 1, &L, &histRange, uniform, accumulate );
  //sciprint("Hello1");
  //Calc PDF of the image
  Sp_hist=S_hist/(image.rows*image.cols);
 //calc s=(L-1)*CDF(p)
  Mat CDF=cumsum(Sp_hist);
  
  S=(L-1)*CDF;
   
   for(int i=0;i<S.rows;i++)
    
    S.at<Point2f>(i,0).x=(float)cvRound(S.at<Point2f>(i,0).x);//round S

   F=Mat::zeros(S.size(),CV_32F);
   
   int minIndex=-1;

   double T,min=100000;

  for(int i=0;i<S.rows;i++) 
  {
      for(int j=0;j<G.rows;j++)
      {
          T=abs(double(S.at<Point2f>(i,0).x)-double(G.at<Point2f>(j,0).x));
          if (T==0)
         {
             minIndex=j;
             break;
         }
         else
             if(T<min)
             {
                 minIndex=j;
                 min=T;
             }
       }
    
     F.at<Point2f>(i,0).x=(float)minIndex;
     minIndex=-1;
     min=1000000;
   }
     uchar table[256]; 
     for(int i=0;i<256;i++)
   {
      table[i]=(int)F.at<Point2f>(i,0).x;
   }
     Result= ScanImageAndReduceC(image, table);
  
   }
   
   }
    catch (cv::Exception & e) 
   {

         Scierror(999,e.what());
         return 0;

   }



  int temp = nbInputArgument(pvApiCtx) + 1;
  string tempstring = type2str(Result.type());
  char *checker;
  checker = (char *)malloc(tempstring.size() + 1);
  memcpy(checker, tempstring.c_str(), tempstring.size() + 1);
  returnImage(checker, Result, 1);
  free(checker);

  // Assigning the list as the Output Variable
  AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
  // Returning the Output Variables as arguments to the Scilab environment
  ReturnArguments(pvApiCtx);
  return 0;
}
/* ==================================================================== */
}
