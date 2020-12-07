#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <termios.h>
#include <std_msgs/Float64MultiArray.h>
#include <geometry_msgs/Vector3.h>
#include <cmath>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/UInt8.h>
#include <std_srvs/Empty.h>
#include <std_srvs/Trigger.h>
#include <geometry_msgs/Twist.h>

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
ros::Publisher gnss_puber;
static double gnss_time = 1;
static double gnss_x = 3;
static double gnss_y = 9;
static double gnss_PDOP = 4;

//testbench
double simulation_step_t = 0.02; // 0.02s
double imu_angle =0;
// 创建计算弧度和在x,y方向上走的每步的距离
void updata_location(double x_0,double y_0,double speed,double angle,double* result)
{
    double rad = (angle/180)*M_PI;
    double x_1 = x_0 + speed*simulation_step_t*cos(rad);
    double y_1 = y_0 + speed*simulation_step_t*sin(rad);
    result[0]=x_1;
    result[1]=y_1;
}

void suber_angle_Callback(const geometry_msgs::Vector3 &imu_message){
    imu_angle = imu_message.z;
    //cout<<"!angle: "<<imu_angle<<endl;
}
double simulation_speed;
void suber_speed_Callback(const geometry_msgs::Twist &speed_message){
    simulation_speed = speed_message.linear.x;
}

// GNSS decode
// 分隔符函数
vector<string> split3(const string &str, const char pattern)
{
    vector<string> res;
    stringstream input(str);
    string temp;

    while(getline(input, temp, pattern))
    {
        res.push_back(temp);
    }
    return res;
}


void GNSS_decode(std::string input_str,double* GNSS_data){
    GNSS_data[0] = -1;
    GNSS_data[1] = -1;
    size_t position = input_str.find_first_of ("$");
    if (position == -1) return;
    std::string item = input_str.substr(position+1);
    
    vector<string> vStr;
    vStr = split3(item,','); // 以逗号为分隔方式，分段存储，检索需要的段
    

    if (vStr.size()>0){
        string title = vStr[0];
        // title
        if(title == string("GNRMC")){
            string time = vStr[1];
            if(time == string("")){
                return;
            }
            string latitude_s = vStr[3];
            if(latitude_s == string("")){
                return;
            }
            double latitude = stod(latitude_s);
            

            string longitude_s = vStr[5];
            double longitude = stod(longitude_s);
            
            
//            a_la=floor(latitude/100)+(rem(latitude,100))/60;
//            a_lo=floor(longitude/100)+(rem(longitude,100))/60;
            // 转换la,lo
            int A = int (latitude/100);
            latitude = (((latitude/100)-A)*100)/60+A;
            A = int (longitude/100);
            longitude = (((longitude/100)-A)*100)/60+A;

            GNSS_data[0] = latitude;
            GNSS_data[1] = longitude;
            //cout<<"lat :"<<std::setprecision(10)<<latitude<<endl;
           // cout<<"lon :"<<std::setprecision(10)<<longitude<<endl;

                        
        }
    }
}
// GNSS decode end

int main(int argc, char **argv) {
    int fdwrite=creat("/home/husarion/yy/project/gnss_data/data.txt",COPYMODE);//写入的地址
    ros::init(argc, argv, "gnss_node");
    ros::NodeHandle n("~");
    ros::Rate loop_rate(1000);
    gnss_puber = n.advertise<std_msgs::Float64MultiArray>("gnss_data", 1);
//    ros::Subscriber suber_speed = n.subscribe("/cmd_vel",10,suber_speed_Callback);
//    ros::Subscriber suber_angle = n.subscribe("/rpy",10,suber_angle_Callback);
    
    // open GNSS serial port USB1
    int fd = open("/dev/gnss",O_RDWR);
    char buf[1024];
    while (ros::ok())
    {
        // read gnss
        int read_len = read(fd,buf,1024);
        //cout<<"len:"<<read_len<<"content:"<<std::string(buf)<<endl;
        /*if (read_len!=67){
            continue;
        }*/
        ssize_t result = write(fdwrite,buf,read_len);//读串口，写入
        double GNSS_data[2];
        GNSS_decode(std::string(buf),GNSS_data);
        double GNSS_data_lat = GNSS_data[0];
        double GNSS_data_lon = GNSS_data[1];
        if (GNSS_data_lat != -1){
                    //package value to gnss_message
            std::vector<double> gnss_data_buf;
            gnss_data_buf.push_back(gnss_time);
            gnss_data_buf.push_back(GNSS_data_lat);
            gnss_data_buf.push_back(GNSS_data_lon);
            gnss_data_buf.push_back(gnss_PDOP);
            std_msgs::Float64MultiArray gnss_value;
            gnss_value.data = gnss_data_buf;
            gnss_puber.publish(gnss_value); 
        }
        

        
        ros::spinOnce();
        
        loop_rate.sleep();
    }
    
}



