#include "ros/ros.h"
#include "std_msgs/String.h"
#include <nav_msgs/GetMap.h>
#include <signal.h>
#include <vector>
#include <map>

using namespace std;

//定义回调函数chatterCallback，当收到chatter话题的消息就会调用这个函数。
//消息通过boost shared_ptr（共享指针）来传递。
//但收到消息，通过ROS_INFO函数显示到终端
void chatterCallback(const nav_msgs::OccupancyGrid& maps)
{
    int rows;
    int cols;
    double orX;
    double orY;
    double mapResolution;

    int count = 0;

    vector<vector<map<char, double> > > grid;

    ROS_INFO("Received a %d X %d map @ %.3f m/px\n", maps.info.width, maps.info.height, maps.info.resolution);
    rows = maps.info.height;
    cols = maps.info.width;
    mapResolution = maps.info.resolution;

    orX = maps.info.origin.position.x;
    orY = maps.info.origin.position.y;// +maps.info.height * mapResolution;

    cout << maps.info.origin.position.x << " ";
    cout << maps.info.origin.position.y << " ";
    cout << maps.info.origin.position.z << endl;
    cout << maps.info.origin.orientation.x << " ";
    cout << maps.info.origin.orientation.y << " ";
    cout << maps.info.origin.orientation.z << " ";
    cout << maps.info.origin.orientation.w << endl;

	 ros::Duration(5).sleep();
/*
	void shutdown(int sig)
	{

  	ros::Duration(1).sleep(); // sleep for  a second
  	ROS_INFO(" ended!");
  	ros::shutdown();
	}
*/
    grid.resize(rows);
    for (int i = 0; i < rows; i++)
    {
        grid[i].resize(cols);
    }
    int currCell = 0;
    for (int i = 0; i < rows; i++)
    {
		
        for (int j = 0; j < cols; j++)
        {
            //grid[i][j] = map.data[currCell];
            if(maps.data[currCell] == 0)
            {
                count++;
		grid[i][j]['x'] = orX + i * mapResolution;
                grid[i][j]['y'] = orY +j * mapResolution;
        	ROS_INFO("free point is data:%d X:%.3f Y:%.3f count:%d", maps.data[currCell],grid[i][j]['x'], grid[i][j]['y'], count);
		//signal(SIGINT, shutdown);
  		ros::Duration(50).sleep();
            }
            else
            {
                grid[i][j]['x'] = 9999;
                grid[i][j]['y'] = 9999;
            }

            currCell++;
        }
    }

}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "listener");
    ros::NodeHandle nh;

    //定义订阅节点，名称为chatter，指定回调函数chatterCallback
    //但收到消息，则调用函数chatterCallback来处理。
    //参数1000，定义队列大小，如果处理不够快，超过1000，则丢弃旧的消息。
    ros::Subscriber sub = nh.subscribe("/map", 1000, chatterCallback);

    //调用此函数才真正开始进入循环处理，直到 ros::ok()返回false才停止。
    ros::spin();

    return 0;
}
