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


#include "rqt_cascade_hfsm/Transition.hpp"


namespace rqt_cascade_hfsm
{
Transition::Transition(State * src, State * dst) : QGraphicsPolygonItem(), src_(src), dst_(dst)
{
  QGraphicsPolygonItem::setFlags(ItemIsSelectable);
  QGraphicsPolygonItem::setZValue(-1);
  trackNodes();
}

Transition::Transition(const Transition & other) : QGraphicsPolygonItem(), src_(other.src_), dst_(other.dst_)
{
  QGraphicsPolygonItem::setFlags(ItemIsSelectable);
  QGraphicsPolygonItem::setZValue(-1);
  trackNodes();
}

Transition::~Transition()
{
  src_->removeTransition(this);
  dst_->removeTransition(this);
}

void Transition::trackNodes()
{
  QPen pen;
  pen.setColor(Qt::red);
  pen.setStyle(Qt::SolidLine);
  pen.setWidth(5);

  setPen(pen);

  QPointF intersectPoint;
  intersectPoint = (src_->pos() + dst_->pos()) / 2;

  QLineF centerLine(src_->pos(), dst_->pos());

  qreal arrowSize = 20;
  double angle = ::acos(centerLine.dx() / centerLine.length());
  if (centerLine.dy() >= 0) {
    angle = (M_PI * 2) - angle;
  }

  QPointF arrowP1 = intersectPoint - QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
  QPointF arrowP2 =
      intersectPoint - QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);

  arrowHead.clear();

  arrowHead << centerLine.p1() << dst_->pos() << centerLine.p1();
  arrowHead << intersectPoint << arrowP1 << arrowP2 << intersectPoint;

  setPolygon(arrowHead);
}

}  // namespace rqt_cascade_hfsm
