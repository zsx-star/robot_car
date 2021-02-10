#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>


class Car
{
public: 
	Car();
	~Car(){};
	void run();
	bool init(ros::NodeHandle nh);
	
    const double degree = M_PI/180;
    const double radius = 2;
    
private:
	double angle;
	
private:
	ros::Publisher joint_pub;
	tf::TransformBroadcaster broadcaster;
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::JointState joint_state;
};

Car::Car():angle(0)
{
	
}

bool Car::init(ros::NodeHandle nh)
{	
	joint_pub = nh.advertise<sensor_msgs::JointState>("joint_states", 1);
    odom_trans.header.frame_id = "odom";     //参考
    odom_trans.child_frame_id = "base_link"; //小车本身
	return true;
}


void Car::run()
{
    ros::Rate loop_rate(30);
    
	while (ros::ok())
	{
	
        joint_state.header.stamp = ros::Time::now();
        joint_state.name.resize(4);
        joint_state.position.resize(4);
		joint_state.name[0] ="wheel1";
        joint_state.position[0] = 0;
		joint_state.name[1] ="wheel2";
        joint_state.position[1] = 0;
		joint_state.name[2] ="wheel3";
        joint_state.position[2] = 0;
		joint_state.name[3] ="wheel4";
        joint_state.position[3] = 0;
 
        odom_trans.header.stamp = ros::Time::now();
        odom_trans.transform.translation.x = radius * cos(angle);
        odom_trans.transform.translation.y = radius * sin(angle);
        odom_trans.transform.translation.z = 0.0;
        odom_trans.transform.rotation = tf::createQuaternionMsgFromYaw(angle);  //从四元数得到欧拉角度

        joint_pub.publish(joint_state);
        broadcaster.sendTransform(odom_trans);

        angle += degree/4;
        loop_rate.sleep();
	}
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "state_publisher");
    ros::NodeHandle nh;
    Car app;
    if(app.init(nh))
    	app.run();
    return 0;
}


