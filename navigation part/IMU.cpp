/////////////////////////////////////////////////////////////////////////////////////////
//  IMU                                                                        //
//                                                                                     //
//  Reference: IMU example code                                                //
/////////////////////////////////////////////////////////////////////////////////////////


#include "hFramework.h"
#include "hCloudClient.h"
#include "ros.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float32MultiArray.h"
#include "ROSbot.h"
//#include "sensor_msgs/Range.h"
//#include <vector>

using namespace hFramework;


ros::NodeHandle  nh;
std_msgs::Float32MultiArray IMU_msg;
ros::Publisher *IMU_pub;

static const SensorType sensor_type = SENSOR_LASER; 
static const ImuType imu_type = MPU9250;

void initializeFloat32MultiArray();

////////////////////   Iniziation Publish and Subscribe   ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
void initIMUPublisher()
{
    IMU_pub = new ros::Publisher("/IMU", &IMU_msg);
    nh.advertise(*IMU_pub);
}

/////////////////////////////////////////////////////////////////////////////////////

//////////////////// END Iniziation Publish ////////////////////////////////////////

void hMain()
{   
    rosbot.initROSbot(sensor_type);
	rosbot.initIMU();

    platform.begin(&RPi); 
    nh.getHardware()->initWithDevice(&platform.LocalSerial);
    nh.initNode();

    initIMUPublisher();
    initializeFloat32MultiArray();
    
    std::vector<float> IMU_dataVec = {0, 0, 0};
 
    while (true)
    {   
        nh.spinOnce();
        
        IMU_dataVec  = rosbot.getRPY();
		IMU_msg.data[0] = IMU_dataVec[0]; //Roll 
  		IMU_msg.data[1] = IMU_dataVec[1]; //Pitch
    	IMU_msg.data[2] = IMU_dataVec[2]; //Yaw
        IMU_pub->publish(&IMU_msg);
       
        sys.delay(200);
    }
}

//////////////////////       Functions       /////////////////////////////////////////

void initializeFloat32MultiArray(){

    IMU_msg.data = (float *)malloc(sizeof(float)*3);
    IMU_msg.data_length = 3;

  	IMU_msg.data[0] = 0; //Roll 
    IMU_msg.data[1] = 0; //Pitch
    IMU_msg.data[2] = 0; //Yaw
}


