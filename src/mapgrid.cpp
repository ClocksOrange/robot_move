#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nav_msgs/GetMap.h>
#include <signal.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>
#include <string>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <visualization_msgs/Marker.h>
#include <sstream>
#define dur(a)  0<=a&&a<99
#define dis 18
using namespace std;

vector<vector<int> > grid;

    double orX,xx;
    double orY,yy;
    double mapResolution;

void shutdown(int sig)
{
    ros::Duration(1).sleep(); // sleep for  a second
    ROS_INFO(" ended!");
    ros::shutdown();
}

void init_markers(visualization_msgs::Marker* marker,int c)
{

  marker->type     = visualization_msgs::Marker::SPHERE;
  marker->action   = visualization_msgs::Marker::ADD;
  marker->lifetime = ros::Duration();//0 is forever
  marker->scale.x  = 0.2;
  marker->scale.y  = 0.2;
  marker->scale.z  = 0.1;	
  marker->color.r  = (double)c;
  marker->color.g  = 1.0;
  marker->color.b  = 0.0;
  marker->color.a  = 1.0;
  marker->header.frame_id = "map";
  marker->header.stamp = ros::Time::now();
	 
}
void  pub_markers(vector< geometry_msgs::Point>& point, int k,int c)
{
  
  //s1+=topic;
  ros::NodeHandle node;
  ros::Publisher marker_pub;
  marker_pub = node.advertise<visualization_msgs::Marker>("visualization_marker",1000);

  vector< visualization_msgs::Marker>  line_list;
  line_list.reserve(200);
  visualization_msgs::Marker  m;
  int i=0;
  while( (i<k) && (ros::ok()) )
  {
    m.pose.position=point[i];
    line_list.push_back(m);
	//line_list[i].pose.position=point[i];
    init_markers(&line_list[i],c);    
    std::stringstream sstr;
	sstr<<i;
	line_list[i].ns=sstr.str();
	line_list[i].id=i;
    marker_pub.publish(line_list[i]);
    ROS_INFO("this is %d marker",i+1);
	ros::spinOnce();
	ros::Duration(1).sleep();
	++i;
   }
}
// scan the cell  surrounding conditon
bool free_cell(int w,int h) //   0.5*0.5 m2
{
    for(int i=-2;i<=2;i+=2)
    {
        for(int j=-2;j<=2;j+=2)
        {
            if(!(dur(grid[i+w][j+h])))
               return false;
        }
    }
    return true;

}
//  move  to  goal point

void move_goal(vector< geometry_msgs::Point>& point, int k)
{
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac("move_base",true);
    signal(SIGINT, shutdown);
    /*new add erase market */
     ros::NodeHandle n2;
     ros::Publisher m_pub;
     m_pub = n2.advertise<visualization_msgs::Marker>("visualization_marker",1000);
     visualization_msgs::Marker  m;

    ROS_INFO("Waiting for move_base action server...");
    if(!ac.waitForServer(ros::Duration(60)))
    {
        ROS_INFO("Can't connected to move base server");
        return ;
    }
    ROS_INFO("Connected to move base server");
    ROS_INFO("Starting navigation test");
    int count=0;
    while (count<k&&ros::ok()) {
        //intialize the goal
        move_base_msgs::MoveBaseGoal goal;
        //use the map frame to define goal poses
        goal.target_pose.header.frame_id="map";
        goal.target_pose.header.stamp = ros::Time::now();
        //Set the goal pose to the i-th point
            m.pose.position=point[count];
        goal.target_pose.pose.position=point[count];
        goal.target_pose.pose.orientation.w=1;
        goal.target_pose.pose.orientation.y=0;
        goal.target_pose.pose.orientation.z=0;
        goal.target_pose.pose.orientation.x=0;
        ROS_INFO(" Start the robot moving toward the goal %d ",count+1);
        ac.sendGoal(goal);
       // ROS_INFO("%")
        //If we dont get there in time, abort the goal
        if(!ac.waitForResult(ros::Duration(30)))
        {
                 ac.cancelGoal();
                    ROS_INFO("Timed out achieving goal %d ",count+1);
        }
        else
        {	
             signal(SIGINT, shutdown);
             //We made it!
             if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
                {
                          ROS_INFO("This is %d Goal succeeded!",count+1);
                    //new
                    init_markers(&m,1); 
                    std::stringstream sstr;
                	sstr<<count;
                    m.ns=sstr.str();
                	m.id=count;
                    m_pub.publish(m);
                    ROS_INFO("the succeeded goal marker has change"); 
                }
                else
                {
                   ROS_INFO("The base  failed for some reason,this is %d Goal ",count+1);
                }
         }
         ROS_INFO(" sleep for 5 second");
         ros::Duration(5).sleep(); // sleep for  a second
         count += 1;
    }
     ROS_INFO("end...");
}


void update_wifi_change(double x,double y)
{
    int cell_j=(x-orX)/mapResolution;
    int cell_i=(y-orY)/mapResolution;
    vector<geometry_msgs::Point> uppoint;
    geometry_msgs::Point p;
    p.x=x;p.y=y;
    ROS_INFO("%d,%d,%.3lf,%.3lf",cell_i,cell_j,p.x,p.y);
    uppoint.push_back(p);
    for(int k=0;k<4;k+=2)
    {   
        cell_j-=dis;
        cell_i-=dis;
        int j=cell_j;
        int i=cell_i;
            for(int t=0;t<2+k;t++)
            {      
                 if(free_cell(i,j))
                 {
                    p.x=orX+j*mapResolution;
                    p.y=orY+i*mapResolution;
                    ROS_INFO("%d,%d,%lf,%lf",i,j,orX+j*mapResolution,orY+i*mapResolution);
                    uppoint.push_back(p);
      
                  }
                  j+=dis;
            }
            for(int t=0;t<2+k;t++)
            {      
                 if(free_cell(i,j))
                 {
                    p.x=orX+j*mapResolution;
                    p.y=orY+i*mapResolution;
                    uppoint.push_back(p);
                  }
                  i+=dis;
     
            }
            for(int t=0;t<2+k;t++)
            {      
                 if(free_cell(i,j))
                 {
                    p.x=orX+j*mapResolution;
                    p.y=orY+i*mapResolution;
                    uppoint.push_back(p);
                  }
                  j-=dis;
                
            }
             for(int t=0;t<2+k;t++)
            {      
                 if(free_cell(i,j))
                 {
                    p.x=orX+j*mapResolution;
                    p.y=orY+i*mapResolution;
                    uppoint.push_back(p);
                  }
                  i-=dis;
            }
   
    }
    for(int i=0;i<uppoint.size();i++)
        printf("%.3lf,%.3lf  ",uppoint[i].x,uppoint[i].y);
    ros::Duration(1).sleep();
    pub_markers(uppoint,uppoint.size(),1);
    ROS_INFO("move base");
  //  move_goal(uppoint,uppoint.size());
}

void chatterCallback(const nav_msgs::OccupancyGrid& maps)
{
    int rows;//heghit
    int cols;//width

//fsdafsd 
    int count = 0;
    ROS_INFO("Received a %d X %d map @ %.3f m/px\n", maps.info.width, maps.info.height, maps.info.resolution);
    rows = maps.info.height;   //cell number
    cols = maps.info.width;
    mapResolution = maps.info.resolution;
    orX = maps.info.origin.position.x;      //origin
    orY = maps.info.origin.position.y;

    cout << maps.info.origin.position.x << " ";
    cout << maps.info.origin.position.y << " ";
    cout << maps.info.origin.position.z << endl;
    cout << maps.info.origin.orientation.x << " ";
    cout << maps.info.origin.orientation.y << " ";
    cout << maps.info.origin.orientation.z << " ";
    cout << maps.info.origin.orientation.w << endl;

	// ros::Duration(5).sleep();

    int currCell =0;
    int widmin=cols,widmax=0,heigmin=rows,heigmax=0; 
    for(int i=0;i<rows;i++)   //cut min shapes
    {
        for(int j=0;j<cols;j++)
        {
            if(maps.data[currCell]!=-1)
            {

                if(j<widmin)
                    widmin=j;
                if(j>widmax)
                    widmax=j;
                if(i<heigmin)
                    heigmin=i;
                if(i>heigmax)
                    heigmax=i;

            }
            currCell++;
        }
    }
    //
     ROS_INFO("widmin=%d,widmax=%d,heigmin=%d,heigmax=%d",widmin,widmax,heigmin,heigmax);

    int k=cols-1-widmax+widmin;
    currCell = heigmin*cols+widmin;
    cols=widmax-widmin+1;    //  small new map of cut
    rows=heigmax-heigmin+1;  //
    orX += widmin * mapResolution;  // the new origin
    orY += heigmin * mapResolution;  //

    ROS_INFO("%.3lf,%.3lf",orX,orY);
    grid.resize(rows);        //init grid
    for (int i = 0; i < grid.size(); i++)
    {
        grid[i].resize(cols);
    }
    //
    for(int i=0;i<rows;++i)
    {
        for(int j=0;j<cols;++j)
        {
            grid[i][j]=maps.data[++currCell];  //reach cell
        }
        currCell+=k; //update rows
    }

    vector< geometry_msgs::Point> buff(cols);
    vector< geometry_msgs::Point> point;
    point.reserve(200);
    int t=0,flag=0,f=0;                //flag
    for(int i=10;i<rows;i+=dis)
    {   k=0;                              //0.5m distans cell scan,  0.5==0.05*10
        for(int j=10;j<cols;j+=6)
        {
            if(free_cell(i,j))  //aviluable reach piont
           //if(grid[i][j]==0)
            {
                xx=orX+j*mapResolution;
                yy=orY+i*mapResolution;
                flag=1;
                if(t%(dis/6)==0)
                {
                    buff[k].x=xx;
                    buff[k].y=yy;
                    cout<<grid[i][j] << ":("<<buff[k].x<<","<<buff[k].y<<")";
                    k++;
                    flag=0;
                }
                t++;
            }
            else
            {       //special point
                if(flag==1)
                {
                    if(t%(dis/6)==2)
                        --k;
                    buff[k].x=xx;
                    buff[k].y=yy;
                    flag=0;
                    cout<<grid[i][j] << "("<<buff[k].x<<","<<buff[k].y<<")";
                    k++;
                    t=0;
                } 
            }
        }
        cout<<"   k:"<<k<<endl;
        ++f;
        if(f%2)
            for(int j=0;j<k;j++)
            {
                point.push_back(buff[j]);
            }
        else 
            for(int j=k-1;j>=0;j--)
            {
                point.push_back(buff[j]);
            } 
             
    }
   
    pub_markers(point,point.size(),0);
    ROS_INFO("move base");
    // update_wifi_change(3.2,-2.5);
   // move_goal(point,point.size());

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mapgrid");
    ros::NodeHandle nh;

    //定义订阅节点，名称为chatter，指定回调函数chatterCallback
    //但收到消息，则调用函数chatterCallback来处理。
    //参数1000，定义队列大小，如果处理不够快，超过1000，则丢弃旧的消息。
    ros::Subscriber sub = nh.subscribe("/move_base/global_costmap/costmap", 1000, chatterCallback);
    //signal(SIGINT, shutdown);
    //调用此函数才真正开始进入循环处理，直到 ros::ok()返回false才停止。
    ros::spin();
    return 0;
}
