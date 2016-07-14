#include "ardrone/ardrone.h"
#include "move.h"
// --------------------------------------------------------------------------
// main(Number of arguments, Argument values)
// Description  : This is the entry point of the program.
// Return value : SUCCESS:0  ERROR:-1
// Thanks to puku0x @ github
// --------------------------------------------------------------------------
using namespace std;

int fly()
{
    int tick= 0;
    land = false;
    
    // AR.Drone class
    ARDrone ardrone;

    // Initialize
    if (!ardrone.open()) {
        std::cout << "Failed to initialize." << std::endl;
        return -1;
    }

    // Battery
    std::cout << "Battery = " << ardrone.getBatteryPercentage() << "[%]" << std::endl;

    // Outdoor mode
    ardrone.setOutdoorMode(true);
    
    while (1) {
        // Key input
        int key = cv::waitKey(33);
        if (key == 0x1b) break;

        // Get an image
        cv::Mat image = ardrone.getImage();
        
        // Take off
        if (ardrone.onGround()) ardrone.takeoff();

        // Change camera
        static int mode = 0;
        if (key == 'c') ardrone.setCamera(++mode % 4);

        // Display the image
        cv::imshow("camera", image);
        
        ardrone.move3D(vx(), vy(), vz(), vr());
        
        if(tick==50) land=true;
        
        if(check()){
            ardrone.landing();
        }
        
        tick++;
        
        // optional altitude check
        cout << "Altitude: " << ardrone.getAltitude() << endl;
    }

    // See you
    ardrone.close();

    return 0;
}







