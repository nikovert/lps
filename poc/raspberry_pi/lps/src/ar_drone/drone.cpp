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
bool altitude;
int tick;

bool landing(){
    ardrone.landing();
    // See you
    ardrone.close();
}

//Initialisizes Drone, first time only
void initialize_drone(){
    tick= 0;
    land = false;
    cout << "Initializing drone" << endl;
    // Initialize
    if (!ardrone.open()) {
        std::cout << "Failed to initialize." << std::endl;
        return;
    }
    
    drone_initialised = true;
    bool front_camera = true;
    
    // Battery
    std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;

    // Outdoor mode
    ardrone.setOutdoorMode(true);
    //init_detect(ardrone.getImage());
    ardrone.takeoff();

    return;
}

//main fly function
void fly(){
    if(!drone_initialised)
        initialize_drone();
    
    // Get an image
    //cv::Mat image = ardrone.getImage();
    
    // detect marker
    detect();
    
    // Take off
    if (ardrone.onGround()) ardrone.takeoff();
        
    ardrone.move3D(vx(), vy(), vz(), vr());
        
    if(check()){
        if(!landing()) cout << "error landing";
    }
    // optional altitude check
    if(altitude) cout << "Altitude: " << ardrone.getAltitude() << endl;
            
    tick++;
}

//int fly(){
//    
//    //    // read from camera or from  file
//    //    raspicam::RaspiCam_Cv Camera; //Camera object
//    //
//    //    //set camera params
//    //    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
//    //
//    //    //Open camera
//    //    cout<<"Opening Camera..."<<endl;
//    //
//    //    // check video is open
//    //    if ( !Camera.open()) {cerr<<"Error opening camera"<<endl;return -1;}
//    //
//    int tick= 0;
//    land = false;
//    cout << "Initializing drone" << endl;
//    // Initialize
//    if (!ardrone.open()) {
//        std::cout << "Failed to initialize." << std::endl;
//        return -1;
//    }
//    
//    drone_initialised = true;
//    bool front_camera = true;
//    
//    // Battery
//    std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;
//    
//    // Outdoor mode
//    ardrone.setOutdoorMode(true);
//    //init_detect(ardrone.getImage());
//    ardrone.takeoff();
//    
//    while (tick<2000) {
//        // Key input
//        int key = cv::waitKey(33);
//        if (key == 0x1b) break;
//        
//        // Get an image
//        //cv::Mat image = ardrone.getImage();
//        
//        // detect marker
//        detect();
//        
//        // Take off
//        if (ardrone.onGround()) ardrone.takeoff();
//        
//        ardrone.move3D(vx(), vy(), vz(), vr());
//        
//        if(check()){
//            ardrone.landing();
//        }
//        if(tick == 100) move_up(100);
//        if(tick == 200) move_down(100);
//        // optional altitude check
//        //if(tick%50 == 0) cout << "Altitude: " << ardrone.getAltitude() << endl;
//        
//        tick++;
//    }
//    
//    if (!ardrone.onGround()){
//        cout << "landing" << endl;
//        ardrone.landing();
//    }
//    
//    // See you
//    ardrone.close();
//    
//    return 0;
//}

//for if the ar_drone camera stream is wanted, but the drone hasn't been initialized.
//can be deleted
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







