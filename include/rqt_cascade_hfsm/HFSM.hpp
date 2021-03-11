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

#ifndef BICA_GUI_HFSM_H
#define BICA_GUI_HFSM_H

#include <list>
#include <iostream>
#include <string>

#include <QGraphicsScene>

#include "rqt_cascade_hfsm/State.hpp"
#include "rqt_cascade_hfsm/Transition.hpp"

namespace rqt_cascade_hfsm
{
class HFSM
{
public:
  HFSM();
  ~HFSM();

  bool isElement(int x, int y);
  bool isState(int x, int y);
  bool isTransition(int x, int y);

  bool existState(const std::string& id);

  void addState(std::string id, int x, int y);

  void draw(QGraphicsScene & scene);

private:
  std::list<State> states_;
  std::list<Transition> transitions_;
};
}  // namespace rqt_cascade_hfsm

#endif  // BICA_GUI_HFSM_H
