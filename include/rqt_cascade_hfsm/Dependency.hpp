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

#ifndef BICA_GUI_DEPENDENCY_H_
#define BICA_GUI_DEPENDENCY_H_

#include <string>
#include <list>

#include <QGraphicsItem>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QPointF>

#include "rqt_cascade_hfsm/State.hpp"
#include "rqt_cascade_hfsm/Component.hpp"

namespace rqt_cascade_hfsm
{
class State;
class Component;

class Dependency : public QGraphicsLineItem
{
public:
  Dependency(State * state, Component * comp);
  Dependency(const Dependency& other);
  ~Dependency();

  void trackNodes();

  Component * getComponent()
  {
    return comp_;
  }

  State * getState()
  {
    return state_;
  }

  QRectF boundingRect() const;
  QPainterPath shape() const;

private:
  Component * comp_;
  State * state_;
};

}  // namespace rqt_cascade_hfsm

#endif  // BICA_GUI_DEPENDENCY_H_
