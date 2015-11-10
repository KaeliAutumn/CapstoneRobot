/*
 *  Arduino code to read the encoders and give that information to ROS
 */


#include <ros.h>
#include <Encoder.h>
#include <std_msgs/Int32.h>

// Set up ROS publishing
ros::NodeHandle  nh;
std_msgs::Int32 leftencoder_msg;
std_msgs::Int32 rightencoder_msg;
ros::Publisher pub_LeftEncoder("LeftEncoder", &leftencoder_msg);
ros::Publisher pub_RightEncoder("RightEncoder", &rightencoder_msg);

Encoder leftEnc(A0, A2); // Create left encoder object
Encoder rightEnc(A1, A3); // Create right encoder object

int newLeftPos = leftEnc.read();
int newRightPos = rightEnc.read();
int oldLeftPos = -999;
int oldRightPos = -999;

void setup()
{  
  Serial.begin(9600);
  nh.initNode();
  nh.advertise(pub_LeftEncoder);
  nh.advertise(pub_RightEncoder);
}



void loop()
{
  newLeftPos = leftEnc.read();
  newRightPos = rightEnc.read();
  // If the encoder positions have changed, update their values with ROS
  if (newLeftPos != oldLeftPos) {
    oldLeftPos = newLeftPos;
    leftencoder_msg.data = newLeftPos;
  }
  if (newRightPos != oldRightPos) {
    oldRightPos = newRightPos;
    rightencoder_msg.data = newRightPos;
  }
  pub_LeftEncoder.publish( &leftencoder_msg);
  pub_RightEncoder.publish( &rightencoder_msg);
  nh.spinOnce();
}