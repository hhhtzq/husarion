#include “ros/ros.h”
#include “sensor_msgs/LaserScan.h”
#include “geometry_msgs/Twist.h”
#include <stdlib.h>
#include <stdio.h>
#define RAD2DEG(x) ((x)*180./M_PI)

int stopFlag=0;
int turnFlag=0;
float distMinLeft=0.0;
float distMinRight=0.0;
void miniAngleCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	int size = scan->scan_time / scan->time_increment;
	int minIndexLeft = 0;
	int minIndexRight = size/2;
	for(int i=479; i<size/2; i++)
    {
		if (scan->ranges[i] < scan->ranges[minIndexRight] && scan->ranges[i] > 0.15){
		minIndexRight = i;
        }
    }
	for (int i = size/2; i <959 ; i++)
	{
		if (scan->ranges[i] < scan->ranges[minIndexLeft] && scan->ranges[i] > 0.15){
		minIndexLeft = i;
		}
	}
	distMinLeft = scan->ranges[minIndexLeft];
	distMinRight = scan->ranges[minIndexRight];
}
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	int count = scan->scan_time / scan->time_increment;
	ROS_INFO("I heard a laser scan %s[%d]:", scan->header.frame_id.c_str(), count);
	ROS_INFO("angle_range, %f, %f", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
	 
	for(int i = 0; i < count; i++) {
	    float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
	    if(degree >=-30 && degree <=30 ){
		    ROS_INFO(": [%f, %f]", degree, scan->ranges[i]);
		    if (scan->ranges[i] <= 0.8)
		        {
			    stopFlag = 1;
		        }
		        else    stopFlag = 0;
		}
	    else 
	    {
	    	continue;
	    }
	    miniAngleCallback(scan);
		if(stopFlag == 1 && distMinLeft > distMinRight)
	    {
	                turnFlag = 1;//turn left;
	    }
	    else 
	    {       
		    turnFlag = 0;//turn right;
	    }
	 
	
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, “rplidar_node_client”);
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
	 
	ros::Publisher pub=n.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
	ros::Rate loop_rate(15);
	 
	while(ros::ok())
	{
		ros::spinOnce();
		geometry_msgs::Twist msg;
		msg.linear.x=0.3;
		//msg.linear.x=(double)(rand()/(double(RAND_MAX)));
		//msg.angular.z=(double)(rand()/(double(RAND_MAX)));
		if(stopFlag == 1 && turnFlag==1)  // ture left
		{
		    msg.linear.x= 0;
		    msg.angular.z=2;
		}
		else if(stopFlag == 1 && turnFlag ==0)  // ture right
		{
		msg.linear.x= 0;
		    msg.angular.z=-2;
		}
		else if (stopFlag == 0)
		{
		msg.linear.x=0.3;
		}
		pub.publish(msg);
		ROS_INFO("msg.linear.x:%f ,  msg.angular.z:%f",msg.linear.x,msg.angular.z);
		loop_rate.sleep();
	} 
	return 0;
}