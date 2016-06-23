# LPS
Local Positioning System.

This Project is dedicated to develop a positioning system based on the aruco marker model used for augmented reality.

###Installing Aruco
Currently lps only runs on osx, therfore we will only documents an osx installation, we hope to update this issue soon.

•	First off, install OpenCV 
	http://www.learnopencv.com/install-opencv-3-on-yosemite-osx-10-10-x/

•	Download Aruco from 
	https://sourceforge.net/projects/aruco/files/1.3.0/

•	Open the Terminal and go to the location of the Aruco project

•	Add the following line to the documents aruco_test_board_gl_mask.cpp and aruco_test_board_gl.cpp (found under util)
	
		#include <opencv2/imgproc/imgproc.hpp>
	
•	Create and setup the build folder
	
		mkdir build
		cd build
		cmake ..
		make
		sudo make install

###Setting up Xcode for the first time

•	Open XCode Preferences 

•	Select Locations Tab 

•	Click Advanced 

• Change the location button from Unique to Legacy

###Creating a new project in Xcode for the lps library

•	Open XCode 

•	Create a new project 

•	Under OSX Application > Choose Command Line Tool 

•	Give your application a name then save it

•	Click the XCode project file in your inspector (which is the blue icon in the furthest left
hand tab). You should now have three tabs in the centre window. BuildSettings | Build Options | Build Rules 

•	Click Build Settings 

•	Scroll down until you find Search Paths 

•	Double click the Header Search Paths option, then Click the plus icon 

•	Type in the following 

		/usr/local/include

•	We will also need to link to the aruco folder, for instants: 

		/Users/"username"/Documents/OpenCv/aruco-1.3.0/src

•	Next Double Click the Library Search Paths option, then Click the plus icon 

•	Type in the following: 

		/usr/local/lib 

•	Click on your project name in the left-hand column then click on Build Phases at the top. Under "Link Binary with Libraries" click "+" then "other" and press "/" to navigate to /usr/local/lib and add "libaruco.dylib".

•	Under Build Settings, go to linking and add to Other linking:

		-lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab

 
