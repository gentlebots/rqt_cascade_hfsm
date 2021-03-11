// NOLINT (legal/copyright) 

#ifndef IDM_H_
#define IDM_H_

#include <string>

#include "std_msgs/msg/string.hpp"
#include "rclcpp_cascade_lifecycle/rclcpp_cascade_lifecycle.hpp"

namespace cascade_hfsm
{
class ID : public rclcpp_cascade_lifecycle::CascadeLifecycleNode
{
public:
  ID();
  virtual ~ID();

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_activate(const rclcpp_lifecycle::State & previous_state);

  virtual rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
  on_deactivate(const rclcpp_lifecycle::State & previous_state);

FUNCTIONS_STATES
FUNCTIONS_TRANSITIONS

  void tick();

protected:
  rclcpp::Time state_ts_;

private:
FUNCTIONS_ACTS

STATES

  int state_;

  std::string myBaseId_;
  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr state_pub_;
  rclcpp::TimerBase::SharedPtr loop_timer_;
};

}  // namespace cascade_hfsm

#endif  // IDM_H_
