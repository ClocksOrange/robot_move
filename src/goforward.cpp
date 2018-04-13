#include <ros/ros.h>
#include <signal.h>
#include <geometry_msgs/Twist.h>
#include <string>
#include <iostream>
#include <visualization_msgs/Marker.h>
#include <sstream>


void init_markers(visualization_msgs::Marker *marker)
{

  marker->type     = visualization_msgs::Marker::ARROW;
  marker->action   = visualization_msgs::Marker::ADD;
  marker->lifetime = ros::Duration();//0 is forever
  marker->scale.x  = 0.4;
  marker->scale.y  = 0.2;
  marker->scale.z  = 0.2;	
  marker->color.r  = 0.0;
  marker->color.g  = 1.0;
  marker->color.b  = 0.0;
  marker->color.a  = 1.0;

  marker->header.frame_id = "/frame_marker";
  marker->header.stamp = ros::Time::now();
	 
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "GoForward");//初始化ROS,它允许ROS通过命令行进行名称重映射
  //std::string topic = "/cmd_vel_mux/input/teleop";
  std::string s2,s1="namespace";
  //s1+=topic;
 // std::cout<<s1<<std::endl;
  ros::NodeHandle node;
  ros::Publisher marker_pub;
  marker_pub = node.advertise<visualization_msgs::Marker>("visualization_marker",10);
	
  visualization_msgs::Marker  line_list[4];
  std::stringstream sstr;
  for(int i=0;i<4;i++)
  {
	init_markers(&line_list[i]);
	line_list[i].pose.position.x=i;
	sstr<<i;
	s2=sstr.str();
	s2=s1+s2;
	line_list[i].ns=s2;
	line_list[i].id=i;
        marker_pub.publish(line_list[i]);
	//ros::spinOnce();
  }
int i=0;
 while( (i<4) && (ros::ok()) )
	{
		ROS_INFO("this is %d destination ",i+1);
     		marker_pub.publish(line_list[i]);
		ros::spinOnce();
		ros::Duration(1).sleep();
		++i;
	}
	
 /* ros::NodeHandle node;//为这个进程的节点创建一个句柄
  cmdVelPub = node.advertise<geometry_msgs::Twist>(topic, 1);//告诉master将要在/cmd_vel topic上发布一个geometry_msgs/Twist的消息
  ros::Rate loopRate(10);//The desired rate to run at in Hz,ros::Rate对象可以允许你指定自循环的频率
  // Override the default ros sigint handler. This must be set after the first NodeHandle is creat

  ROS_INFO("goforward cpp start...");
  geometry_msgs::Twist speed; // 控制信号载体 Twist message
  while (ros::ok())
  {
    speed.linear.x = 0.1; // 设置线速度为0.1m/s，正为前进，负为后退
    speed.angular.z = 0; // 设置角速度为0rad/s，正为左转，负为右转
    cmdVelPub.publish(speed); // 将刚才设置的指令发送给机器人
    loopRate.sleep();//休眠直到一个频率周期的时间
  }*/
  return 0;
}
