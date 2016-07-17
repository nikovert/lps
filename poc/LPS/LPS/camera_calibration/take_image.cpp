#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>
#include "drone.h"

using namespace cv;
using namespace std;

class Image_List{
public:
    Image_List(){}
public:   // Data Members
    vector<string> list;
    
    void write(FileStorage& fs) const                        //Write serialization for this class
    {
        fs << "{" << "A" << list << "}";
    }
    
    void add(string str){
        list.push_back(str);
    }
};

void write(FileStorage& fs, const std::string&, const Image_List& x)
{
    x.write(fs);
}

void delete_image(string str) {
    ifstream fin(str);
    if (fin)
    {
        remove(str.c_str());
        fin.close();
    }
    return;
}


int webcam(){
    int i = 0;
    while(i<50){
        Mat frame;
        VideoCapture cap;
        // open the default camera, use something different from 0 otherwise;
        // Check VideoCapture documentation.
        if(!cap.open(0))
            return 0;
        for(;;)
        {
            cap >> frame;
            if( frame.empty() ){
                cout << "ERROR: frame empty!" << endl;
                break; // end of video stream
            }
            imshow("camera calibration", frame);
            if( waitKey(10) == 49 ) break; // stop capturing by pressing ESC
        }
        
        if(!frame.data )
        {
            printf( " No image data \n " );
            return -1;
        }
        string str = "../../LPS/include/images/img";
        str += to_string(i);
        str += ".jpg";
        delete_image(str);
        imwrite(str,frame);
        i++;
        waitKey(1);
    }
    return 0;
}

int ar_drone(){
    FileStorage fs("../../LPS/include/include_img.xml", FileStorage::WRITE );
    Image_List imglist;
    vector<string> list;
    int i = 0;
    if(!init_stream(1)) return -1; //initializing bottom camera
    while(i<10){
        Mat frame;
        frame = ardrone.getImage();
//        for(;;){
//            frame = ardrone.getImage();
//            if( frame.empty() ) break; // end of video stream
//            imshow("camera calibration", frame);
//            if( waitKey(10) == 49 ) break; // stop capturing by pressing ESC
//        }
        
        if(!frame.data )
        {
            printf( " No image data \n " );
            return -1;
        }
        
        string str = "../../LPS/include/images/img";
        str += to_string(i);
        str += ".jpg";
        delete_image(str);
        list.push_back(str);
        imwrite(str,frame);
        i++;
        waitKey(1000);
    }
    fs << "images" << list;
    fs.release();
    cout << "finished taking pictures";
    ardrone.close();
    return 0;
}

int take_image(int x){
    if(x==0)
        return webcam();
    else
        return ar_drone();
}
