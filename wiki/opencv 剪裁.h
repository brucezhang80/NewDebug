第一步：将必需剪切的图像图像不局部设置为ROI

cvSetImageROI(src , cvRect(x,y,width,height));

第二步：修建一个与必需剪切的Lumi图像局部同样大小的新图像

cvCreateImage(cvSize(width,height),IPL_DEPTH,nchannels);

第三步：将源图像复制到修建的图像中

cvCopy(src,dst,0);

第四步：释放ROI区域

cvResetIamgeROI(src);

全面过程实例：

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>
IplImage* src;IplImage* dst;
int main(int argc , char** argv )
{ 
	if(argv[1] == NULL) 
		return 0; 
	else 
	{
		src = cvLoadImage(argv[1],1);
		if(!src)
			fatal_error("unable to load image from %s", argv[1]);
		cvNamedWindow("源图像",1);
		cvShowImage("源图像",src);
		cvSetImageROI(src,cvRect(0,0,0.5*src->width,0.5*src->height));
		dst = cvCreateImage(cvSize(0.5*src->width,0.5*src->height),IPL_DEPTH_8U,src->nChannels);
		cvCopy(src,dst,0);
		cvResetImageROI(src);
		cvNamedWindow("垄断后的图像",1);
		cvShowImage("垄断后的图像",dst);cvDestroyWindow("垄断后的图像");
		cvDestroyWindow("源图像"); 
	} 
	cvReleaseImage(&src); 
	cvReleaseImage(&dst); return 0;
}