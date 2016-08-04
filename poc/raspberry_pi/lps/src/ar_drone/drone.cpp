#include "move.h"
#include "drone.h"
#include <unistd.h> // for usleep function
// --------------------------------------------------------------------------
// Thanks to puku0x @ github for supplying the main implementation od the ardrone fuction
// --------------------------------------------------------------------------
using namespace std;

ARDrone ardrone;
bool drone_initialised;
bool altitude;
int tick;


// --------------------------------------------------------------------------
//! @brief   lands the drone and returns if this was successful
//! @param None
//! @return  if the drone has landed
// --------------------------------------------------------------------------
bool landing(){
    ardrone.landing();
    usleep(5000); // wait 5 seconds
    if(!ardrone.onGround())
        return false;
    // See you
    ardrone.close();
    return true;
}


// --------------------------------------------------------------------------
//! @brief   Initialisizes Drone, first time only
//! @param None
//! @return  None
// --------------------------------------------------------------------------
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
    
    // Battery
    std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;

    // Outdoor mode
    ardrone.setOutdoorMode(true);
    //init_detect(ardrone.getImage());
    ardrone.takeoff();

    return;
}

// --------------------------------------------------------------------------
//! @brief   main fly function
//! @param None
//! @return  None
// --------------------------------------------------------------------------
void fly(){
    if(!drone_initialised)
        initialize_drone();
    
    // Take off
    if (ardrone.onGround()) ardrone.takeoff();
    
    // detect marker and updates the drone_location
    detect();
    
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


// --------------------------------------------------------------------------
//! @brief returns the location of the drone according to the GPS
//! @param None
//! @return a 3D Point of the drones location
// --------------------------------------------------------------------------
cv::Point3d get_GPS_position(){
	double x,y,z;
	ardrone.getPosition(&x,&y,&z);
	return cv::Point3d(x,y,z);
}

// --------------------------------------------------------------------------
//! @brief returns the location of the drone according to the IMU
//! @param None
//! @return a 3D Point of the drones location
// --------------------------------------------------------------------------
cv::Point3d get_IMU_position(){
	double x,y,z;
	cerr << "this fuction stil needs to be implemented" << endl;
	//calculate position with IMU datas
		//by getting constant readings of the IMU velocity in the 6 directions of orientation,
		//we can calculate the new position of the drone.
	return cv::Point3d(x,y,z);
}









