#include “ros/ros.h”
#include “sensor_msgs/LaserScan.h”

#define RAD2DEG(x) ((x)*180./M_PI)
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	int count = scan->scan_time / scan->time_increment;
	ROS_INFO(“I heard a laser scan %s[%d]:”, scan->header.frame_id.c_str(), count);
	ROS_INFO(“angle_range, %f, %f”, RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
	for(int i = 0; i < count; i++) {

	    float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
	    if(degree >=-60 && degree <=60) //避光雷达角度设置：-60°-60°，并输出显示scan->angle_increment和count
		ROS_INFO(": [%f, %f, %f, %d]", degree, scan->ranges[i], scan->angle_increment, count);
	    else 
		continue;
	}
}
int main(int argc, char **argv)
{
ros::init(argc, argv, “rplidar_node_client”);
ros::NodeHandle n;

ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
 
ros::spin();
 
return 0;
}