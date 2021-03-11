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

#include "rqt_cascade_hfsm/Component.hpp"

#include "ament_index_cpp/get_package_share_directory.hpp"

namespace rqt_cascade_hfsm
{
Component::Component(std::string id) : QGraphicsItem(), id_(id)
{
  setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

  std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");
  
  ComponentPxmp_ = new QPixmap(QString::fromStdString(path + "/resource/Component.png"));
  ComponentActPxmp_ = new QPixmap(QString::fromStdString(path + "/resource/ComponentAct.png"));
}

Component::~Component()
{
  delete ComponentPxmp_;
  delete ComponentActPxmp_;
}

QRectF Component::boundingRect() const
{
  return QRectF(-ComponentPxmp_->width() / 2, -ComponentPxmp_->height() / 2, ComponentPxmp_->width(),
                ComponentPxmp_->height());
}

void Component::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  (void)option;
  (void)widget;
  QPointF pos(-ComponentPxmp_->width() / 2, -ComponentPxmp_->height() / 2);

  painter->drawPixmap(pos, *ComponentPxmp_);

  QRectF myRect(-ComponentPxmp_->width() / 2, -ComponentPxmp_->height() / 2, ComponentPxmp_->width(),
                ComponentPxmp_->height());
  painter->drawText(myRect, Qt::AlignCenter | Qt::TextWrapAnywhere, QString::fromStdString(id_));
}

QVariant Component::itemChange(GraphicsItemChange change, const QVariant& value)
{
  foreach(Dependency* dep, activations_)
    dep->trackNodes();

  return QGraphicsItem::itemChange(change, value);
}

void Component::addActivation(Dependency* dep)
{
  activations_.insert(dep);
}

void Component::removeActivation(Dependency* dep)
{
  activations_.remove(dep);
}

}  // namespace rqt_cascade_hfsm
