#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int take_image(void)
{
    int i = 0;
    while(i<10){
        Mat frame;
        VideoCapture cap;
        // open the default camera, use something different from 0 otherwise;
        // Check VideoCapture documentation.
        if(!cap.open(0))
            return 0;
        for(;;)
        {
            cap >> frame;
            if( frame.empty() ) break; // end of video stream
            imshow("camera calibration", frame);
            if( waitKey(1) == 27 ) break; // stop capturing by pressing ESC
        }
        
        if(!frame.data )
        {
            printf( " No image data \n " );
            return -1;
        }
        std::string str = "../../include/images/img";
        str += std::to_string(i);
        str += ".jpg";
        
        imwrite(str,frame);
        i++;
        waitKey(0);
    }
    
    return 0;

}