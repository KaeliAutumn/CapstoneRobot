#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <unistd.h>
#include <ctime>

using namespace cv;
using namespace std;

  int thresh=195;
  int max_thresh = 255;
  RNG rng(12345);



Mat findBiggestBlob(Mat &src){
int largest_area=0;
int largest_contour_index=0;
Mat temp(src.rows,src.cols,CV_8UC1);
Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));
src.copyTo(temp);

vector<vector<Point> > contours; // storing contour
vector<Vec4i> hierarchy;

findContours( temp, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

for( int i = 0; i< contours.size(); i++ ) // iterate
{
    double a=contourArea( contours[i],false);  //Find the largest area of contour
    if(a>largest_area)
    {
        largest_area=a;
        largest_contour_index=i;
    }
}

drawContours( dst, contours,largest_contour_index, Scalar(255,0,0), CV_FILLED, 8, hierarchy );
// Draw the largest contour
return dst;
}

Point computeCentroid(const cv::Mat &mask) {
    Moments m = moments(mask, true);
    Point center(m.m10/m.m00, m.m01/m.m00);
    return center;
}




 int main( int argc, char** argv )
 {
  namedWindow("Thresholded Image ON", WINDOW_AUTOSIZE);
  namedWindow("Thresholded Image OFF", WINDOW_AUTOSIZE);
  namedWindow("Original ON", WINDOW_AUTOSIZE);
  namedWindow("Original OFF", WINDOW_AUTOSIZE);
  namedWindow("Diff", WINDOW_AUTOSIZE);

    VideoCapture cap(1); //capture the video from web cam
    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return -1;
    }

  int iLowH = 76;
 int iHighH = 97;

  int iLowS = 0;
 int iHighS = 37;

  int iLowV = 224;
 int iHighV = 255;


    while (true)
    {


namedWindow("Control");
  //Create trackbars in "Control" window
 cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cvCreateTrackbar("HighH", "Control", &iHighH, 179);

  cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cvCreateTrackbar("HighS", "Control", &iHighS, 255);

  cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
 cvCreateTrackbar("HighV", "Control", &iHighV, 255);


        Mat imgOriginalON, imgOriginalOFF;

//while(true){
//cout<<"Taking Picture "<<endl;
//cap>>imgOriginalON;

//waitKey(500);
//cap>>imgOriginalOFF;
  //imshow("Original ON", imgOriginalON); //show the original image
  //imshow("Original OFF", imgOriginalOFF); //show the original image
//waitKey(2000);
//}

cout<<"Taking On"<<endl;
cap>>imgOriginalON;
cout<<"Taking OFF in 5 s"<<endl;

clock_t start;
double duration=0;
start = std::clock();
while(duration<5){
    cap>>imgOriginalOFF;
    duration = (clock() - start ) / (double) CLOCKS_PER_SEC;
}


cout<<"Taking OFF"<<endl;
cap>>imgOriginalOFF;
	//bSuccess = cap.read(imgOriginalOFF);
//cout<<"Taking On in 5.2 s"<<endl;
//waitKey(5000);


    Mat imgHSVON,imgHSVOFF;
 //imgOriginalON = imread("./beaconON.jpg");
 //imgOriginalOFF = imread("./beaconOFF.jpg");
 if(imgOriginalON.empty() || imgOriginalOFF.empty())
 {
     cout << "can not open " << endl;
     return -1;
 }


   cvtColor(imgOriginalON, imgHSVON, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
   cvtColor(imgOriginalOFF, imgHSVOFF, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

  Mat imgThresholdedON, imgThresholdedOFF;

  inRange(imgHSVON, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholdedON); //Threshold the image
  inRange(imgHSVOFF, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholdedOFF); //Threshold the image

  // findGrass(imgOriginal,imgHSV);

  //  while (true)
  //  {
  // ellipse(imgOriginal);
  // cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

  // inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

  //morphological opening (remove small objects from the foreground)
  erode(imgThresholdedON, imgThresholdedON, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholdedON, imgThresholdedON, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(imgThresholdedON, imgThresholdedON, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

  erode(imgThresholdedOFF, imgThresholdedOFF, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholdedOFF, imgThresholdedOFF, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(imgThresholdedOFF, imgThresholdedOFF, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

//Mat bigBlob=findBiggestBlob(imgThresholded);
//line(bigBlob,threshold,minlength,maxgap);


//Point center=computeCentroid(bigBlob);
//Mat blobtest=bigBlob;



  imshow("Thresholded Image ON", imgThresholdedON); //show the thresholded image
  imshow("Thresholded Image OFF", imgThresholdedOFF); //show the thresholded image

  //imshow("blob Image", blobtest);
  imshow("Original ON", imgOriginalON); //show the original image
  imshow("Original OFF", imgOriginalOFF); //show the original image

  imshow("Diff", imgThresholdedON-imgThresholdedOFF);
 //       if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
 //      {
 //           cout << "esc key is pressed by user" << endl;
            //break;
 //      }
 //   }
	waitKey(-1);
}
   return 0;

}