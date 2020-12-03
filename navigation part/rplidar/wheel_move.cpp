#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <stdlib.h>
int main(int argc,char **argv)
{
	ros::init(argc,argv,"cmd_node");
	ros::NodeHandle n;
	ros::Publisher cmd_pub=n.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
	ros::Rate loop_rate(10);
	while(ros::ok())
	{
		geometry_msgs::Twist msg;
		//msg.linear.x=10;
		msg.linear.x=(double)(rand()/(double(RAND_MAX)));
		msg.angular.z=(double)(rand()/(double(RAND_MAX)));
		cmd_pub.publish(msg);
		ROS_INFO("msg.linear.x:%f , msg.angular.z:%f",msg.linear.x,msg.angular.z);
		loop_rate.sleep();
	}
	return 0;
}