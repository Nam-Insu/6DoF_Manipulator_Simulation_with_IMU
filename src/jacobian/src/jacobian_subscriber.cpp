#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32.h"
#include <Eigen/Eigen>
#include "ros_msg/msgData.h"

using namespace Eigen;
using namespace std;

void tutorialCallback(const ros_msg::msgData::ConstPtr& msg){
  ROS_INFO("I heard : [%d]", msg->data);
}
//void chatterCallback(const std_msgs::String::ConstPtr& msg)
//{
//  ROS_INFO("I heard: [%s]", msg->data.c_str());
/*
void chatCallback(const ros_msg::msgData& msg2)
{
  for (int i = 0;i<6;i++) {
     std::cout<<"Joint state"<<i<<" : "<<msg2.Join_state[i]<<std::endl;
  }
  std::cout<<"////////////////////////////////////////////////////"<<std::endl;
}
void chatterCallback(const ros_msg::msgData& msg2)
{
  for (int i = 0;i<6;i++) {
     std::cout<<"Joint "<<i<<" : "<<msg2.Join_data[i]<<std::endl;
  }
  std::cout<<"////////////////////////////////////////////////////"<<std::endl;
}*/
#define PI 3.14159265359

volatile double d_a[8] = { 0,     0,    0,      0.0825, -0.0825, 0,      0.088,  0 };
volatile double d_d[8] = { 0.333, 0,    0.316,  0,      0.384,   0,      0,      0.107 };
volatile double alpha[8] = { 0,   -PI / 2,PI / 2,   PI / 2,   PI / 2,    -PI / 2,  PI / 2,   0 };
volatile double theta[8] = { 0, 0,0,0, 0,30*PI/180,45*PI/180,0 };//simulation default joint state

void TransMatrix(Matrix4d& T, double theta, int n)
{
    T(0, 0) = cos(theta); T(0, 1) = -cos(alpha[n]) * sin(theta); T(0, 2) = sin(alpha[n]) * sin(theta);  T(0, 3) = cos(theta) * d_a[n];
    T(1, 0) = sin(theta); T(1, 1) = cos(alpha[n]) * cos(theta);  T(1, 2) = -sin(alpha[n]) * cos(theta); T(1, 3) = sin(theta) * d_a[n];
    T(2, 0) = 0.0;        T(2, 1) = sin(alpha[n]);               T(2, 2) = cos(alpha[n]);               T(2, 3) = d_d[n];
    T(3, 0) = 0.0;        T(3, 1) = 0.0;                         T(3, 2) = 0.0;                         T(3, 3) = 1.0;
}

Vector3d Position_vector(const Vector3d& P_0toF, const Vector3d& P, const Vector3d& Z)
{
    Vector3d P_j, P_itoF;
    P_j.setZero();
    P_itoF = P - P_0toF;
    P_j(0, 0) = P_itoF(1, 0) * Z(2, 0) - P_itoF(2, 0) * Z(1, 0);
    P_j(1, 0) = P_itoF(2, 0) * Z(0, 0) - P_itoF(0, 0) * Z(2, 0);
    P_j(2, 0) = P_itoF(0, 0) * Z(1, 0) - P_itoF(1, 0) * Z(0, 0);
    return P_j;
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "jacobian_subscriber");
  ros::NodeHandle nh;

  Matrix4d T_base;
  Matrix4d T;
  Matrix4d T_m[8];


  MatrixXd Jacobian(6, 6);
  MatrixXd I_Jacobian(6, 6);
  MatrixXd I(6, 6);
  MatrixXd I_3(3, 3);

  MatrixXd End_velo(6, 1);
  MatrixXd Joint_velo(6, 1);
  MatrixXd target_positon(3, 1);

  ros::Rate loop_rate(10);
  //ros::Subscriber sub = nh.subscribe("Joint_msg", 1000, chatterCallback);
 //ros::Subscriber joint_sub = nh.subscribe("Joint_state", 1000, chatCallback);
  ros::Subscriber sub2 = nh.subscribe("tutorial_msg",1000,tutorialCallback);
  ros::spin();
  while (ros::ok())
  {
    //ros_msg::msgData msg2;
    //ros_msg::msgData joint_msg;
    //msg2.data = cnt++;

    ROS_INFO("Send : ");
   // tutorial_pub.publish(msg2);
    T_base.setIdentity();

    for (int i = 0; i < 8; i++)
    {
        TransMatrix(T, theta[i], i);
        T_base *= T;
        T_m[i] = T_base;
    }
    Jacobian.block(0, 0, 3, 1) = Position_vector(T_m[7].block(0, 3, 3, 1), T_m[0].block(0, 3, 3, 1), T_m[0].block(0, 2, 3, 1));
    Jacobian.block(0, 1, 3, 1) = Position_vector(T_m[7].block(0, 3, 3, 1), T_m[1].block(0, 3, 3, 1), T_m[1].block(0, 2, 3, 1));
    Jacobian.block(0, 2, 3, 1) = Position_vector(T_m[7].block(0, 3, 3, 1), T_m[3].block(0, 3, 3, 1), T_m[3].block(0, 2, 3, 1)); //J_p[3];
    Jacobian.block(0, 3, 3, 1) = Position_vector(T_m[7].block(0, 3, 3, 1), T_m[4].block(0, 3, 3, 1), T_m[4].block(0, 2, 3, 1));
    Jacobian.block(0, 4, 3, 1) = Position_vector(T_m[7].block(0, 3, 3, 1), T_m[5].block(0, 3, 3, 1), T_m[5].block(0, 2, 3, 1));
    Jacobian.block(0, 5, 3, 1) = Position_vector(T_m[7].block(0, 3, 3, 1), T_m[6].block(0, 3, 3, 1), T_m[6].block(0, 2, 3, 1));

    Jacobian.block(3, 0, 3, 1) = T_m[0].block(0, 2, 3, 1);
    Jacobian.block(3, 1, 3, 1) = T_m[1].block(0, 2, 3, 1);
    Jacobian.block(3, 2, 3, 1) = T_m[3].block(0, 2, 3, 1);
    Jacobian.block(3, 3, 3, 1) = T_m[4].block(0, 2, 3, 1);
    Jacobian.block(3, 4, 3, 1) = T_m[5].block(0, 2, 3, 1);
    Jacobian.block(3, 5, 3, 1) = T_m[6].block(0, 2, 3, 1);

    I_Jacobian = (Jacobian + 0.0000003 * I).inverse();
    Joint_velo = I_Jacobian * End_velo;

    //theta[0] += 0.01 * Joint_velo(0, 0);
    //theta[1] += 0.01 * Joint_velo(1, 0);
    //theta[3] += 0.01 * Joint_velo(2, 0);
    //theta[4] += 0.01 * Joint_velo(3, 0);
    //theta[5] += 0.01 * Joint_velo(4, 0);
    //theta[6] += 0.01 * Joint_velo(5, 0);

    for (int i = 0;i<6;i++) {
     // ROS_INFO("Send : %f", Joint_velo(i,0));
    //    joint_msg.Join_data[i] = Joint_velo(i,0);
    }
    //joint_pub.publish(joint_msg);
    cout << "End_Effector Position is " << endl << T_m[7].block(0, 3, 3, 1) << endl;
    ROS_INFO("////////////////////////////////////////////");
    //chatter_pub.publish(Join_velocity);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
