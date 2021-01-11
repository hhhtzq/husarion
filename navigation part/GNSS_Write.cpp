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
#include <iomanip>

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


void gnss_decode(std::string input_str, double* gnss_data)
{
    gnss_data[0] = -1;
    gnss_data[1] = -1;
    gnss_data[2] = -1;
    gnss_data[3] = -1;

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
            double time_d = stod(arr1[1]);
            string UTC_Time = arr1[1].substr(0, 2) + ":" + arr1[1].substr(2, 2) + ":" + arr1[1].substr(4, 2);

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

            //string Latitude = to_string(latitude);  // Convert to string to storage
            //string Longitude = to_string(longitude);

            double fix_status = stod(arr1[6]); // Fix mode; 0 = Invalid; 1 = 2D/3D; 2 = DGNSS; 4 = Fixed RTX; 5 = Float RTX; 6 = Dead Reckoning

            gnss_data[0] = time_d;
            gnss_data[1] = latitude;
            gnss_data[2] = longitude;
            gnss_data[3] = fix_status;
        }      
    }

}

int main(int argc, char** argv)
{
    int fdwrite = creat("/home/husarion/wenqi/gnss_data.txt",COPYMODE);
    stringstream fileoutstream;

    ros::init(argc, argv, "gnss_node"); //Initialization of ROS node
    ros::NodeHandle n("~"); //Get the handle for node
    ros::Rate loop_rate(1000); //Define rate for repeatable operations.
    gnss_puber = n.advertise<std_msgs::Float64MultiArray>("gnss_data", 1);  //Defining topic to subscribe 发布话题gnss_data

    int fd = open("/dev/ttyACM0", O_RDWR|O_NOCTTY|O_NDELAY);  // Open gnss port
    char buff[1024];

    int sockfd;
    struct sockaddr_in servadd;
    inet_aton("108.61.171.128", &(servadd.sin_addr));
    servadd.sin_port = htons(29000); 
    servadd.sin_family = AF_INET; // AF_INET（TCP/IP – IPv4）
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // Create socket “sockfd”

    if (connect(sockfd, (struct sockaddr*)&servadd, sizeof(servadd)) != 0) 
       // Make a connection request to the server
    {
        cout << "Connected" << endl;
    }
    
    recv_len = recv(sockfd, recv_buf, 100, 0); // Receive data

    while (ros::ok()) //Check if ROS is working
    {
        int mess_len = read(sockfd, buff, 1024); //read data from 108.61.171.128
        write(fd, buff, mess_len); // Write data to the port
        int read_len = read(fd, buff, 1024); // Read GNSS data

        double gnss_data[4];

        gnss_decode(std::string(buff), gnss_data);  // Decode gnss data

        double gnss_data_time = gnss_data[0];
        double gnss_data_lat = gnss_data[1];
        double gnss_data_lon = gnss_data[2];
        double gnss_data_status = gnss_data[3];


        if (gnss_data_lat != -1) 
        {
            //package value to gnss_message
            cout << gnss_data_time << "\t\t";
            cout << gnss_data_lat << "\t\t";
            //cout << fixed << setprecision(5) << gnss_data_lon << "\t\t";
            cout << gnss_data_lon << "\t\t";
            cout << gnss_data_status << endl;

            fileoutstream << gnss_data_time << " " << setprecision(10) << gnss_data_lat << " " << setprecision(10) << gnss_data_lon << endl;
            string outstring = fileoutstream.str();
            ssize_t result = write(fdwrite,outstring.c_str(),outstring.size());
            fileoutstream.str("");

            std_msgs::Float64MultiArray gnss_value; //定义消息gnss_value，往gnss_value中装入需要广播的消息
            gnss_value.data.push_back(gnss_data_time);
            gnss_value.data.push_back(gnss_data_lat);
            gnss_value.data.push_back(gnss_data_lon);
            gnss_value.data.push_back(gnss_data_status);
            gnss_puber.publish(gnss_value);  //广播
        }
        ros::spinOnce(); //Process all incoming messages.
        loop_rate.sleep(); //Wait until defined time passes.
    }
}
