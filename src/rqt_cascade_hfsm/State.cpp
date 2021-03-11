// Copyright 2021 Intelligent Robotics Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>

#include "ament_index_cpp/get_package_share_directory.hpp"

#include "rqt_cascade_hfsm/State.hpp"


namespace rqt_cascade_hfsm
{
State::State(std::string id, rclcpp::Node::SharedPtr node)
: QGraphicsItem(), id_(id), initial_(false), debugActive_(false), node_(node)
{
  setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

  std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");

  statePxmp_ = new QPixmap(QString::fromStdString(path + "/resource/RegularState.png"));
  stateActivePxmp_ = new QPixmap(QString::fromStdString(path + "/resource/RegularStateDbg.png"));

  last_active_ = node_->now();
}

State::~State()
{
  delete statePxmp_;
}

void State::setInitial(bool initial)
{
  if (initial == initial_) {
    return;
  }

  initial_ = initial;

  delete statePxmp_;
  std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");

  if (initial_) {
    statePxmp_ = new QPixmap(QString::fromStdString(path + "/resource/InitialState.png"));
  } else {
    statePxmp_ = new QPixmap(QString::fromStdString(path + "/resource/RegularState.png"));
  }

  update();
}

QRectF State::boundingRect() const
{
  return QRectF(-statePxmp_->width() / 2, -statePxmp_->height() / 2, statePxmp_->width(), statePxmp_->height());
}

void State::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  QPointF pos(-statePxmp_->width() / 2, -statePxmp_->height() / 2);

  if (debugActive_ && (node_->now() - last_active_).seconds() < 1.0) {
    painter->drawPixmap(pos, *stateActivePxmp_);
  } else {
    painter->drawPixmap(pos, *statePxmp_);
  }

  QRectF myRect(-statePxmp_->width() / 2, -statePxmp_->height() / 2, statePxmp_->width(), statePxmp_->height());
  painter->drawText(myRect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString::fromStdString(id_));
}

QVariant State::itemChange(GraphicsItemChange change, const QVariant & value)
{
  for (Dependency * dep : activations_) {
    dep->trackNodes();
  }
  for (Transition * tran : transitions_) {
    tran->trackNodes();
  }

  return QGraphicsItem::itemChange(change, value);
}

void State::addActivation(Dependency * dep)
{
  activations_.insert(dep);
}

void State::removeActivation(Dependency * dep)
{
  activations_.remove(dep);
}

void State::addTransition(Transition * tran)
{
  transitions_.insert(tran);
}

void State::removeTransition(Transition * tran)
{
  transitions_.remove(tran);
}

void State::debugIsActive(bool active)
{
  debugActive_ = active;
  last_active_ = node_->now();

  QGraphicsItem::update(boundingRect());
}

}  // namespace rqt_cascade_hfsm
