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

#ifndef BICA_GUI_TRANSITION_H
#define BICA_GUI_TRANSITION_H

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

namespace rqt_cascade_hfsm
{
class State;

class Transition : public QGraphicsPolygonItem
{
public:
  Transition(State* src, State* dst);
  Transition(const Transition& other);

  ~Transition();

  void trackNodes();

  State* getSrcState()
  {
    return src_;
  }
  State* getDestinyState()
  {
    return dst_;
  }

  // QRectF boundingRect() const;
  // QPainterPath shape() const;

private:
  State* src_;
  State* dst_;

  QPolygonF arrowHead;
};
}  // namespace rqt_cascade_hfsm
#endif  // BICA_GUI_TRANSITION_H
