// NOLINT (legal/copyright)

#include "ID.hpp"

namespace cascade_hfsm
{
ID::ID()
: CascadeLifecycleNode("ID"), state_(INITIAL_STATE), myBaseId_("ID")
{
  declare_parameter("frequency");

  state_ts_ = now();
  state_pub_ = create_publisher<std_msgs::msg::String>("/" + myBaseId_ + "/state", 1);
}

ID::~ID()
{
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ID::on_activate(const rclcpp_lifecycle::State & previous_state)
{
INITIAL_ITERATION

  double frequency = 5.0;
  get_parameter_or<double>("frequency", frequency, 5.0);

  loop_timer_ = create_wall_timer(
    std::chrono::duration<double, std::ratio<1>>(1.0 / frequency),
    std::bind(&ID::tick, this));

  state_pub_->on_activate();
  return CascadeLifecycleNode::on_activate(previous_state);
}

rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn
ID::on_deactivate(const rclcpp_lifecycle::State & previous_state)
{
  loop_timer_ = nullptr;

  return CascadeLifecycleNode::on_deactivate(previous_state);
}

void ID::tick()
{
  std_msgs::msg::String msg;

  switch (state_) {
STATES_ITERATIONS
  }
}

STATE_ACTIVATIONS

}  // namespace cascade_hfsm
