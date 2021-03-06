#include "ros_node_full.h"

FullRosNode::FullRosNode()
{
}

/*****************************************************************************************
FullRosNode: construct an object for ros node
******************************************************************************************/
FullRosNode::FullRosNode(ros::NodeHandle nh ,std::string name){
    _nh = nh;
    _name = name;
    _queue_size = 10;

    _pub_w   = _nh.advertise <geometry_msgs::Vector3Stamped>("testbed/sensors/w"       , _queue_size);
    _pub_du  = _nh.advertise <geometry_msgs::TwistStamped>  ("testbed/motors/du"       , _queue_size);
    _pub_enc = _nh.advertise <geometry_msgs::Vector3Stamped>("testbed/sensors/encoders", _queue_size);
    _pub_imu = _nh.advertise <sensor_msgs::Imu>             ("testbed/sensors/imu"          , _queue_size);
    _pub_mag = _nh.advertise <sensor_msgs::MagneticField>   ("testbed/sensors/mag"          , _queue_size);
    _pub_rpy = _nh.advertise <geometry_msgs::Vector3Stamped>("testbed/sensors/rpy/filtered" , _queue_size);

    _sub_ang = _nh.subscribe("testbed/cmd/angle", _queue_size, &FullRosNode::cmdAngCallback,this);
    _sub_du  = _nh.subscribe("testbed/cmd/du"   , _queue_size, &FullRosNode::cmdDuCallback ,this);
    _cmd_ang [0] =0.0;
    _cmd_ang [1] =0.0;
    _cmd_ang [2] =0.0;
    _cmd_du[0] = 0.0;
    _cmd_du[1] = 0.0;
    _cmd_du[2] = 0.0;
    _cmd_du[3] = 0.0;
 }

/*****************************************************************************************
publishIMUMsg: Publish imu msgs
******************************************************************************************/
void FullRosNode::publishIMUMsg(float gyro[3], float acc[3], float quat[4]){
    sensor_msgs::Imu msg_imu;

    msg_imu.header.stamp = _time;
    msg_imu.header.seq++;
    msg_imu.angular_velocity.x = gyro[0];
    msg_imu.angular_velocity.y = gyro[1];
    msg_imu.angular_velocity.z = gyro[2];
    msg_imu.linear_acceleration.x = acc[0];
    msg_imu.linear_acceleration.y = acc[1];
    msg_imu.linear_acceleration.z = acc[2];
    msg_imu.orientation.x = quat[0];
    msg_imu.orientation.y = quat[1];
    msg_imu.orientation.z = quat[2];
    msg_imu.orientation.w = quat[3];
    _pub_imu.publish(msg_imu);
}

/*****************************************************************************************
publishMagMsg: Publish magnetometer msgs
******************************************************************************************/
void FullRosNode::publishMagMsg(float Mag[3]){
    sensor_msgs::MagneticField msg_mag;

    msg_mag.header.stamp = _time;
    msg_mag.header.seq++;
    msg_mag.magnetic_field.x = Mag[0];
    msg_mag.magnetic_field.y = Mag[1];
    msg_mag.magnetic_field.z = Mag[2];
    _pub_mag.publish(msg_mag);
}

/*****************************************************************************************
publishRPYMsg: Publish roll pitch yaw angles msgs
******************************************************************************************/
void FullRosNode::publishRPYMsg(float rpy[3])
{
    geometry_msgs::Vector3Stamped msg_rpy;

    msg_rpy.header.stamp = _time;
    msg_rpy.header.seq++;
    msg_rpy.vector.x = rpy[0];
    msg_rpy.vector.y = rpy[1];
    msg_rpy.vector.z = rpy[2];
    _pub_rpy.publish(msg_rpy);
}

/*****************************************************************************************
publishEncMsg: Publish encoder msgs
******************************************************************************************/
void FullRosNode::publishEncMsg(float enc[3]){
    geometry_msgs::Vector3Stamped msg_enc;

    msg_enc.header.stamp = _time;
    msg_enc.header.seq++;
    msg_enc.vector.x = enc[0];
    msg_enc.vector.y = enc[1];
    msg_enc.vector.z = enc[2];
    _pub_enc.publish(msg_enc);
}

/*****************************************************************************************
publishMsgs: Publish motors' inputs du msgs
******************************************************************************************/
void FullRosNode::publishWMsg(float w[3]){
    geometry_msgs::Vector3Stamped msg_w;

    msg_w.header.stamp = _time;
    msg_w.header.seq++;
    msg_w.vector.x = w[0];
    msg_w.vector.y = w[1];
    msg_w.vector.z = w[2];
    _pub_w.publish(msg_w);
}
/*****************************************************************************************
publishMsgs: Publish motors' inputs du msgs
******************************************************************************************/
void FullRosNode::publishDuMsg(float du[3]){
    geometry_msgs::TwistStamped msg_du;

    msg_du.header.stamp = _time;
    msg_du.header.seq++;
    msg_du.twist.angular.x = du[0];
    msg_du.twist.angular.y = du[1];
    msg_du.twist.angular.z = du[2];
    msg_du.twist.linear.x  = du[3];
    _pub_du.publish(msg_du);
}

/*****************************************************************************************
publishMsgs: Publish motors' inputs du msgs
******************************************************************************************/
void FullRosNode::publishAllMsgs(float gyro[3], float acc[3], float quat[4], float mag[3], float rpy[3], float w[3], float du[3], float enc[3]){
    _time = ros::Time::now();
    publishIMUMsg(gyro, acc, quat);
    publishMagMsg(mag);
    publishRPYMsg(rpy);   
    publishWMsg(w);
    publishDuMsg(du);
    publishEncMsg(enc);
}

/*****************************************************************************************
angCmdCallback: Read encoders and map it to gloabal variable
******************************************************************************************/
void FullRosNode::cmdAngCallback(const geometry_msgs::Vector3Stamped::ConstPtr& msg)
{
    _cmd_ang[0] = msg->vector.x;
    _cmd_ang[1] = msg->vector.y;
    _cmd_ang[2] = msg->vector.z;
}

/*****************************************************************************************
duCmdCallback: Read cmanded du values and map it to gloabal variable
******************************************************************************************/
void FullRosNode::cmdDuCallback(const geometry_msgs::TwistStamped::ConstPtr& msg)
{
    _cmd_du[0] = msg->twist.angular.x;
    _cmd_du[1] = msg->twist.angular.y;
    _cmd_du[2] = msg->twist.angular.z;
    _cmd_du[3] = msg->twist.linear.z;
}
