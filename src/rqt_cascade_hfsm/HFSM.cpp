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
#include <list>

#include "rqt_cascade_hfsm/HFSM.hpp"


namespace rqt_cascade_hfsm
{
HFSM::HFSM()
{
}

HFSM::~HFSM()
{
}

bool HFSM::isElement(int x, int y)
{
  return isState(x, y) || isTransition(x, y);
}

bool HFSM::isState(int x, int y)
{
  for (std::list<State>::iterator it = states_.begin(); it != states_.end(); ++it) {
    if (it->isInPos(x, y)) {
      return true;
    }
  }
  return false;
}

bool HFSM::isTransition(int x, int y)
{
  for (std::list<Transition>::iterator it = transitions_.begin(); it != transitions_.end(); ++it) {
    if (it->isInPos(x, y)) {
      return true;
    }
  }
  return false;
}

bool HFSM::existState(const std::string & id)
{
  for (std::list<State>::iterator it = states_.begin(); it != states_.end(); ++it) {
    if (it->getId() == id) {
      return true;
    }
  }
  return false;
}

void HFSM::addState(std::string id, int x, int y)
{
  std::cerr << "Adding State [" << id << "]" << std::endl;
  State newState(id, x, y);
  states_.push_back(newState);
}

void HFSM::draw(QGraphicsScene& scene)
{
  for (std::list<State>::iterator it = states_.begin(); it != states_.end(); ++it) {
    fprintf(stderr, "Drawing State [%s]", it->getId().c_str());
    it->draw(scene);
  }
  for (std::list<Transition>::iterator it = transitions_.begin(); it != transitions_.end(); ++it) {
    fprintf(stderr, "Drawing Transition [%s]", it->getId().c_str());
    it->draw(scene);
  }
}

}  // namespace rqt_cascade_hfsm
