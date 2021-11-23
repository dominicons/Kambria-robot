# ohmni-ros-gui
# **The required packages to build the control GUI for Ohmnibot**

These packages alow us to perfrom certain tasks on OHmnibot:

## *Prequisite setting:*

**Clone the *ohmni_amcl* inside the local machine and build with catkin_make**

**#mounting a directory inside the Ohmnibot ROS docker environment**
```
In bot cli> docker run -it --network host --privileged -v /data/data/com.ohmnilabs.telebot_rtc/files:/app -v /dev:/dev -e ROS_IP=[bot_ip] [tb_control_docker_image]
```
*Create the ohmni_ws directory to contain the developing source code.*

**Clone *rplidar_ros* and *ohmni_gui_bridge* packages inside the ohmni_ws/src directory and run catkin_make to build the packages**

Note: 

The Lidar port might changes when we restart the bot so follow this step to redirect the port:
```
Ohmni_ws> cd src/rplidar_ros/launch
```
_Use Vim to edit the file rplidar.launch - Change the serial port value accordingly (Usually /dev/ttyUSB0 or /dev/ttyUSB1)_

<br />

## **1. Use RPLidar to map the working environment and perform SLAM on Ohmnibot.**
> Set up the environment.
```bash
In bot ohmni_ws> source devel/setup.bash
In bot ohmni_ws> roslaunch rplidar_ros rplidar.launch

In local machine> export ROS_IP=[local_machine_ip]
In local machine> export ROS_MASTER_URI=http://[bot_ip]:11311
```
> Mapping the work environment
```bash
In local machine> rviz # Open rviz and change the fixed frame into map,

#Use rqt_robot_steering to manually control the bot around the place. 
In local machine> rosrun rqt_robot_steering rqt_robot_steering

#Run Ros gmapping.
In local machine> rosrun gmapping slam_gmapping scan:=/scan
In local machine> rosrun map_server map_saver -f ~/map #After completed mapping
```
+ Note: the *map.pgm* and *map.yaml* should be save to the maps folder inside the package.
> Perform simple localization using ROS amcl
```bash
In local machine> roslaunch ohmni_amcl ohmni_amcl.launch
```

## **2. Building a web API to connect the Ohmni's ROS topics with other webAPIs by using Rosbridge.**
>Set up the environment.
```bash
# Run webserver for the GUI using rosbridge.
In bot ohmni_ws> roslaunch ohmni_gui_bridge websocket.launch

# Navigate to the directory with the .html file and run  a Python http server
In bot ohmni_ws> cd src/ohmni_gui_bridge/gui
In bot ohmni_ws> python -m SimpleHTTPServer
```
Afterwards you should be able to go to http://localhost:8000/ and you should see a directory listing containing the gui.html file. Click on it and it should open your website on the Ohmni touch screen web browser.


## **3. Publish Measured data to server via MQTT protocol.**
> Set up the environment.
1. We use ESP32 module to collect external data (SpO2 sensor, temperature sensor, ...)
2. Server HiveMQ: http://www.hivemq.com/demos/websocket-client/ 
> Run ESP32_Hive_MQ.ino file.

```bash
Note: For convenience, we simulate the measured values from the sensor using the included real-time matching random function.
```
- Replace your SSID, password to connect HiveMQ server.
- Data will be retrieved from hiveMQ server and displayed on screen Ohmni Robot
