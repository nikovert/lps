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

•	Alternativly one can add the content of src to the include path 
	
		/usr/local/include/aruco
	
•	Next Double Click the Library Search Paths option, then Click the plus icon 

•	Type in the following: 

		/usr/local/lib 

•	Click on your project name in the left-hand column then click on Build Phases at the top. Under "Link Binary with Libraries" click "+" then "other" and press "/" to navigate to /usr/local/lib and add "libaruco.dylib".

•	Under Build Settings, go to linking and add to Other linking:

		-lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videoio -lopencv_videostab

###Installing on Raspberry

		sudo apt-get update
		sudo apt-get upgrade -y
		sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev libjpeg-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libtiff5-dev libjasper-dev libpng12-dev libxvidcore-dev libx264-dev libgtk2.0-dev libatlas-base-dev gfortran -y
		cd ~
		wget -O opencv.zip https://github.com/Itseez/opencv/archive/3.1.0.zip
		unzip opencv.zip
		cd ~/opencv-3.1.0/
		mkdir build
		cd build
		cmake -D CMAKE_BUILD_TYPE=RELEASE \ -D CMAKE_INSTALL_PREFIX=/usr/local \ -D INSTALL_C_EXAMPLES=OFF \ -D INSTALL_PYTHON_EXAMPLES=ON \ -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.0.0/modules \ -D BUILD_EXAMPLES=ON ..
		make -j7
		sudo make install
		sudo ldconfig
		sudo apt-get update
		sudo apt-get upgrade -y
		sudo apt-get install libicu-dev freeglut3 freeglut3-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libxine2-dev -y
		export OpenCV_DIR= ~/opencv-3.1.0/ build/
		cd
		wget https://sourceforge.net/projects/aruco/files/2.0.10/aruco-2.0.10.zip
		unzip aruco-2.0.10.zip
		cd aruco-2.0.10
		mkdir build
		cd build
		cmake ..
		make
		sudo make install
		cd
		git clone https://github.com/cedricve/raspicam.git
		cd raspicam/
		mkdir build
		cd build 
		cmake ..
		make
		sudo make install
		sudo apt-get install libavdevice-dev libavfilter-dev
		git clone https://github.com/felixge/lps.git
		cd lps/poc/raspberry_pi/lps/
		mkdir build
		cd build
		cmake ../src
		make

•	enabel the camera and reboot with the command
		sudo raspi-config
	
