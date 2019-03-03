#include <QCoreApplication>

#include <cstdio>
#include <cstring>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 5){
        printf("No enough parameters");
        return -1;
    }

    char *inFilename = argv[1];
    char *outFilename = argv[2];
    int x1 = atoi(argv[3]);
    int y1 = atoi(argv[4]);
    int x2 = atoi(argv[5]);
    int y2 = atoi(argv[6]);

    IplImage *img = cvLoadImage(inFilename);
    IplImage *tmp;

    if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0){
        // don't crop
        return 0;
    }

    cvSetImageROI(img, cvRect(x1, y1, (x2-x1), (y2-y1)));

    tmp = cvCreateImage(cvGetSize(img), img->depth, img->nChannels);

    cvCopy(img, tmp, NULL);

    cvResetImageROI(img);

    img = cvCloneImage(tmp);

    cvSaveImage(outFilename, img);

    return 0;
}
