#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

#include <sstream>

using namespace std;

ros::Publisher velocity_publisher;

const double PI = 3.14159265359;

void move(double speed, double distance, bool isForward);
void rotate(double angular_speed, double angle, bool clockwise);
double degrees2radians(double angle_in_degrees);

int main(int argc, char **argv)
{
    // Initiate new ROS node named "talker"
    ros::init(argc, argv, "tb3_driver");
    ros::NodeHandle n;
    double speed, angular_speed;
    bool isForward, clockwise;

    velocity_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 100);
    
    ROS_INFO("\n\n\n******START TESTING************\n");
    cout << "Enter speed: ";
    cin >> speed;
    cout << "Enter angular velocity (degree/sec): ";
    cin >> angular_speed;
    cout << "Clockwise? (1 for Yes, 0 for No): ";
    cin >> clockwise;

    // Gerakan Bujur Sangkar
    for (int i = 0; i < 4; i++) {
        // Pindah 1 meter maju
        move(speed, 1.0, true); // Menggunakan move untuk maju
        // Putar 90 derajat
        rotate(degrees2radians(angular_speed), degrees2radians(90), clockwise); // 90 derajat
    }

    return 0;
}

void move(double speed, double distance, bool isForward)
{
    geometry_msgs::Twist vel_msg;
    if (isForward)
        vel_msg.linear.x = abs(speed);
    else
        vel_msg.linear.x = -abs(speed);
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    vel_msg.angular.z = 0;

    double t0 = ros::Time::now().toSec();
    double current_distance = 0.0;
    ros::Rate loop_rate(100);
    do
    {
        velocity_publisher.publish(vel_msg);
        double t1 = ros::Time::now().toSec();
        current_distance = speed * (t1 - t0);
        ros::spinOnce();
        loop_rate.sleep();
    } while (current_distance < distance);
    
    vel_msg.linear.x = 0;
    velocity_publisher.publish(vel_msg);
}

void rotate(double angular_speed, double relative_angle, bool clockwise)
{
    geometry_msgs::Twist vel_msg;
    vel_msg.linear.x = 0;
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;

    if (clockwise)
        vel_msg.angular.z = -abs(angular_speed);
    else
        vel_msg.angular.z = abs(angular_speed);

    double current_angle = 0.0;
    double t0 = ros::Time::now().toSec();
    ros::Rate loop_rate(10);
    do
    {
        velocity_publisher.publish(vel_msg);
        double t1 = ros::Time::now().toSec();
        current_angle = angular_speed * (t1 - t0);
        ros::spinOnce();
        loop_rate.sleep();
    } while (current_angle < relative_angle);

    vel_msg.angular.z = 0;
    velocity_publisher.publish(vel_msg);
}

double degrees2radians(double angle_in_degrees)
{
    return angle_in_degrees * PI / 180.0;
}