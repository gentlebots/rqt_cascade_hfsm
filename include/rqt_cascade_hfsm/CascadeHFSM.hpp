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

#ifndef BICA_GUI_VICODE_H
#define BICA_GUI_VICODE_H

#include "rqt_gui_cpp/plugin.h"
#include "ui_cascade_hfsm.h"

#include <QWidget>
#include <QPoint>
#include <QMenu>
#include <QInputDialog>


#include <QMessageBox>
#include <QString>

#include "rqt_cascade_hfsm/HFSMScene.hpp"
#include "rqt_cascade_hfsm/HFSM.hpp"

namespace rqt_cascade_hfsm
{
class CascadeHFSM : public rqt_gui_cpp::Plugin
{
  Q_OBJECT
public:
  CascadeHFSM();
  virtual void initPlugin(qt_gui_cpp::PluginContext& context);
  virtual void shutdownPlugin();

  virtual void saveSettings(
    qt_gui_cpp::Settings & plugin_settings,
    qt_gui_cpp::Settings & instance_settings) const;
  virtual void restoreSettings(
    const qt_gui_cpp::Settings& plugin_settings,
    const qt_gui_cpp::Settings& instance_settings);

  rclcpp::Node::SharedPtr getNode() {return node_;}

private:
  Ui::CascadeHFSMWidget ui_;
  QWidget * widget_;
  HfsmScene * scene_;
};
}  // namespace rqt_cascade_hfsm
#endif  // BICA_GUI_VICODE_H
