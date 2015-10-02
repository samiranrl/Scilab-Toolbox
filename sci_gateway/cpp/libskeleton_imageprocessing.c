#ifdef __cplusplus
extern "C" {
#endif
#include <mex.h> 
#include <sci_gateway.h>
#include <api_scilab.h>
#include <MALLOC.h>
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc opencv_imread;
extern Gatefunc opencv_blur;
extern Gatefunc opencv_canny;
extern Gatefunc opencv_circle;
extern Gatefunc opencv_clipline;
extern Gatefunc opencv_convexhull;
extern Gatefunc opencv_cornerEigenValsAndVecs;
extern Gatefunc opencv_cornerHarris;
extern Gatefunc opencv_cornerMinEigenVal;
extern Gatefunc opencv_dilate;
extern Gatefunc opencv_ellipse2poly;
extern Gatefunc opencv_erode;
extern Gatefunc opencv_fillconvexpoly;
extern Gatefunc opencv_filter2D;
extern Gatefunc opencv_findContours;
extern Gatefunc opencv_gaussianblur;
extern Gatefunc opencv_getStructuringElement;
extern Gatefunc opencv_getTextSize;
extern Gatefunc opencv_goodfeaturestotrack;
extern Gatefunc opencv_houghcircles;
extern Gatefunc opencv_houghlines;
extern Gatefunc opencv_im2double;
extern Gatefunc opencv_imcontrast;
extern Gatefunc opencv_imcrop;
extern Gatefunc opencv_imresize;
extern Gatefunc opencv_imsharpen;
extern Gatefunc opencv_imwrite;
extern Gatefunc opencv_laplacian;
extern Gatefunc opencv_line;
extern Gatefunc opencv_medianblur;
extern Gatefunc opencv_puttext;
extern Gatefunc opencv_scharr;
extern Gatefunc opencv_sobel;
extern Gatefunc opencv_threshold;
extern Gatefunc opencv_undistort;
extern Gatefunc opencv_boundingRect;
extern Gatefunc opencv_ellipse;
extern Gatefunc opencv_montage;
extern Gatefunc opencv_morphologyEx;
extern Gatefunc opencv_pyrDown;
extern Gatefunc opencv_pyrUp;
extern Gatefunc opencv_rectangle;
extern Gatefunc opencv_sepFilter2D;
extern Gatefunc opencv_copymakeborder;
extern Gatefunc opencv_cvtColor;
extern Gatefunc opencv_getgaussiankernel;
extern Gatefunc opencv_getrectsubpix;
extern Gatefunc opencv_getrotationmatrix2D;
extern Gatefunc opencv_houghlinesp;
extern Gatefunc opencv_gray2ind;
extern Gatefunc opencv_imfuse;
extern Gatefunc opencv_imshowpair;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imread,"imread"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_blur,"blur"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_canny,"canny"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_circle,"circle"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_clipline,"clipline"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_convexhull,"convexhull"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_cornerEigenValsAndVecs,"cornerEigenValsAndVecs"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_cornerHarris,"cornerHarris"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_cornerMinEigenVal,"cornerMinEigenVal"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_dilate,"dilate"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_ellipse2poly,"ellipse2poly"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_erode,"erode"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_fillconvexpoly,"fillconvexpoly"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_filter2D,"filter2D"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_findContours,"findContours"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_gaussianblur,"gaussianblur"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_getStructuringElement,"getStructuringElement"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_getTextSize,"getTextSize"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_goodfeaturestotrack,"goodfeaturestotrack"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_houghcircles,"houghcircles"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_houghlines,"houghlines"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_im2double,"im2double"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imcontrast,"imcontrast"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imcrop,"imcrop"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imresize,"imresize"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imsharpen,"imsharpen"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imwrite,"imwrite"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_laplacian,"laplacian"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_line,"line"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_medianblur,"medianblur"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_puttext,"puttext"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_scharr,"scharr"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_sobel,"sobel"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_threshold,"threshold"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_undistort,"undistort"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_boundingRect,"boundingRect"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_ellipse,"ellipse"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_montage,"montage"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_morphologyEx,"morphologyEx"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_pyrDown,"pyrDown"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_pyrUp,"pyrUp"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_rectangle,"rectangle"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_sepFilter2D,"sepFilter2D"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_copymakeborder,"copymakeborder"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_cvtColor,"cvtColor"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_getgaussiankernel,"getgaussiankernel"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_getrectsubpix,"getrectsubpix"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_getrotationmatrix2D,"getrotationmatrix2D"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_houghlinesp,"houghlinesp"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_gray2ind,"gray2ind"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imfuse,"imfuse"},
  {(Myinterfun)sci_gateway_without_putlhsvar,opencv_imshowpair,"imshowpair"},
};
 
int C2F(libskeleton_imageprocessing)()
{
  Rhs = Max(0, Rhs);
  if (*(Tab[Fin-1].f) != NULL) 
  {
     if(pvApiCtx == NULL)
     {
       pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
     }
     pvApiCtx->pstName = (char*)Tab[Fin-1].name;
    (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  }
  return 0;
}
#ifdef __cplusplus
}
#endif
