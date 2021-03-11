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


#ifndef BICA_GUI_COMPONENT_H
#define BICA_GUI_COMPONENT_H

#include <string>
#include <list>

#include <QGraphicsItem>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QPointF>

#include "rqt_cascade_hfsm/Dependency.hpp"

namespace rqt_cascade_hfsm
{
class Dependency;

class Component : public QGraphicsItem
{
public:
  explicit Component(std::string id);
  ~Component();

  std::string getId() const
  {
    return id_;
  }

  void addActivation(Dependency * dep);
  void removeActivation(Dependency * dep);
  QSet<Dependency*>& getActivations()
  {
    return activations_;
  }

  QRectF boundingRect() const;
  void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant& value);

  static const int RADIUS = 10;

private:
  QPixmap * ComponentPxmp_;
  QPixmap * ComponentActPxmp_;
  std::string id_;

  QSet<Dependency*> activations_;
};
}  // namespace rqt_cascade_hfsm
#endif  // BICA_GUI_COMPONENT_H
