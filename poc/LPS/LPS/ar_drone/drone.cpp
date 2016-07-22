#include "move.h"
#include "drone.h"
// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// Thanks to puku0x @ github
// --------------------------------------------------------------------------
using namespace std;

ARDrone ardrone;
bool drone_initialised;

int fly(){
    int tick= 0;
    land = true;
    cout << "Initializing drone" << endl;
    // Initialize
    if (!ardrone.open()) {
        std::cout << "Failed to initialize." << std::endl;
        return -1;
    }
    
    drone_initialised = true;
    bool front_camera = true;
    // Battery
    std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;

    // Outdoor mode
    ardrone.setOutdoorMode(true);
    //init_detect(ardrone.getImage());
    while (1) {
        // Key input
        int key = cv::waitKey(33);
        if (key == 0x1b) break;

        // Get an image
        cv::Mat image = ardrone.getImage();
        
        // detect marker
        //ar_detect(image);
        
        // Take off
//        if (ardrone.onGround()) ardrone.takeoff();

        // Change camera
        static int mode = 0;
        if (key == 'c' || front_camera){
            ardrone.setCamera(++mode % 4);
            front_camera = false;
        }

        // Display the image
        cv::imshow("camera", image);
        
//        ardrone.move3D(vx(), vy(), vz(), vr());
        
//        if(tick==50) land=true;
        
//        if(check()){
//            ardrone.landing();
//        }
        
        tick++;
        
        // optional altitude check
        //cout << "Altitude: " << ardrone.getAltitude() << endl;
    }

    // See you
    ardrone.close();

    return 0;
}

//for if the ar_drone camera stream is wanted, but the drone hasn't been initialized.
bool init_stream(int camera_channel){
    if(!drone_initialised){
        cout << "Initializing drone..." << endl;
        if (!ardrone.open()) {
            std::cout << "Failed to initialize." << std::endl;
            cv::Mat image;
            return false;
        }
        cout << "Drone up and running!" << endl;
        drone_initialised = true;
        ardrone.setCamera(camera_channel%2);
    }
    return true;
}







