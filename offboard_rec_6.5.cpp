#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include<string.h>
#include<geometry_msgs/PoseStamped.h>
#include<nav_msgs/Path.h>
#include<mavros_msgs/PositionTarget.h>
#include <mavros_msgs/CommandBool.h>
#include <std_msgs/String.h>

mavros_msgs::State current_state;

void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state = *msg;
}
geometry_msgs::PoseStamped current_pose;
mavros_msgs::PositionTarget target_location;
void pose_callback(const geometry_msgs::PoseStamped::ConstPtr& msg)
{
    current_pose = *msg;
}


bool point_reached(geometry_msgs::PoseStamped current_position, geometry_msgs::PoseStamped final_destination)
{
    float erro_in_x=std::fabs(current_position.pose.position.x-final_destination.pose.position.x);
    float erro_in_y=std::fabs(current_position.pose.position.y-final_destination.pose.position.y);
    float erro_in_z=std::fabs(current_position.pose.position.z-final_destination.pose.position.z);

    if(erro_in_x <= 0.4 && erro_in_y <=0.4 && erro_in_z<=0.5)
    {

        return true;`
    }
    else
    {
        return false;
    }


}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offboard_node");
    ros::NodeHandle nh;
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>("/mavros/state", 10, state_cb);
    ros::Subscriber pose_sub =  nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose", 1, pose_callback);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 1);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");

    ros::Rate rate(20.0);
    //ros::AsyncSpinner spinner(4);
    ros::Time last_request = ros::Time::now();

    geometry_msgs::PoseStamped pose;
    geometry_msgs::PoseStamped next_pose;
    pose.pose.position.x = 0;
    pose.pose.position.y = 0;
    pose.pose.position.z = 2;


     int count=1;


    //send a few setpoints before starting
    for(int i = 200; ros::ok() && i > 0; --i)
    {

        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }


    ROS_INFO("READY FOR OFFBOARD MODE");
    geometry_msgs::PoseStamped captured_pose;

    while(ros::ok() )
    {
        captured_pose=current_pose;
	ros::spinOnce();
	ROS_INFO("POSITION CAPTURED");
	local_pos_pub.publish(captured_pose);
	ROS_INFO("POSITION CAPTURED %f", captured_pose.pose.position.x);

    }


    while(ros::ok()  && current_state.mode == "OFFBOARD")
    {
	ROS_INFO("INSIDE OFFBOARD LOOP");

	if( count%5==1 )
	{

	next_pose.pose.position.y=captured_pose.pose.position.y + 6.5;
	next_pose.pose.position.x=captured_pose.pose.position.x;
	next_pose.pose.position.z=5.0;


	bool reached= point_reached(current_pose,next_pose);
	while(reached != true && ros::ok())
	{

 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);

	}

	while(reached != false && ros::ok())
	{

 	ros::spinOnce();
 	captured_pose=current_pose;
 	ROS_INFO("the cureent pose in count 1  x = %f", current_pose.pose.position.x);
 	ROS_INFO("the cureent pose in count 1  y = %f", current_pose.pose.position.y);
 	next_pose.pose.position.x=captured_pose.pose.position.x+radius;
 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	count++;
	}


	}


	if( count%5==2 )
	{

	next_pose.pose.position.y=captured_pose.pose.position.y;
	next_pose.pose.position.x=captured_pose.pose.position.x + 6.5;
	next_pose.pose.position.z=5.0;
	bool reached= point_reached(current_pose,next_pose);
	while(reached != true && ros::ok())
	{

 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	}

	while(reached != false && ros::ok())
	{

 	ros::spinOnce();
 	reached= point_reached(current_pose,next_pose);
 	captured_pose=current_pose;
 	ROS_INFO("the cureent pose in count 2  x = %f", current_pose.pose.position.x);
 	ROS_INFO("the cureent pose in count 2  y = %f", current_pose.pose.position.y);
 	next_pose.pose.position.y=captured_pose.pose.position.y-(radius);
 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	count++;
	}


	}

	if( count%5==3 )
 	{
    next_pose.pose.position.y=captured_pose.pose.position.y - 6.5;
	next_pose.pose.position.x=captured_pose.pose.position.x;
	next_pose.pose.position.z=5.0;
	bool reached= point_reached(current_pose,next_pose);
	while(reached != true && ros::ok())
	{

 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	}

	while(reached != false && ros::ok())
	{

 	ros::spinOnce();
 	captured_pose=current_pose;
 	ROS_INFO("the cureent pose in count 3  x = %f", current_pose.pose.position.x);
 	ROS_INFO("the cureent pose in count 3  y = %f", current_pose.pose.position.y);
 	next_pose.pose.position.x=captured_pose.pose.position.x-(radius);
 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	count++;
	}


	}

	if( count%5==4 )
	{

	next_pose.pose.position.y=captured_pose.pose.position.y - 6.5;
	next_pose.pose.position.x=captured_pose.pose.position.x;
	next_pose.pose.position.z=5.0;
	bool reached= point_reached(current_pose,next_pose);
	while(reached != true && ros::ok())
	{

 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	}

	while(reached != false && ros::ok())
	{

 	ros::spinOnce();
 	captured_pose=current_pose;
 	ROS_INFO("the cureent pose in count 4  x = %f", current_pose.pose.position.x);
 	ROS_INFO("the cureent pose in count 4  y = %f", current_pose.pose.position.y);
 	next_pose.pose.position.y=captured_pose.pose.position.y+radius;
 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	count++;
	}


	}

	if( count%5==0 )
	{

    next_pose.pose.position.z=0;
	bool reached= point_reached(current_pose,next_pose);
	while(reached != true && ros::ok())
	{

 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	}

	while(reached != false && ros::ok())
	{

 	ros::spinOnce();
 	captured_pose=current_pose;
 	ROS_INFO("the cureent pose in count 4  x = %f", current_pose.pose.position.x);
 	ROS_INFO("the cureent pose in count 4  y = %f", current_pose.pose.position.y);
 	next_pose.pose.position.y=captured_pose.pose.position.y+radius;
 	ros::spinOnce();
	local_pos_pub.publish(next_pose);
	reached= point_reached(current_pose,next_pose);
	count++;
	}


	}




    }
}
