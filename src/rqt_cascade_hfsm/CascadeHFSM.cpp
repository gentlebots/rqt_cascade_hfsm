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


#include "rqt_cascade_hfsm/CascadeHFSM.hpp"

#include "pluginlib/class_list_macros.hpp"

#include <QStringList>

namespace rqt_cascade_hfsm
{
CascadeHFSM::CascadeHFSM() : rqt_gui_cpp::Plugin(), widget_(0)
{
  setObjectName("CascadeHFSM");
}

void CascadeHFSM::initPlugin(qt_gui_cpp::PluginContext& context)
{
  QStringList argv = context.argv();
  widget_ = new QWidget();
  ui_.setupUi(widget_);

  scene_ = new HfsmScene(this);
  ui_.graphicsView->setScene(scene_);
  ui_.graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
  ui_.graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
  ui_.graphicsView->setContextMenuPolicy(Qt::ActionsContextMenu);
  ui_.graphicsView->setMouseTracking(true);

  context.addWidget(widget_);
}

void CascadeHFSM::saveSettings(qt_gui_cpp::Settings & plugin_settings, qt_gui_cpp::Settings & instance_settings) const
{
  (void)plugin_settings;
  (void)instance_settings;
}

void CascadeHFSM::restoreSettings(const qt_gui_cpp::Settings& plugin_settings, const qt_gui_cpp::Settings& instance_settings)
{
  (void)plugin_settings;
  (void)instance_settings;
}

void CascadeHFSM::shutdownPlugin()
{
}

}  // namespace rqt_cascade_hfsm
PLUGINLIB_EXPORT_CLASS(rqt_cascade_hfsm::CascadeHFSM, rqt_gui_cpp::Plugin)
