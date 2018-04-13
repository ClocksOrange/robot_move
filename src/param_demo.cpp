#include<ros/ros.h>

int main(int argc,char ** argv)
{
    ros::init(argc, argv, "param_demo");
    ros::NodeHandle n;
     ros::NodeHandle pn("~my_namespace");

    std::string s;
    int num;


   // n.setParam("string_param","1default_string"); 
 // pn.setParam("int_param",12333);   

    n.param<std::string>("string_param", s, "default_string");
    pn.param<int>("int_param", num, 2333);


    printf("\nstring_param:  %s\n", s.c_str());//输出初始化值
    printf("int_param:  %d\n\n", num);//输出初始化值

    ros::Rate loop_rate(0.5);

    while (ros::ok())
    {
        if(n.getParam("string_param",s))   
        printf("string_param:  %s\n", s.c_str());
        if( pn.getParam("int_param",num))
        printf("int_param:  %d\n\n", num);
         ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}