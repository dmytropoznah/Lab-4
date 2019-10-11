        #include <opencv2/core/core.hpp>
        #include <opencv2/highgui/highgui.hpp>
        #include <opencv2/imgproc/imgproc.hpp>
        #include "opencv2/calib3d/calib3d.hpp"
        #include <iostream>

    using namespace cv;
            using namespace std;

            Mat src_gray;
            Mat dst;

            int threshold_value = 100; 

            int main(int argc, char** argv) {

           
            const int minRectDim = 50;
            const int maxRectDim = 150;

            Mat src;
            src = imread("C:/Medals.jpg", IMREAD_COLOR);

            imshow("original", src);

            cvtColor(src, src_gray, COLOR_BGR2GRAY); 
            threshold(src_gray, dst, threshold_value, 255, 0); 

            Mat dst2;
            src.copyTo(dst2);
            
            for (int y = 0; y<dst.rows; y++) {

        for (int x = 0; x<dst.cols; x++) {

        int value = dst.at<uchar>(y, x);

        if (value == 255) { 
        Rect rect;
        int count = floodFill(dst, Point(x, y), Scalar(200), &rect);
        
        if (rect.width >= minRectDim && rect.width <= maxRectDim && rect.height >= minRectDim && rect.height <= maxRectDim)
        {
        int x = rect.x + rect.width / 2;
        int y = rect.y + rect.height / 2;
        int rad = (rect.width + rect.height) / 4;
        circle(dst2, Point(x, y), rad, Scalar(255, 0, 0), 2);
        }
        }
        }
        }

        imshow("detected", dst2);

        Mat image; 
        src_gray.copyTo(image);

        int rows = image.rows;
        int cols = image.cols;
        int size = rows * cols;

        Mat skel = Mat::zeros(image.rows, image.cols, CV_8U); 
        threshold(image, image, 100, 255, THRESH_BINARY);
        Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));

        bool done = false;
        while (!done) { 
        Mat eroded;
        erode(image, eroded, element);
        Mat temp;
        dilate(eroded, temp, element); 
        subtract(image, temp, temp);
        bitwise_or(skel, temp, skel);
        eroded.copyTo(image);

        int zeros = size - countNonZero(image); 
        if (zeros == size) { 
        done = true;
        }
        }

        imshow("skel", skel); 

        waitKey(0);
        return 0;
        }
