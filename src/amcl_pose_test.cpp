#include "ros/ros.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"

void subCallback(const geometry_msgs::PoseWithCovarianceStamped& amcl_pose)
{
    ROS_INFO("(%lf,%lf)",amcl_pose.pose.pose.position.x,amcl_pose.pose.pose.position.y);

}
 int main(int argc, char **argv)
{
    /* code for main function */
    for(int i=0;i<argc;i++)
        ROS_INFO("This argv[%d]  : %s",i,argv[i]);
    ros::init(argc, argv, "amcl_pose_test");
    ros::NodeHandle nh;
    ros::Subscriber pose = nh.subscribe("amcl_pose", 1000, subCallback);
    ros::spin();
    return 0;
}