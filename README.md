# LPS
Local Positioning System.

This Project is dedicated to develop a local positioning system for the Ar Drone using augmented reality markers.

There is no stable release at the moment, but feel free to use as much of the code as needed.

All that is needed is a raspberry pi, a pi camera (needs to be calibrated (the calibration program is in poc)) and an AR Drone. The Raspberry Pi should be mounted under the drone with the camera pointing down. We used a carpet of aruco markers, but the more markers are used, the slower the program gets.

###Installation for the Raspberry Pi:

		sudo apt-get update
		sudo apt-get upgrade -y
		sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev libjpeg-dev libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libtiff5-dev libjasper-dev libpng12-dev libxvidcore-dev libx264-dev libgtk2.0-dev libatlas-base-dev gfortran -y
		cd ~
####Installing Opencv 3.1:

		wget -O opencv.zip https://github.com/Itseez/opencv/archive/3.1.0.zip
		unzip opencv.zip
		cd ~/opencv-3.1.0/
		mkdir build
		cd build
		cmake -D CMAKE_BUILD_TYPE=RELEASE \ -D CMAKE_INSTALL_PREFIX=/usr/local \ -D INSTALL_C_EXAMPLES=OFF \ -D INSTALL_PYTHON_EXAMPLES=ON \ -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.0.0/modules \ -D BUILD_EXAMPLES=ON ..
		make -j7
#####get a coffee, this takes a while

		sudo make install
		sudo ldconfig
		
		sudo apt-get update
		sudo apt-get upgrade -y
		sudo apt-get install libicu-dev freeglut3 freeglut3-dev libgstreamer0.10-dev libgstreamer-plugins-base0.10-dev libxine2-dev -y
		export OpenCV_DIR= ~/opencv-3.1.0/ build/
		cd
		
####Installing Aruco 2.0.10:

		wget https://sourceforge.net/projects/aruco/files/2.0.10/aruco-2.0.10.zip
		unzip aruco-2.0.10.zip
		cd aruco-2.0.10
		mkdir build
		cd build
		cmake ..
		make
		sudo make install
		cd
		
####Install raspicam

		git clone https://github.com/cedricve/raspicam.git
		cd raspicam/
		mkdir build
		cd build 
		cmake ..
		make
		sudo make install
		cd
		
####Install lps

		sudo apt-get install libavdevice-dev libavfilter-dev
		git clone https://github.com/nikovert/lps.git
		cd lps/poc/raspberry_pi/lps/
		mkdir build
		cd build
		cmake ../src
		make
		
###The last step is to enable the pi camera and reboot

•	enabel the camera and reboot with the command

		sudo raspi-config
	
