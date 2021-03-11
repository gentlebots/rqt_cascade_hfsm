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

#ifndef BICA_GUI_HFSMSCENE_H
#define BICA_GUI_HFSMSCENE_H

#include <vector>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <filesystem>

#include <QGraphicsScene>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QPointF>
#include <QMessageBox>
#include <QInputDialog>
#include <QGraphicsView>
#include <QMenu>
#include <QFileDialog>

#include <QtXml/QtXml>
#include <QTextStream>

#include "rqt_cascade_hfsm/State.hpp"
#include "rqt_cascade_hfsm/Component.hpp"
#include "rqt_cascade_hfsm/Dependency.hpp"
#include "rqt_cascade_hfsm/Transition.hpp"

#include "std_msgs/msg/string.hpp"

namespace rqt_cascade_hfsm
{
class HfsmScene : public QGraphicsScene
{
public:
  explicit HfsmScene(QObject* parent = 0);
  virtual ~HfsmScene();

  void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);

  State* addState(std::string id, int x, int y);
  void addComponent(std::string id, int x, int y);
  void addDependency(State * state, Component * comp);
  void addTransition(State * src, State * dst);

  void resizeEvent(QResizeEvent * ev);
  void showEvent(QShowEvent * ev);
  void fitView();

  void stateCB(const std_msgs::msg::String::SharedPtr msg);

private:
  void get_all(
    const std::filesystem::path & root, const std::string & ext,
    std::vector<std::filesystem::path> & ret);
  std::string getFullPath(const std::filesystem::path & root, const std::string & filename);

  void voidContextMenu(QGraphicsSceneMouseEvent * mouseEvent);
  void stateContextMenu(QGraphicsSceneMouseEvent * mouseEvent, State * state);
  void componentContextMenu(QGraphicsSceneMouseEvent * mouseEvent, Component * component);
  void dependencyContextMenu(QGraphicsSceneMouseEvent * mouseEvent, Dependency * dependency);
  void transitionContextMenu(QGraphicsSceneMouseEvent * mouseEvent, Transition * transition);

  void addStateMenu(QGraphicsSceneMouseEvent * mouseEvent);
  void addComponentMenu(QGraphicsSceneMouseEvent * mouseEvent);

  bool existState(std::string id);
  bool existComponent(std::string id);

  bool removeState(State * state);
  bool removeComponent(Component * component);
  bool removeDependency(Dependency * dependency);
  bool removeTransition(Transition * transition);

  void clearAll();
  void save(const std::string & path, const std::string & id);
  void load(const std::string & filename);

  void saveDialog();
  void loadDialog();

  void generateBica(const std::string & file);

  std::string getFileContent(const std::string & file);
  void replaceAll(std::string & str, const std::string & from, const std::string & to);
  std::string getFunctionsFromStates();
  std::string getFunctionsFromTransitions();
  std::string getActivFunctionsFromStates();
  std::string getStates();
  void generateHeader(const std::string & file);

  State* getInitial();
  std::string getStateIteration(State * state);
  std::string getAllStateIterations();
  std::string getTransitionCodeTo(State * state, bool short_indent = false);
  std::string getAllActivations();
  void generateSource(const std::string & file);

  State* getState(const std::string & id);
  Component* getComponent(const std::string & id);

  static const int IDLE = 0;
  static const int ADD_STATE = 1;
  static const int ADD_COMPONENT = 2;
  static const int ADD_DEPENDENCY = 3;
  static const int ADD_TRANSITION = 4;

  int state_;

  State* transOrigin;

  QSet<State*> states_;
  QSet<Component*> components_;
  QSet<Dependency*> dependecies_;
  QSet<Transition*> transitions_;

  std::string id_;
  std::string path_;

  bool need_path_;

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr state_sub_;
  rclcpp::Time state_ts_;
  std::string active_state_;
  rclcpp::Node::SharedPtr node_;
};

}  // namespace rqt_cascade_hfsm
#endif  // BICA_GUI_HFSMSCENE_H
