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
#include <vector>
#include <algorithm>

#include "ament_index_cpp/get_package_share_directory.hpp"

#include "rqt_cascade_hfsm/HFSMScene.hpp"
#include "rqt_cascade_hfsm/CascadeHFSM.hpp"

using std::placeholders::_1;

namespace rqt_cascade_hfsm
{
HfsmScene::HfsmScene(QObject * parent)
  : QGraphicsScene(parent), state_(IDLE), id_(""), need_path_(false), active_state_("")
{
  char* ros_env = getenv("COLCON_PREFIX_PATH");
  std::string ros_env_str(ros_env);

  path_ = ros_env_str.substr(0, ros_env_str.size() - 8);

  node_ = dynamic_cast<CascadeHFSM*>(parent)->getNode();
  state_sub_ = node_->create_subscription<std_msgs::msg::String>(
    id_ + "/state", 1, std::bind(&HfsmScene::stateCB, this, _1));
}

HfsmScene::~HfsmScene()
{
}

void HfsmScene::resizeEvent(QResizeEvent* ev)
{
  (void)ev;
}

void showEvent(QShowEvent* ev)
{
  (void)ev;
}

void HfsmScene::fitView()
{
}

void HfsmScene::dependencyContextMenu(QGraphicsSceneMouseEvent * mouseEvent, Dependency * dependency)
{
  QMenu depMenu;

  depMenu.addAction("Remove");
  QPoint globalPos = mouseEvent->screenPos();

  QAction* selectedItem = depMenu.exec(globalPos);

  if (selectedItem) {
    removeDependency(dependency);
    state_ = IDLE;
  } else {
    state_ = IDLE;
  }
}

void HfsmScene::transitionContextMenu(QGraphicsSceneMouseEvent * mouseEvent, Transition * transition)
{
  QMenu depMenu;

  depMenu.addAction("Remove");
  QPoint globalPos = mouseEvent->screenPos();

  QAction* selectedItem = depMenu.exec(globalPos);

  if (selectedItem) {
    removeTransition(transition);
    state_ = IDLE;
  } else {
    state_ = IDLE;
  }
}

void HfsmScene::stateContextMenu(QGraphicsSceneMouseEvent * mouseEvent, State * state)
{
  QMenu stateMenu;

  stateMenu.addAction("Rename");
  stateMenu.addAction("Remove");
  stateMenu.addAction("Add Transition");
  stateMenu.addAction("Add Dependency");
  stateMenu.addAction("Set initial");

  QPoint globalPos = mouseEvent->screenPos();

  QAction* selectedItem = stateMenu.exec(globalPos);

  if (selectedItem) {
    if (selectedItem->text().toStdString() == "Rename") {
      QInputDialog getIdDialog;
      getIdDialog.setWindowTitle("Rename state");
      getIdDialog.setLabelText("Select Id");
      getIdDialog.move(mouseEvent->screenPos());
      int result = getIdDialog.exec();

      if (result == QDialog::Accepted) {
        std::string id = getIdDialog.textValue().toUtf8().constData();

        if (!existState(id)) {
          state->setId(id);
        } else {
          QMessageBox dialog;
          dialog.setWindowTitle("State exist");
          dialog.setText("State already exist");
          dialog.exec();
        }
      }
      state_ = IDLE;
    } else if (selectedItem->text().toStdString() == "Remove") {
      removeState(state);

      state_ = IDLE;
    } else if (selectedItem->text().toStdString() == "Add Dependency") {
      std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");
      this->views().first()->setCursor(QCursor(QPixmap(QString::fromStdString(path + "/resource/Connection.png"))));

      transOrigin = state;
      state_ = ADD_DEPENDENCY;
    } else if (selectedItem->text().toStdString() == "Add Transition") {
      std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");
      this->views().first()->setCursor(
          QCursor(QPixmap(QString::fromStdString(path + "/resource/NodeTransitionSmall.png"))));

      transOrigin = state;
      state_ = ADD_TRANSITION;
    } else if (selectedItem->text().toStdString() == "Set initial") {
      for (State * stateit : states_) {
        stateit->setInitial(false);
      }
      state->setInitial(true);

      state_ = IDLE;
    } else {
      state_ = IDLE;
    }
  } else {
    state_ = IDLE;
  }
}

void HfsmScene::componentContextMenu(QGraphicsSceneMouseEvent * mouseEvent, Component * component)
{
  QMenu stateMenu;

  stateMenu.addAction("Remove");
  stateMenu.addAction("Load");

  QPoint globalPos = mouseEvent->screenPos();

  QAction* selectedItem = stateMenu.exec(globalPos);

  if (selectedItem) {
    if (selectedItem->text().toStdString() == "Remove") {
      removeComponent(component);
      state_ = IDLE;
    } else if (selectedItem->text().toStdString() == "Load") {
      std::filesystem::path path(path_);

      std::string fullPath = getFullPath(path_, component->getId() + ".bica");

      if (fullPath != "") {
        clearAll();
        need_path_ = true;
        load(fullPath);
      }
      state_ = IDLE;
    } else {
      state_ = IDLE;
    }
  } else {
    state_ = IDLE;
  }
}

bool HfsmScene::removeState(State * state)
{
  this->removeItem(state);
  states_.remove(state);

  if (state->isInitial() && states_.size() > 0) {
    (*states_.begin())->setInitial(true);
  }

  for (Dependency * dep : state->getActivations()) {
    this->removeItem(dep);
    delete dep;
  }

  for (Transition * tran : state->getTransition()) {
    this->removeItem(tran);
    delete tran;
  }

  if (state != NULL) {
    delete state;
  }
  
  return true;
}

bool HfsmScene::removeDependency(Dependency * dependency)
{
  this->removeItem(dependency);
  dependecies_.remove(dependency);
  delete dependency;

  return true;
}

bool HfsmScene::removeTransition(Transition * transition)
{
  this->removeItem(transition);
  transitions_.remove(transition);

  delete transition;

  return true;
}

bool HfsmScene::removeComponent(Component * component)
{
  this->removeItem(component);
  components_.remove(component);

  for (Dependency* dep : component->getActivations()) {
    this->removeItem(dep);
    delete dep;
  }

  if (component != NULL) {
    delete component;
  }

  return true;
}

void HfsmScene::voidContextMenu(QGraphicsSceneMouseEvent * mouseEvent)
{
  QMenu voidMenu;
  voidMenu.addAction("Add state");
  voidMenu.addAction("Add component");
  voidMenu.addAction("---");
  voidMenu.addAction("New component");
  voidMenu.addAction("Load...");
  voidMenu.addAction("Save...");
  voidMenu.addAction("Quick Save");

  std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");

  QPoint globalPos = mouseEvent->screenPos();  // this->views().first()->mapToGlobal(mouseEvent->scenePos());
  QAction* selectedItem = voidMenu.exec(globalPos);

  if (selectedItem) {
    if (selectedItem->text().toStdString() == "Add state") {
      this->views().first()->setCursor(QCursor(QPixmap(QString::fromStdString(path + "/resource/RegularState.png"))));
      state_ = ADD_STATE;
    } else if (selectedItem->text().toStdString() == "Add component") {
      this->views().first()->setCursor(QCursor(QPixmap(QString::fromStdString(path + "/resource/Component.png"))));
      state_ = ADD_COMPONENT;
    } else if (selectedItem->text().toStdString() == "New component") {
      clearAll();
      state_ = IDLE;
    } else if (selectedItem->text().toStdString() == "Save...") {
      saveDialog();

      state_ = IDLE;
    } else if (selectedItem->text().toStdString() == "Quick Save") {
      if (id_ != "" && !need_path_) {
        save(path_.c_str(), id_);
      } else {
        QMessageBox dialog;
        dialog.setWindowTitle("No Id");
        dialog.setText("Save first for selecting Component Id");
        dialog.exec();
      }
      state_ = IDLE;
    } else if (selectedItem->text().toStdString() == "Load...") {
      loadDialog();

      state_ = IDLE;
    } else {
      state_ = IDLE;
    }
  } else {
    state_ = IDLE;
  }
}

void HfsmScene::addStateMenu(QGraphicsSceneMouseEvent * mouseEvent)
{
  QInputDialog getIdDialog;
  getIdDialog.setWindowTitle("add State");
  getIdDialog.setLabelText("Select Id");
  getIdDialog.move(mouseEvent->screenPos());
  int result = getIdDialog.exec();

  if (result == QDialog::Accepted)
  {
    std::string id = getIdDialog.textValue().toUtf8().constData();

    if (!existState(id))
      addState(id, mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
    else
    {
      QMessageBox dialog;
      dialog.setWindowTitle("State exist");
      dialog.setText("State already exist");
      dialog.exec();
    }
  }
}

bool HfsmScene::existState(std::string id)
{
  QSet<State*>::iterator it;
  for (it = states_.begin(); it != states_.end(); ++it) {
    if ((*it)->getId() == id) {
      return true;
    }
  }

  return false;
}

bool HfsmScene::existComponent(std::string id)
{
  QSet<Component*>::iterator it;
  for (it = components_.begin(); it != components_.end(); ++it) {
    if ((*it)->getId() == id) {
      return true;
    }
  }

  return false;
}

void HfsmScene::addComponentMenu(QGraphicsSceneMouseEvent * mouseEvent)
{
  QMenu compsMenu;

  compsMenu.addAction("Add manual...");

  QPoint globalPos = mouseEvent->screenPos();
  QAction* selectedItem = compsMenu.exec(globalPos);

  if (selectedItem) {
    std::string id;
    if (selectedItem->text().toStdString() == "Add manual...") {
      QInputDialog getIdDialog;
      getIdDialog.setWindowTitle("Add manual");
      getIdDialog.setLabelText("Select Id");
      getIdDialog.move(mouseEvent->screenPos());
      int result = getIdDialog.exec();

      if (result == QDialog::Accepted) {
        std::string id = getIdDialog.textValue().toUtf8().constData();
        if (!existComponent(id)) {
          addComponent(id, mouseEvent->scenePos().x(), mouseEvent->scenePos().y());
        } else {
          QMessageBox dialog;
          dialog.setWindowTitle("Component exist");
          dialog.setText("Component already exist");
          dialog.exec();
        }
      }
    }
  }
}

void HfsmScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  QList<QGraphicsItem*> selected_items =
      items(mouseEvent->scenePos(), Qt::IntersectsItemBoundingRect, Qt::DescendingOrder);

  State* currentState = 0;
  Component* currentComp = 0;
  Dependency* currentDep = 0;
  Transition* currentTran = 0;

  if (selected_items.size() > 0) {
    currentState = dynamic_cast<State*>(*(selected_items.begin()));
    currentComp = dynamic_cast<Component*>(*(selected_items.begin()));
    currentDep = dynamic_cast<Dependency*>(*(selected_items.begin()));
    currentTran = dynamic_cast<Transition*>(*(selected_items.begin()));
  }

  switch (state_) {
    case IDLE:
      if ((mouseEvent->button() == Qt::RightButton) && (currentState != 0)) {
        stateContextMenu(mouseEvent, currentState);
      } else if ((mouseEvent->button() == Qt::RightButton) && (currentComp != 0)) {
        componentContextMenu(mouseEvent, currentComp);
      } else if ((mouseEvent->button() == Qt::RightButton) && (currentDep != 0)) {
        dependencyContextMenu(mouseEvent, currentDep);
      } else if ((mouseEvent->button() == Qt::RightButton) && (currentTran != 0)) {
        transitionContextMenu(mouseEvent, currentTran);
      } else if ((mouseEvent->button() == Qt::RightButton)) {
        voidContextMenu(mouseEvent);
      } else {
        state_ = IDLE;
      }

      break;

    case ADD_STATE:

      if ((mouseEvent->button() == Qt::LeftButton) && (currentState == 0)) {
        addStateMenu(mouseEvent);

        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      } else {
        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      }
      break;

    case ADD_COMPONENT:

      if ((mouseEvent->button() == Qt::LeftButton) && (currentState == 0)) {
        addComponentMenu(mouseEvent);

        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      } else {
        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      }
      break;

    case ADD_DEPENDENCY:

      if ((mouseEvent->button() == Qt::LeftButton) && (currentComp != 0)) {
        addDependency(transOrigin, currentComp);

        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      } else {
        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      }

      break;

    case ADD_TRANSITION:

      if ((mouseEvent->button() == Qt::LeftButton) && (currentState != 0)) {
        addTransition(transOrigin, currentState);

        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      } else {
        this->views().first()->setCursor(Qt::ArrowCursor);
        state_ = IDLE;
      }

      break;
  }

  QGraphicsScene::mousePressEvent(mouseEvent);
}

State* HfsmScene::addState(std::string id, int x, int y)
{
  State* newState = new State(id, node_);

  newState->setX(x);
  newState->setY(y);

  if (states_.empty()) {
    newState->setInitial(true);
  }

  states_.insert(newState);

  addItem(newState);

  return newState;
}

void HfsmScene::addComponent(std::string id, int x, int y)
{
  Component* newComp = new Component(id);

  newComp->setX(x);
  newComp->setY(y);

  components_.insert(newComp);
  addItem(newComp);
}

void HfsmScene::addDependency(State * state, Component * comp)
{
  QSet<Dependency*>::iterator it;
  for (it = dependecies_.begin(); it != dependecies_.end(); ++it) {
    if (((*it)->getState() == state) && ((*it)->getComponent() == comp)) {
      QMessageBox dialog;
      dialog.setWindowTitle("Dependency exist");
      dialog.setText("Dependency already exist");
      dialog.exec();

      return;
    }
  }

  Dependency* newDep = new Dependency(state, comp);
  dependecies_.insert(newDep);
  state->addActivation(newDep);
  comp->addActivation(newDep);
  addItem(newDep);
}

void HfsmScene::addTransition(State * src, State * dst)
{
  QSet<Transition*>::iterator it;
  for (it = transitions_.begin(); it != transitions_.end(); ++it) {
    if (((*it)->getSrcState() == src) && ((*it)->getDestinyState() == dst)) {
      QMessageBox dialog;
      dialog.setWindowTitle("Transition exist");
      dialog.setText("Transition already exist");
      dialog.exec();

      return;
    }
  }

  if (src == dst) {
    QMessageBox dialog;
    dialog.setWindowTitle("Same states");
    dialog.setText("Source and Destiny are the same");
    dialog.exec();

    return;
  }

  Transition* newTran = new Transition(src, dst);

  transitions_.insert(newTran);
  src->addTransition(newTran);
  dst->addTransition(newTran);
  addItem(newTran);
}

void HfsmScene::get_all(const std::filesystem::path & root, const std::string & ext,
                        std::vector<std::filesystem::path> & ret)
{
  if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root)) {
    return;
  }

  std::filesystem::recursive_directory_iterator it(root, std::filesystem::directory_options::follow_directory_symlink);
  std::filesystem::recursive_directory_iterator endit;

  while (it != endit) {
    if (std::filesystem::is_regular_file(*it) && it->path().extension() == ext) {
      ret.push_back(it->path().filename());
    }
    ++it;
  }
}

std::string HfsmScene::getFullPath(const std::filesystem::path & root, const std::string & filename)
{
  if (!std::filesystem::exists(root) || !std::filesystem::is_directory(root)) {
    return "";
  }

  std::filesystem::recursive_directory_iterator it(root, std::filesystem::directory_options::follow_directory_symlink);
  std::filesystem::recursive_directory_iterator endit;

  while (it != endit) {
    if (std::filesystem::is_regular_file(*it) && it->path().filename() == filename) {
      return it->path().string();
    }
    ++it;
  }

  return "";
}

void HfsmScene::clearAll()
{
  for (Dependency * dep : dependecies_) {
    delete dep;
  }
  dependecies_.clear();

  for (Transition * tran : transitions_) {
    delete tran;
  }
  transitions_.clear();

  for (State * state : states_) {
    delete state;
  }
  states_.clear();

  for (Component * comp : components_) {
    delete comp;
  }
  components_.clear();

  this->clear();
}

void HfsmScene::save(const std::string & path, const std::string & id)
{
  generateBica(path + "/" + id + ".bica");
  generateHeader(path + "/" + id + ".hpp");
  generateSource(path + "/" + id + ".cpp");
}

void HfsmScene::load(const std::string& filename)
{
  clearAll();

  QFile file(filename.c_str());

  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox dialog;
    dialog.setWindowTitle("Fail to open");
    dialog.setText("Failing opening file");
    dialog.exec();
    return;
  }

  QByteArray content = file.readAll();
  QDomDocument doc;

  QString errorMessage;
  int line, col;

  if (!doc.setContent(content, &errorMessage, &line, &col)) {
    QMessageBox dialog;
    dialog.setWindowTitle("Fail to open");
    dialog.setText("Error openning. More info in stderr");

    std::cerr << "Error in Line " << line << ", column " << col << ":" <<
      qPrintable(errorMessage) << std::endl;

    dialog.exec();
    return;
  }

  QDomElement root = doc.documentElement();

  if (root.hasAttribute("id")) {
    QDomAttr attr = root.attributeNode("id");
    id_ = attr.value().toStdString();
    state_sub_ = node_->create_subscription<std_msgs::msg::String>(
      id_ + "/state", 1, std::bind(&HfsmScene::stateCB, this, _1));
  }

  // 1st States and components only
  QDomNode node = root.firstChild();
  while (!node.isNull()) {
    if (node.isElement()) {
      QDomElement elem = node.toElement();

      if (elem.tagName().toStdString() == "state") {
        int x, y;
        std::string id;
        bool initial;

        x = atoi(elem.attribute("x").toStdString().c_str());
        y = atoi(elem.attribute("y").toStdString().c_str());
        id = elem.attribute("id").toStdString();
        initial = (elem.attribute("initial").toStdString() == "true") ? true : false;
 
        State* newState = addState(id, x, y);
        newState->setInitial(initial);
      }

      if (elem.tagName().toStdString() == "component") {
        int x, y;
        std::string id;

        x = atoi(elem.attribute("x").toStdString().c_str());
        y = atoi(elem.attribute("y").toStdString().c_str());
        id = elem.attribute("id").toStdString();

        addComponent(id, x, y);
      }
    }
    node = node.nextSibling();
  }

  // 2nd Transitions and dependencies
  node = root.firstChild();
  while (!node.isNull()) {
    if (node.isElement()) {
      QDomElement elem = node.toElement();

      if (elem.tagName().toStdString() == "state") {
        std::string id;
        id = elem.attribute("id").toStdString();

        State* currstate = getState(id);

        if (elem.hasChildNodes()) {
          QDomNode activations = elem.firstChild();
          while (!activations.isNull()) {
            if (activations.isElement()) {
              QDomElement elemact = activations.toElement();
              if (elemact.tagName().toStdString() == "activation") {
                std::string actid;
                actid = elemact.attribute("id").toStdString();

                addDependency(currstate, getComponent(actid));
              }
            }

            activations = activations.nextSibling();
          }
        }
      }

      if (elem.tagName().toStdString() == "transition") {
        std::string src, dst;
        src = elem.attribute("src").toStdString();
        dst = elem.attribute("dst").toStdString();

        addTransition(getState(src), getState(dst));
      }
    }
    node = node.nextSibling();
  }
}

State * HfsmScene::getState(const std::string & id)
{
  for (State * state : states_) {
    if (state->getId() == id) {
      return state;
    }
  }

  std::cerr << "State not found [" << id << "]" << std::endl;

  return NULL;
}

Component* HfsmScene::getComponent(const std::string& id)
{
  for (Component * component : components_) {
    if (component->getId() == id) {
      return component;
    }
  }

  std::cerr << "Component not found [" << id << "]" << std::endl;

  return NULL;
}

void HfsmScene::loadDialog()
{
  QFileDialog fileDialog;
  fileDialog.setNameFilter(tr("Images (*.bica)"));
  fileDialog.setDirectory(path_.c_str());

  int fileResult = fileDialog.exec();

  if (fileResult != QDialog::Accepted) {
    return;
  }

  need_path_ = true;

  load((*fileDialog.selectedFiles().begin()).toStdString());
}

void HfsmScene::saveDialog()
{
  QInputDialog getIdDialog;
  getIdDialog.setWindowTitle("Component Id");
  getIdDialog.setLabelText("Select Id for this component");
  getIdDialog.setTextValue(id_.c_str());

  int result = getIdDialog.exec();

  if (result == QDialog::Accepted) {
    id_ = getIdDialog.textValue().toStdString();
    state_sub_ = node_->create_subscription<std_msgs::msg::String>(
      id_ + "/state", 1, std::bind(&HfsmScene::stateCB, this, _1));
  } else {
    return;
  }

  QFileDialog dirDialog;
  dirDialog.setFileMode(QFileDialog::DirectoryOnly);
  dirDialog.setDirectory(path_.c_str());

  int dirResult = dirDialog.exec();

  if (dirResult != QDialog::Accepted)
    return;

  path_ = (*dirDialog.selectedFiles().begin()).toStdString();

  need_path_ = false;

  save(path_.c_str(), id_);
}

void HfsmScene::generateBica(const std::string & file)
{
  QDomDocument doc;
  QDomElement componentRoot = doc.createElement("component");
  componentRoot.setAttribute("id", id_.c_str());

  for (State * state : states_) {
    QDomElement stateElem = doc.createElement("state");
    stateElem.setAttribute("id", state->getId().c_str());
    stateElem.setAttribute("x", state->x());
    stateElem.setAttribute("y", state->y());
    stateElem.setAttribute("initial", state->isInitial() ? "true" : "false");

    for (Dependency * dep : state->getActivations()) {
      QDomElement compElem = doc.createElement("activation");
      compElem.setAttribute("id", dep->getComponent()->getId().c_str());
      stateElem.appendChild(compElem);
    }
    componentRoot.appendChild(stateElem);
  }

  for (Component * component : components_) {
    QDomElement compElem = doc.createElement("component");
    compElem.setAttribute("id", component->getId().c_str());
    compElem.setAttribute("x", component->x());
    compElem.setAttribute("y", component->y());
    componentRoot.appendChild(compElem);
  }

  for (Transition * tran : transitions_) {
    QDomElement transElem = doc.createElement("transition");
    transElem.setAttribute("src", tran->getSrcState()->getId().c_str());
    transElem.setAttribute("dst", tran->getDestinyState()->getId().c_str());
    componentRoot.appendChild(transElem);
  }

  doc.appendChild(componentRoot);

  QFile data(file.c_str());
  if (data.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream out(&data);
    out << doc.toString();
  }
}

std::string HfsmScene::getFileContent(const std::string & file)
{
  std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");
  std::string fullPath = path + "/resource/" + file;

  std::ifstream t(fullPath.c_str());
  std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

  std::cerr << "reading fullPath ["  << fullPath << "]" << std::endl;
  std::cerr << "reading str ["  << str << "]" << std::endl;

  return str;
}

void HfsmScene::replaceAll(std::string & str, const std::string & from, const std::string & to)
{
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

std::string HfsmScene::getFunctionsFromStates()
{
  std::string ret;

  for (State * state : states_) {
    ret += "  virtual void " + state->getId() + "_code_iterative() {}\n";
    ret += "  virtual void " + state->getId() + "_code_once() {}\n";
  }

  return ret;
}

std::string HfsmScene::getFunctionsFromTransitions()
{
  std::string ret;

  for (Transition * tran : transitions_) {
    ret += "  virtual bool " + tran->getSrcState()->getId() + "_2_" +
      tran->getDestinyState()->getId() + "() {return false;}\n";
  }

  return ret;
}

std::string HfsmScene::getStates()
{
  std::string ret;
  int counter = 0;

  for (State * state : states_) {
    std::string id_upper = state->getId();
    std::transform(id_upper.begin(), id_upper.end(),id_upper.begin(), toupper);

    ret += "  static const int " + id_upper + " = " +
      std::to_string(counter++) + ";\n";
  }

  return ret;
}

std::string HfsmScene::getActivFunctionsFromStates()
{
  std::string ret;

  ret += "  void deactivateAllDeps();\n";

  for (State * state : states_) {
    ret += "  void " + state->getId() + "_activateDeps();\n";
  }

  return ret;
}

void HfsmScene::generateHeader(const std::string & file)
{
  std::string path = ament_index_cpp::get_package_share_directory("rqt_cascade_hfsm");
  std::string pattern = getFileContent("patternSrc.hpp");

  std::string id_upper = id_;
  std::transform(id_upper.begin(), id_upper.end(),id_upper.begin(), toupper);

  replaceAll(pattern, "IDM", id_upper);
  replaceAll(pattern, "ID", id_);
  replaceAll(pattern, "FUNCTIONS_STATES", getFunctionsFromStates());
  replaceAll(pattern, "FUNCTIONS_TRANSITIONS", getFunctionsFromTransitions());
  replaceAll(pattern, "FUNCTIONS_ACTS", getActivFunctionsFromStates());
  replaceAll(pattern, "STATES", getStates());

  std::ofstream out(file);
  out << pattern;
  out.close();
}

State * HfsmScene::getInitial()
{
  for (State * state : states_) {
    if (state->isInitial()) {
      return state;
    }
  }

  std::cerr << "Not initial found" << std::endl;

  return NULL;
}

std::string HfsmScene::getStateIteration(State* state)
{
  std::string ret;

  std::string id_upper = state->getId();
  std::transform(id_upper.begin(), id_upper.end(),id_upper.begin(), toupper);

  ret += "    case " + id_upper + ":\n";
  ret += "      " + state->getId() + "_code_iterative();\n\n";

  ret += "      msg.data = \"" + state->getId() + "\";\n";
  ret += "      state_pub_->publish(msg);\n\n";

  for (Transition* tran : state->getTransition()) {
    if (state == tran->getSrcState()) {
      ret += "      if (" + tran->getSrcState()->getId() + "_2_" + 
        tran->getDestinyState()->getId() + "()) {\n";
      ret += getTransitionCodeTo(tran->getDestinyState()) + "      }\n";
    }
  }

  ret += "      break;\n";

  return ret;
}

std::string HfsmScene::getTransitionCodeTo(State * state, bool short_indent)
{
  std::string ret;

  std::string id_upper = state->getId();
  std::transform(id_upper.begin(), id_upper.end(),id_upper.begin(), toupper);

  std::string start = "        ";
  if (short_indent) {
    start = "  ";
  }

  ret += start + "deactivateAllDeps();\n\n";
  ret += start + "state_ = " + id_upper + ";\n";
  ret += start + "state_ts_ = now();\n\n";
  ret += start + state->getId() + "_activateDeps();\n";
  ret += start + state->getId() + "_code_once();\n";

  return ret;
}

std::string HfsmScene::getAllStateIterations()
{
  std::string ret;
  for (State* state : states_) {
    ret += getStateIteration(state);
  }
  ret.pop_back();  // remove last \n
  return ret;
}

std::string HfsmScene::getAllActivations()
{
  std::string ret;

  ret += "void\n" + id_ + "::deactivateAllDeps()\n{\n";

  for (Component* component : components_) {
    ret += "  remove_activation(\"" + component->getId() + "\");\n";
  }

  ret += "}\n\n";
  for (State* state: states_) {
    ret += "void\n" + id_ + "::" + state->getId() + "_activateDeps()\n{\n";

    for (Dependency * dependency : state->getActivations()) {
      ret += "  add_activation(\"" + dependency->getComponent()->getId() + "\");\n";
    }

    ret += "}\n";
  }

  return ret;
}

void HfsmScene::generateSource(const std::string & file)
{
  std::string pattern = getFileContent("patternSrc.cpp");

  replaceAll(pattern, "ID", id_);

  std::string id_upper = getInitial()->getId();
  std::transform(id_upper.begin(), id_upper.end(),id_upper.begin(), ::toupper);

  replaceAll(pattern, "INITIAL_STATE", id_upper);
  replaceAll(pattern, "INITIAL_ITERATION", getTransitionCodeTo(getInitial(), true));
  replaceAll(pattern, "STATES_ITERATIONS", getAllStateIterations());
  replaceAll(pattern, "STATE_ACTIVATIONS", getAllActivations());

  std::ofstream out(file);
  out << pattern;
  out.close();
}

void HfsmScene::stateCB(const std_msgs::msg::String::SharedPtr msg)
{
  State * selected = getState(msg->data);

  if (selected != NULL) {
    for (State * state : states_) {
      if (state == selected) {
        state->debugIsActive(true);
      } else {
        state->debugIsActive(false);
      }
    }
  }
}

}  // namespace rqt_cascade_hfsm
