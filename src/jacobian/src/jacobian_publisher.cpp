#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Int32.h"
#include <Eigen/Eigen>
#include "ros_msg/msgData.h"
#include "sensor_msgs/Imu.h"

using namespace Eigen;
using namespace std;

typedef Eigen::Matrix<double, 4, 4> Matrix4d;

#define PI 3.14159265359

volatile double d_a[8] = {   0,     0,        0,      82.5,     - 82.5,          0,       88.0,       0 };
volatile double d_d[8] = { 333.0,   0,    316.0,         0,      384.0,          0,          0,      107.0 };
volatile double alpha[8] = {      0,   -PI /2,  PI / 2,    PI / 2,     -PI / 2,     PI / 2,        PI / 2,       0 };
volatile double theta[8] = {      0,        0,       0,  0*PI/180,           0,   0*PI/180,      0*PI/180,       0 };//simulation default joint state
volatile double top_theta[6] =    {  166*PI/180,  101*PI/180,   0*PI/180, 166*PI/180, 215*PI/180, 166*PI/180 };
volatile double bottom_theta[6] = { -166*PI/180, -101*PI/180,-176*PI/180,-166*PI/180,  -1*PI/180,-166*PI/180 };
Matrix4d T_base;
Matrix4d T;
Matrix4d T_m[8];


MatrixXd Jacobian(6, 6);
MatrixXd I_Jacobian(6, 6);
MatrixXd I(6, 6);
MatrixXd I_3(3, 3);

MatrixXd Joint_velo(6, 1);
MatrixXd target_positon(3, 1);

Vector3d P_d;         //// start position

ros::Publisher joint_pub;
//volatile double X_velo[5] = {0,};
//volatile double Y_velo[5] = {0,};
//volatile double Z_velo[5] = {0,};
volatile double x_vel= 0;
volatile double y_vel= 0;
volatile double z_vel= 0;

volatile double pre_x_vel= 0;
volatile double pre_y_vel= 0;
volatile double pre_z_vel= 0;

//volatile int cnt=0;
void TransMatrix(Matrix4d& T, double theta, int n)
{

  if(alpha[n]==PI / 2)
  {
    T(0, 0) =  cos(theta);  T(0, 1) = -sin(theta);   T(0, 2) =  0.0;   T(0, 3) =  d_a[n];
    T(1, 0) =  0.0;         T(1, 1) =  0.0;          T(1, 2) = -1.0;   T(1, 3) =  -d_d[n];
    T(2, 0) =  sin(theta);  T(2, 1) =  cos(theta);   T(2, 2) =  0.0;   T(2, 3) =  0.0;
    T(3, 0) =  0.0;         T(3, 1) =  0.0;          T(3, 2) =  0.0;   T(3, 3) =  1.0;
  }
  else if(alpha[n]== -PI / 2)
  {
    T(0, 0) =  cos(theta);  T(0, 1) = -sin(theta);   T(0, 2) = 0.0;  T(0, 3) =  d_a[n];
    T(1, 0) =  0.0;         T(1, 1) =  0.0;          T(1, 2) = 1.0;  T(1, 3) =  d_d[n];
    T(2, 0) = -sin(theta);  T(2, 1) = -cos(theta);   T(2, 2) = 0.0;  T(2, 3) =  0.0;
    T(3, 0) =  0.0;         T(3, 1) =  0.0;          T(3, 2) = 0.0;  T(3, 3) =  1.0;
  }
  else if(alpha[n] == 0)
  {
    T(0, 0) = cos(theta); T(0, 1) = -sin(theta); T(0, 2) = 0.0;  T(0, 3) =  d_a[n];
    T(1, 0) = sin(theta); T(1, 1) =  cos(theta); T(1, 2) = 0.0;  T(1, 3) =  0;
    T(2, 0) = 0.0;        T(2, 1) =  0.0;        T(2, 2) = 1.0;  T(2, 3) =  d_d[n];
    T(3, 0) = 0.0;        T(3, 1) =  0.0;        T(3, 2) = 0.0;  T(3, 3) = 1.0;
  }
  else {
    T(0, 0) = cos(theta);                 T(0, 1) = -sin(theta);                   T(0, 2) = 0.0;             T(0, 3) =  d_a[n];
    T(1, 0) = sin(theta) * cos(alpha[n]); T(1, 1) =  cos(theta) * cos(alpha[n]);   T(1, 2) = -sin(alpha[n]);  T(1, 3) = -sin(alpha[n]) * d_d[n];
    T(2, 0) = sin(theta) * sin(alpha[n]); T(2, 1) =  cos(theta) * sin(alpha[n]);   T(2, 2) =  cos(alpha[n]);  T(2, 3) =  cos(alpha[n]) * d_d[n];
    T(3, 0) = 0.0;                        T(3, 1) =  0.0;                          T(3, 2) = 0.0;             T(3, 3) = 1.0;
  }
}

Vector3d Position_vector(const Vector3d& P_0toF, const Vector3d& P, const Vector3d& Z)
{

    Vector3d P_j, P_itoF;
    P_j.setZero();
    P_itoF = P-P_0toF;

    P_j = P_itoF.cross(Z);
    //P_j(0, 0) = P_itoF(1, 0) * Z(2, 0) - P_itoF(2, 0) * Z(1, 0);
    //P_j(1, 0) = P_itoF(2, 0) * Z(0, 0) - P_itoF(0, 0) * Z(2, 0);
    //P_j(2, 0) = P_itoF(0, 0) * Z(1, 0) - P_itoF(1, 0) * Z(0, 0);
    return P_j;
}
void PositionCallback(const sensor_msgs::Imu& msg)
{
  ROS_INFO("IMU X_vel[%f], Y_vel[%f], Z_vel[%f]", msg.linear_acceleration.x, msg.linear_acceleration.y,msg.linear_acceleration.z);
/*
  x_vel= 0;
  y_vel= 0;
  z_vel= 0;

  for (int i=0;i<4;i++) {
    X_velo[i]=X_velo[i+1];
    Y_velo[i]=Y_velo[i+1];
    Z_velo[i]=Z_velo[i+1];
  }
  X_velo[4]=msg.linear_acceleration.x;
  Y_velo[4]=msg.linear_acceleration.y;
  Z_velo[4]=msg.linear_acceleration.z;

  for (int i=0;i<5;i++) {
    x_vel +=X_velo[i];
    y_vel +=Y_velo[i];
    z_vel +=Z_velo[i];
  }*/
  P_d = T_m[7].block(0, 3, 3, 1);
  x_vel= 0.97*pre_x_vel +(1.0-0.97)*msg.linear_acceleration.x;
  y_vel= 0.97*pre_y_vel +(1.0-0.97)*msg.linear_acceleration.y;
  z_vel= 0.97*pre_z_vel +(1.0-0.97)*msg.linear_acceleration.z;
  ROS_INFO("filter X_vel[%f], Y_vel[%f], Z_vel[%f]", x_vel,y_vel,z_vel);
  pre_x_vel=x_vel;
  pre_y_vel=y_vel;
  pre_z_vel=z_vel;
  //if(msg.linear_acceleration.x>0.1 ||msg.linear_acceleration.x<-0.1)
  if(-750.0<P_d(0)<750.0)
   {
    P_d(0) -=  100.0*x_vel;
  }
  //if(msg.linear_acceleration.y>0.1 ||msg.linear_acceleration.y<-0.1)
  if(-750.0<P_d(1)<750.0)
  {
    P_d(1) -=  100.0*y_vel;
  }
  //if(msg.linear_acceleration.z>0.1 ||msg.linear_acceleration.z<-0.1)
  if(50.0<P_d(2)<750.0)
    P_d(2) +=  100.0*z_vel;
  std::cout<<P_d(0)<<", "<<P_d(1)<<", "<<P_d(2)<<std::endl;
  std::cout<<"/////////////////////////////////////////////////"<<std::endl;
}

void chatCallback(const ros_msg::msgData& msg2)
{
  ros_msg::msgData joint_msg;
  /////// rad data
  theta[0]=msg2.Join_state[0];
  theta[1]=msg2.Join_state[1];
  theta[3]=msg2.Join_state[2];
  theta[4]=msg2.Join_state[3];
  theta[5]=msg2.Join_state[4];
  theta[6]=msg2.Join_state[5];


  /////////////// vrep data, theta////////////////////////
  /*std::cout<<std::endl<<"/////////////  first_vrep -> jacobian  //////////////////"<<std::endl;
  for (int i =0;i<6 ;i++) {
    std::cout<<msg2.Join_state[i]*180/PI<<", ";
  }
  std::cout<<std::endl;
  for (int i =0;i<7 ;i++) {
    std::cout<<theta[i]*180/PI<<", ";
  }
  std::cout<<std::endl<<"///////////////////////////////"<<std::endl;
*/
  T_base.setIdentity();
  I.setIdentity();
  I_3.setIdentity();
//  Jacobian.setZero();
//  I_Jacobian.setZero();
  Joint_velo.setZero();

  ///////////////// Forward Kinematics, Matrixs ///////////////////////////
  //std::cout<<std::endl<<"/////////////  Second_ FK, Trans Matrixs  //////////////////"<<std::endl;
  for (int i = 0; i < 8; i++)
  {
      TransMatrix(T, theta[i], i);
      T_base *= T;
      T_m[i] = T_base;
      //cout << "joint matrix"<<i<< " Position is " << endl << T<< endl;
      //cout << " * joint matrix"<<i<< " Position is " << endl << T_m[i]<< endl;
      T.setZero();
      /*for(int j=0;j<4;j++)
      {
        cout << "joint matrix"<<i<< " Position is "<< endl;
        cout << T.block(j, 0, 1, 4)<<"             " <<T_m[i].block(j, 0, 1, 4)<< endl
      }*/
  }
  //std::cout<<std::endl<<"///////////////////////////////"<<std::endl;
  /////////////////////////////////////////////////////////////////////

  ///////////////Jacobian Matrix ////////////////////////////////////////

 //std::cout<<std::endl<<"/////////////  Third_ Jacobian, sudo Inverse  //////////////////"<<std::endl;

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

  //std::cout<<std::endl<<"Jacobian "<<std::endl<<Jacobian<<std::endl;



  MatrixXd Jp = Jacobian.block(0,0,3,6);
   //std::cout<<std::endl<<"Position Jacobian "<<std::endl<<Jp<<std::endl;
  I_Jacobian = Jp.transpose()*((Jp*Jp.transpose() + 0.003 * I_3).inverse());
  Joint_velo = 100.0 * I_Jacobian *(P_d-T_m[7].block(0, 3, 3, 1)); //end_velo eotls target_positon - now_position
  //I_Jacobian = (Jacobian + 0.00003 * I).inverse();
  //Joint_velo = I_Jacobian * End_velo; //end_velo eotls target_positon - now_position

  //std::cout<<std::endl<<"sudo Inverse "<<std::endl<<I_Jacobian<<std::endl;

  ///////////////////////////////////////////////////////////////////////////

  ///////////////////// Forth_ theta update and Transmit ////////////////////
//std::cout<<std::endl<<"/////////////  Forth_ Joint_velo, Update theta  //////////////////"<<std::endl;
/*
  for (int i =0;i<7 ;i++) {
    std::cout<<theta[i]*180/PI<<", ";
  }
  std::cout<<std::endl;
*/
  theta[0] += 0.01 * Joint_velo(0, 0);
  theta[1] += 0.01 * Joint_velo(1, 0);
  theta[3] += 0.01 * Joint_velo(2, 0);
  theta[4] += 0.01 * Joint_velo(3, 0);
  theta[5] += 0.01 * Joint_velo(4, 0);
  theta[6] += 0.01 * Joint_velo(5, 0);

  for (int i = 0;i<2;i++) {
    if(theta[i]>top_theta[i])
      theta[i] = top_theta[i];
    else if (theta[i]<bottom_theta[i])
      theta[i] = bottom_theta[i];
    //ROS_INFO("Send %d : %f",i, theta[i]*180/PI);
      joint_msg.Join_data[i] = theta[i];
  }
  for (int i = 2;i<6;i++) {
    if(theta[i+1]>top_theta[i])
      theta[i+1] = top_theta[i];
    else if (theta[i+1]<bottom_theta[i])
      theta[i+1] = bottom_theta[i];
   // ROS_INFO("Send %d : %f",i, theta[i+1]*180/PI);
      joint_msg.Join_data[i] = theta[i+1];
  }
  /*
  for (int i =0;i<6 ;i++) {
    std::cout<<Joint_velo(i,0)<<", ";
  }
  std::cout<<std::endl;
  for (int i =0;i<7 ;i++) {
    std::cout<<theta[i]*180/PI<<", ";
  }
  std::cout<<std::endl<<"///////////////////////////////"<<std::endl;
*/
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  joint_pub.publish(joint_msg);
  for (int i = 0; i < 8; i++)
  {
      //cout << "joint matrix"<<i<< " Position is " << endl << T_m[i]<< endl;
     //cout << "joint "<<i<< " Position is " << endl << T_m[i].block(0, 3, 3, 1) << endl;
  }
 cout << "Target Position is " << endl << P_d<< endl;
 cout << "End_Effector Position is " << endl << T_m[7].block(0, 3, 3, 1) << endl;
  //ROS_INFO("////////////////////////////////////////////\n");
  //std::cout<<"////////////////////////////////////////////////////"<<std::endl;
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "jaco_pub");
  ros::NodeHandle nh;

  //ros::Publisher chatter_pub = nh.advertise<std_msgs::Float32MultiArray>("chatter", 1000);
 // ros::Publisher tutorial_pub = nh.advertise<ros_msg::msgData>("tutorial_msg",1000);
  joint_pub = nh.advertise<ros_msg::msgData>("/vrep_franka_tutorial/Joint_msg",1000);
  ros::Rate loop_rate(10);
  ros::Subscriber joint_sub = nh.subscribe("/vrep_franka_tutorial/Joint_state", 1000, chatCallback);
  ros::Subscriber velo_sub = nh.subscribe("/imu", 1000, PositionCallback);
  /*Matrix4d T_base;
  Matrix4d T;

  T.setZero();
  T_base.setIdentity();
  Matrix4d T_m[8];


  MatrixXd Jacobian(6, 6);
  MatrixXd I_Jacobian(6, 6);
  MatrixXd I(6, 6);
  Jacobian.setZero();
  I_Jacobian.setZero();
  I.setIdentity();

  MatrixXd End_velo(6, 1);
  MatrixXd Joint_velo(6, 1);
  MatrixXd target_positon(3, 1);
  Joint_velo.setZero();
  End_velo.setZero();
*/
  P_d[0]=585.6;         //// 585.6
  P_d[1]=0.0;         //// 0
  P_d[2]=663.6;         //// 663.6

  while (ros::ok())
  {
/*    //ros_msg::msgData msg2;
    ros_msg::msgData joint_msg;
    //msg2.data = cnt++;

   // ROS_INFO("Send : %d",msg2.data);
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

    theta[0] += 0.01 * Joint_velo(0, 0);
    theta[1] += 0.01 * Joint_velo(1, 0);
    theta[3] += 0.01 * Joint_velo(2, 0);
    theta[4] += 0.01 * Joint_velo(3, 0);
    theta[5] += 0.01 * Joint_velo(4, 0);
    theta[6] += 0.01 * Joint_velo(5, 0);

    for (int i = 0;i<6;i++) {
     // ROS_INFO("Send : %f", Joint_velo(i,0));
        joint_msg.Join_data[i] = Joint_velo(i,0);
    }
    joint_pub.publish(joint_msg);
    cout << "End_Effector Position is " << endl << T_m[7].block(0, 3, 3, 1) << endl;
    ROS_INFO("////////////////////////////////////////////");
    //chatter_pub.publish(Join_velocity);
*/
    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
