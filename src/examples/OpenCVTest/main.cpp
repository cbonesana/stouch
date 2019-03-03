#include <QCoreApplication>

#include <cstdio>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    IplImage *img = cvLoadImage("./test.png");
    IplImage *tmp;

    printf("Orig dimensions: %dx%d\n", img->width, img->height);
    cvSetImageROI(img, cvRect(100, 100, 500, 500));

    tmp = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

    cvCopy(img, tmp, NULL);

    cvResetImageROI(img);

    img = cvCloneImage(tmp);

    printf("Orig dimensions after crop: %dx%d\n", tmp->width, tmp->height);

    cvSaveImage("cropped.png", img);

    return 0;
}
