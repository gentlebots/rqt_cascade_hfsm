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

#include "rqt_cascade_hfsm/Dependency.hpp"

namespace rqt_cascade_hfsm
{
Dependency::Dependency(State * state, Component * comp)
  : QGraphicsLineItem(comp->scenePos().x(), comp->scenePos().y(), state->scenePos().x(), state->scenePos().y())
  , comp_(comp)
  , state_(state)
{
  setFlags(ItemIsSelectable);
  setZValue(-1);
  trackNodes();
}

Dependency::Dependency(const Dependency & other)
  : QGraphicsLineItem(other.comp_->scenePos().x(), other.comp_->scenePos().y(), other.state_->scenePos().x(),
                      other.state_->scenePos().y())
  , comp_(other.comp_)
  , state_(other.state_)
{
  setFlags(ItemIsSelectable);

  trackNodes();
}

Dependency::~Dependency()
{
  comp_->removeActivation(this);
  state_->removeActivation(this);
}

void Dependency::trackNodes()
{
  QPen pen;
  pen.setColor(QColor(0, 0, 255));
  pen.setStyle(Qt::DashLine);
  pen.setWidth(5);

  setPen(pen);

  setLine(QLineF(state_->pos(), comp_->pos()));
}

QRectF Dependency::boundingRect() const
{
  qreal extra = (pen().width() + 20) / 2.0;

  return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(), line().p2().y() - line().p1().y()))
      .normalized()
      .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Dependency::shape() const
{
  QPainterPath path = QGraphicsLineItem::shape();
  return path;
}
};  // namespace rqt_cascade_hfsm
