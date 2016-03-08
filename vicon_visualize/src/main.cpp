#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <std_msgs/Float32.h>

ros::Publisher path_pub;
ros::Publisher path_gen;
ros::Publishre error_pub;

nav_msgs::Path path_msg;
nav_msgs::Path path_gen_msg;

//! @note not necessary
ros::Publisher plot_pub;
std_msgs::Float32 plot_msg;

void pose_feedback(const geometry_msgs::TransformStamped::ConstPtr &cmd)
{
	geometry_msgs::PoseStamped pose_to_add;
	pose_to_add.pose.position.x = cmd->transform.translation.x; 
	pose_to_add.pose.position.y = cmd->transform.translation.y;
	pose_to_add.pose.position.z = cmd->transform.translation.z;
	
	path_msg.poses.push_back(pose_to_add);
}
void gen_feedback(const geometry_msgs::TransformStamped::ConstPtr &cmd)
{
	geometry_msgs::PoseStamped pose_to_add;
	pose_to_add.pose.position.x = cmd->transform.translation.x; 
	pose_to_add.pose.position.y = cmd->transform.translation.y;
	pose_to_add.pose.position.z = cmd->transform.translation.z;
	
	path_gen_msg.poses.push_back(pose_to_add);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "vicon_rviz"); 
  ros::NodeHandle n("~");

  std::string topicName;
  std::string pathName;
  std::string pathGen;
  std::string genTopic;
  std::string error;
  std::string safety;
  std::string avoidance;

  n.param(std::string("topicName"), topicName, std::string("/vicon/M100_1/M100_1"));
  n.param("pathName", pathName, std::string("vis_path"));
  n.param("pathGen",pathGen,std::string("pathGen"));
  n.param("error",error,std::string("error"));
  n.param("safety",safety,std::string("safety"));
  n.param("avoidance",safety,std::string("avoidance"));

  ROS_INFO("%s %s\n", pathName.c_str(), topicName.c_str());

  ros::Subscriber vicon_sub	= n.subscribe(topicName,100, pose_feedback);
  ros::Subscriber gen_sub 	= n.subscribe(genTopic,100,gen_callback);

  path_pub = n.advertise<nav_msgs::Path>(pathName, 100);
  path_gen = n.advertise<nav_msgs::Path>(pathGen,100);  
  path_msg.header.frame_id = "/world";

  ros::Rate r(5);
  while(n.ok())
  {
    ros::spinOnce();

    path_pub.publish(path_msg);
    path_gen.publish(path_gen_msg);

    r.sleep();
  }

  return 0;
}
