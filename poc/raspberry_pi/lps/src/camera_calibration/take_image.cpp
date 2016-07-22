#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <raspicam/raspicam_cv.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace cv;
using namespace std;

//takes images, used for camera calibration
int take_image(void){
    FileStorage fs("../src/include/include_img.xml", FileStorage::WRITE );
    vector<string> list;
    
    // read from camera or from  file
    raspicam::RaspiCam_Cv Camera; //Camera object
    
    //set camera params
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
    
    //Open camera
    cout<<"Opening Camera..."<<endl;
    
    // check video is open
    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}

    int i = 0;
    while(i<200){
        Mat frame;
        // Check VideoCapture documentation.
        Camera.grab();
        Camera.retrieve(frame);
        if( frame.empty() ) break; // end of video stream
        waitKey(500);
        if(!frame.data ){
            printf( " No image data \n " );
            return -1;
        }
        string str = "../src/include/images/img";
        stringstream ss;
        ss << i;
        str += ss.str();
        str += ".jpg";
        cout << "taking picture number " << i << " and writing to "<< str << endl;
        imwrite(str,frame);
        list.push_back(str);
        i++;
    }
    fs << "images" << list;
    fs.release();
    cout << "finished taking pictures";
    return 0;

}