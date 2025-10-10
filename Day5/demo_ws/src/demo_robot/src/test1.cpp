#include<rclcpp/rclcpp.hpp>
#include<geometry_msgs/msg/quaternion.hpp>
#include<tf2_ros/transform_broadcaster.h>
#include<tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include<cmath>
#include<thread>
#include<chrono> // For time related functions

using namespace std::chrono;

class TFBroadcasterNode : public rclcpp::Node
{
	public:
		TFBroadcasterNode():
			Node("broacaster_node")
			{
				RCLCPP_INFO(this->get_logger(),"Started the broadcaster node!");
				broadcasterObject = std::make_shared<tf2_ros::TransformBroadcaster>(this);
				timerObject = this->create_wall_timer(100ms,std::bind(&TFBroadcasterNode::BroadcasterFunction,this));
			}
			void BroadcasterFunction();
	private:
		rclcpp::TimerBase::SharedPtr timerObject;
		std::shared_ptr<tf2_ros::TransformBroadcaster> broadcasterObject;
		
};

void TFBroadcasterNode:: BroadcasterFunction()
{
	geometry_msgs::msg::TransformStamped t1;
	geometry_msgs::msg::TransformStamped t2;

	t1.header.stamp = this->get_clock()->now();
	t2.header.stamp = this->get_clock()->now();

	t1.header.frame_id = "base_link";
	t1.child_frame_id = "first_link";

	t2.header.frame_id = "base_link";
	t2.child_frame_id = "second_link";

	t1.transform.translation.x = 0.5;
	t1.transform.translation.y = 0.5;
	t1.transform.translation.z = 0.0;


	t2.transform.translation.x = -0.5;
	t2.transform.translation.y = -0.5;
	t2.transform.translation.z = 0.0;

	tf2::Quaternion q1;
	tf2::Quaternion q2;

	q1.setRPY(0,0,3.14/2);
	t1.transform.rotation.x = q1.x();
	t1.transform.rotation.y = q1.y();
	t1.transform.rotation.z = q1.z();
	t1.transform.rotation.w = q1.w();

	q2.setRPY(0,0,-3.14/2);
	t2.transform.rotation.x = q2.x();
	t2.transform.rotation.y = q2.y();
	t2.transform.rotation.z = q2.z();
	t2.transform.rotation.w = q2.w();


	broadcasterObject->sendTransform(t1);
	broadcasterObject->sendTransform(t2);

	RCLCPP_INFO(this->get_logger(), "TF message broadcasted");
}

int main(int argc, char * argv[])
{
	rclcpp::init(argc, argv);
	auto TestNode = std::make_shared<TFBroadcasterNode>();
	rclcpp::spin(TestNode);
	rclcpp::shutdown();
	return 0;

}

