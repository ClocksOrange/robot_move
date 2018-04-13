#include <ros/ros.h>
#include <signal.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>
#include <string.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <visualization_msgs/Marker.h>
#include <cmath>
#include<iostream>
#include<string>
#define  de_num 4    //destination number

void shutdown(int sig)
{

  ros::Duration(1).sleep(); // sleep for  a second
  ROS_INFO(" ended!");
  ros::shutdown();
}

void init_markers(visualization_msgs::Marker *marker)
{

  marker->type     = visualization_msgs::Marker::ARROW;
  marker->action   = visualization_msgs::Marker::ADD;
  marker->lifetime = ros::Duration(600);//0 is forever
  marker->scale.x  = 0.4;
  marker->scale.y  = 0.2;
  marker->scale.z  = 0.2;	
  marker->color.r  = 0.0;
  marker->color.g  = 1.0;
  marker->color.b  = 0.0;
  marker->color.a  = 1.0;

  marker->header.frame_id = "map";
  marker->header.stamp = ros::Time::now();
	 
}
int main(int argc, char** argv)
{
  ros::init(argc, argv, "four_points");
  ros::NodeHandle node;

  //Define a marker publisher.
  ros::Publisher marker_pub;
  marker_pub = node.advertise<visualization_msgs::Marker>("visualization_marker", de_num );

  //a pose consisting of a position and orientation in the map frame.
  geometry_msgs::Point point;
  geometry_msgs::Pose pose_list[de_num];
  point.x = -7.04;
  point.y = -2.01;
  point.z = 0.0;
  pose_list[0].position=point;

  point.x = -8.92;
  point.y = -1.92;
  point.z = 0.0;
  pose_list[1].position = point;

  point.x = -8.738;
  point.y = -5.731;
  point.z = 0;
  pose_list[2].position = point;


  point.x =-1.563 ;
  point.y = -4.139;
  point.z = 0.0;
  pose_list[3].position = point;


  //convert the angles to quaternions
  double angle = M_PI/2;
  int angle_count = 0;
  for(angle_count = 0; angle_count < de_num;angle_count++ )
  {
      pose_list[angle_count].orientation = tf::createQuaternionMsgFromRollPitchYaw(0, 0, angle);
      angle = angle + M_PI/2;
  }
 
  
   //Set a visualization marker at each point
  visualization_msgs::Marker  line_list[de_num];
  // Set dedtination number makker 
	line_list[0].ns  ="poingt0" ;
	line_list[1].ns  ="poingt1" ;
	line_list[2].ns  ="poingt2" ;
	line_list[3].ns  ="poingt3" ;
  for(int i = 0; i < de_num; i++)
  {
	
      	init_markers(&line_list[i]);
 	line_list[i].id  = i;
   	line_list[i].pose=pose_list[i];// set eometry_msgs/Pose
  }

  //marker_pub.publish(line_list[count]);

	//visualization_msgs::Marker pub_first=line_list[count];
///	ROS_INFO("this is %d point",count);
	//marker_pub.publish(pub_first);
  //Cycle through the four waypoints

  actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);
  signal(SIGINT, shutdown);

  ROS_INFO("Waiting for move_base action server...");
  //Wait 60 seconds for the action server to become available
  if(!ac.waitForServer(ros::Duration(60)))
  {
    ROS_INFO("Can't connected to move base server");
    return 1;
  }
  ROS_INFO("Connected to move base server");
  ROS_INFO("Starting navigation test");
  int count = 0,i=0;
  while( (count < de_num) && (ros::ok()) )
  {
     //Update the marker display
	while( (i<de_num) && (ros::ok()) )
	{
		ROS_INFO("this is %d destination ",i+1);
     		marker_pub.publish(line_list[i]);
		ros::spinOnce();
		ros::Duration(1).sleep();
		++i;
	}
	
	//Intialize the goal	
	move_base_msgs::MoveBaseGoal goal;
	//Use the map frame to define goal poses
	goal.target_pose.header.frame_id = "map";
	//Set the time stamp to "now"
	goal.target_pose.header.stamp = ros::Time::now();
	//Set the goal pose to the i-th point
	goal.target_pose.pose = pose_list[count]; 
	//Send the goal pose to the MoveBaseAction server
	ROS_INFO(" Start the robot moving toward the goal %d ",count+1);
	ac.sendGoal(goal); 
	//If we dont get there in time, abort the goal
	if(!ac.waitForResult(ros::Duration(180)))
	{
		 ac.cancelGoal();
       		 ROS_INFO("Timed out achieving goal %d ",count+1);
	}
	else 
	{	 //We made it!
		 if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        	{
          		  ROS_INFO("This is %d Goal succeeded!",count+1);
      		 }
        	else
        	{
           		 ROS_INFO("The base  failed for some reason,this is %d Goal ",count+1);
       		 }
	
	}
     ROS_INFO(" sleep for 20 second");
     ros::Duration(20).sleep(); // sleep for  a second
     count += 1;
  }

  ROS_INFO("end...");
  return 0;
}
