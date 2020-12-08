#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <termios.h>
#include <std_msgs/Float64MultiArray.h>
#include <geometry_msgs/Vector3.h>
#include <cmath>
#include <sensor_msgs/Image.h>
#include <std_msgs/UInt8.h>
#include <std_srvs/Empty.h>
#include <std_srvs/Trigger.h>
#include <geometry_msgs/Twist.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#define COPYMODE 0644
using namespace std;


ros::Publisher gnss_puber; // Define publisher gnss_puber

static double gnss_time = 1;
static double gnss_x = 3;
static double gnss_y = 9;
static double gnss_PDOP = 4; //Position Dilution of Precision

void gnss_decode(std::string input_str, double* gnss_data)
{
    gnss_data[0] = -1;
    gnss_data[1] = -1;
    vector<string> arr1;  //定义一个字符串容器  Define a string container
    int position = 0;
    do
    {
        string tmp_s; 
        position = input_str.find(","); //找到逗号的位置       Find the position of the ","
        tmp_s = input_str.substr(0, position); //截取需要的字符串       Intercept the required string
        input_str.erase(0, position + 1); //将已读取的数据删去       Delete the data that has been read
        arr1.push_back(tmp_s);   //将字符串压入容器中     Press the string into the container
    } while (position != -1);

    if (arr1.size() > 0)
    {
        string title = arr1[0].substr(3, 3);  // Define NMEA title
        if (title == string("GGA"))
        {
            string time = arr1[1]; 
            string UTC_Time = time.substr(0, 2) + ":" + time.substr(2, 2) + ":" + time.substr(4, 2);

            string latitude_s = arr1[2];
            string NS_Indicator = arr1[3];
            string longitude_s = arr1[4];
            string EW_Indicator = arr1[5];

            double latitude = stod(latitude_s);
            double longitude = stod(longitude_s);

            int A = int(latitude / 100);
            latitude = (((latitude / 100) - A) * 100) / 60 + A; // Calculate the latitude in degrees
            A = int(longitude / 100);
            longitude = (((longitude / 100) - A) * 100) / 60 + A;  // Calculate the longitude in degrees

            string Latitude = to_string(latitude) + "°";  // Convert to string to storage
            string Longitude = to_string(longitude) + "°";

            gnss_data[0] = latitude;
            gnss_data[1] = longitude;
        }

        else if (title == string("RMC"))
        {
            string time_s = arr1[1];
            string UTC_Time = arr1[1].substr(0, 2) + ":" + arr1[1].substr(2, 2) + ":" + arr1[1].substr(4, 2);

            string latitude_s = arr1[3];
            string NS_Indicator = arr1[4];
            string longitude_s = arr1[5];
            string EW_Indicator = arr1[6];

            double latitude = stod(latitude_s);
            double longitude = stod(longitude_s);
            int A = int(latitude / 100);
            latitude = (((latitude / 100) - A) * 100) / 60 + A;
            A = int(longitude / 100);
            longitude = (((longitude / 100) - A) * 100) / 60 + A;

            //string Latitude = to_string(latitude) + "°";
            //string Longitude = to_string(longitude) + "°";

            gnss_data[0] = latitude;
            gnss_data[1] = longitude;
        }
        
    }

}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "gnss_node"); //Initialization of ROS node
    ros::NodeHandle n("~"); //Get the handle for node
    ros::Rate loop_rate(1000); //Define rate for repeatable operations.
    gnss_puber = n.advertise<std_msgs::Float64MultiArray>("gnss_data", 1);  //Defining topic to subscribe 发布话题gnss_data

    int fd = open("/dev/ttyACM0", O_RDWR|O_NOCTTY|O_NDELAY);  // Open gnss port
    int fdwrite = creat("/home/husarion/wenqi/gnss/data.txt", COPYMODE);
    char buff[1024];

    int sockfd;
    struct sockaddr_in servadd;
    inet_aton("108.61.171.128", &(servadd.sin_addr));
    servadd.sin_port = htons(29000); //将无符号短整型主机字节序转换为网络字节序
    servadd.sin_family = AF_INET; // AF_INET（TCP/IP – IPv4）
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (connect(sockfd, (struct sockaddr*)&servadd, sizeof(servadd)) != 0)
    {
        cout << "connected" << endl;
    }



    while (ros::ok()) //Check if ROS is working
    {
        int mess_len = read(sockfd, buff, 1024); //read data from 108.61.171.128
        write(fd, buff, mess_len); // Write data to the port
        int read_len = read(fd, buff, 1024); // Read GNSS data
        ssize_t result = write(fdwrite, buff, read_len);  // Write the data in to gnss_data.txt

        double gnss_data[2];

        gnss_decode(std::string(buff), gnss_data);  // Decode gnss data

        double gnss_data_lat = gnss_data[0];
        double gnss_data_lon = gnss_data[1];

        //cout<< gnss_data_lat << endl;
        //cout<< gnss_data_lon << endl;

        if (gnss_data_lat != -1) 
        {
            //package value to gnss_message
            cout << "in if" << endl;
            cout<< gnss_data_lat << endl;
            cout<< gnss_data_lon << endl;
            /*
            std::vector<double> gnss_data_buff;
            gnss_data_buff.push_back(gnss_time);
            gnss_data_buff.push_back(gnss_data_lat);
            gnss_data_buff.push_back(gnss_data_lon);
            gnss_data_buff.push_back(gnss_PDOP);
            */

            std_msgs::Float64MultiArray gnss_value; //定义消息gnss_value，往gnss_value中装入需要广播的消息
            gnss_value.data.push_back(gnss_data_lat);
            gnss_value.data.push_back(gnss_data_lon);
            //gnss_value.data = gnss_data_buff;  //将gnss_data_buff 装入 gnss_value
            gnss_puber.publish(gnss_value);  //广播
        }
        ros::spinOnce(); //Process all incoming messages.
        loop_rate.sleep(); //Wait until defined time passes.
    }
}







