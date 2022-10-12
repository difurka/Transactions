#include "b_plus_tree.h"
#include <fstream>
#include <algorithm>

namespace s21 {

using data_t = Storage::Element::Data;

/* -------------------------------------------------------------------------- */
/*                                  override                                  */
/* -------------------------------------------------------------------------- */

void BPlusTree::Set(element element) {
  string key_of_this_element = element.GetKey();
  Node* node_for_key = GetReferenceOfNodeForThisKeyFromNode(key_of_this_element, root_);
  if (node_for_key->HasKey(key_of_this_element) == false) {
    if (node_for_key->GetKeyelements().size() == 2 * kOrder - 1) {
        DivideOnTwoNods(node_for_key);
        node_for_key = GetReferenceOfNodeForThisKeyFromNode(key_of_this_element, root_);
        node_for_key->InsertElementInNode(element, nullptr, nullptr);
    } else {
      node_for_key->InsertElementInNode(element, nullptr, nullptr);
    }
  }
}

BPlusTree::Element BPlusTree::Get(string key) const {
  Element result_element = Element();
  Node* node_for_key = GetReferenceOfNodeForThisKeyFromNode(key, root_);
  int number_of_key = node_for_key->NumberOfKeyThatClosest(key);
  if (node_for_key->HasKey(key))
    result_element = node_for_key->GetKeyelements()[number_of_key];
  return result_element;
}

bool BPlusTree::Exists(string key) const {
  Element element = Get(key);
  if (element.GetKey() == "") return false;
  return true;
}

bool BPlusTree::Del(string key) {
  Node* node_with_key = GetReferenceOfNodeForThisKeyFromNode(key, root_);
  int number_of_key = node_with_key->NumberOfKeyThatClosest(key);
  if (node_with_key->HasKey(key)) {
    if (node_with_key->GetType() == Node::kList) {
      node_with_key->DeleteKeyFromList(key, root_);
    } else if (node_with_key->GetType() == Node::kRoot && node_with_key->GetChildren().size() == 0) {
      node_with_key->DeleteElement(key);
    } else {
      node_with_key->DeleteKeyFromNotList(number_of_key, root_);
    }
  return true;
  }
  return false;
}

bool BPlusTree::Update(string key, const data_t& data) {
  Node* node_for_key = GetReferenceOfNodeForThisKeyFromNode(key, root_);
  int number_of_key = node_for_key->NumberOfKeyThatClosest(key);
  if (node_for_key->HasKey(key)) {
    node_for_key->UpdateData(number_of_key, data);
    return true;
  }
  return false;
}

void BPlusTree::Node::UpdateData(int number, const data_t& data) {
    Element element_for_change = keyelements_[number];
    data_t data_for_change = element_for_change.GetData();
    if (data.surname != "-") data_for_change.surname = data.surname;
    if (data.name != "-") data_for_change.name = data.name;
    if (data.year_of_birth != "-") data_for_change.year_of_birth = data.year_of_birth;
    if (data.city != "-") data_for_change.city = data.city;
    if (data.coins != "-") data_for_change.coins = data.coins;
    element_for_change.SetData(data_for_change);
    keyelements_[number] = element_for_change;
}

bool BPlusTree::Rename(string key, string new_key) {
  Node* node_with_key = GetReferenceOfNodeForThisKeyFromNode(key, root_);
  int number_of_element = node_with_key->NumberOfKeyThatClosest(key);
  if (node_with_key->HasKey(key)) {
    Element element_for_rename = node_with_key->GetKeyelements()[number_of_element];
    Del(key);
    element_for_rename.SetKey(new_key);
    Set(element_for_rename);
    return true;
  }
  return false;
}

int BPlusTree::Ttl(string key) const {
  Element element_for_research = Get(key);
  int life_time = 0;
  if (element_for_research.GetKey() != "") life_time = element_for_research.GetLifeTime();
  return life_time;
}

BPlusTree::vector BPlusTree::Find(const data_t& data) const {
  std::vector<Element> vector_of_elements;
  root_->TakeAllKeyelements(&vector_of_elements, data);
  vector result;
  std::transform(vector_of_elements.begin(), vector_of_elements.end(), std::back_inserter(result),
                                            [](const Element& elem) {return elem.GetKey();});
  return result;
}

void BPlusTree::Init() {
  if (root_ != nullptr) {
    root_->DeleteNodesInTreeWithThisRoot();
    delete root_;
  }
  root_ = new Node(BPlusTree::Node::kRoot);
}

std::vector<BPlusTree::Element> BPlusTree::AllElements() const {
  std::vector<Element> result;
  root_->TakeAllKeyelements(&result, {"", "", "", "", "", Node::kNullData});
  return result;
}
/* -------------------------------------------------------------------------- */
/*                                 BPlusTree                                  */
/* -------------------------------------------------------------------------- */

BPlusTree::BPlusTree() {
  root_ = new Node(Node::kRoot);
}

BPlusTree::BPlusTree(const BPlusTree& other) {
  CopyTree(other);
}

BPlusTree::BPlusTree(BPlusTree&& other) {
  std::swap(root_, other.root_);
}

BPlusTree& BPlusTree::operator=(const BPlusTree& other) {
  CopyTree(other);
  return *this;
}

BPlusTree& BPlusTree::operator=(const BPlusTree&& other) {
  CopyTree(other);
  return *this;
}

void BPlusTree::CopyTree(const BPlusTree& other) {
  if (&other != this) {
    for (auto& element : other.AllElements()) {
      Set(element);
    }
  }
}

BPlusTree::~BPlusTree() {
  root_->DeleteNodesInTreeWithThisRoot();
  delete root_;
}

BPlusTree::Node* BPlusTree::GetReferenceOfNodeForThisKeyFromNode(string key, Node* node)  const {
  size_t number_of_closest = node->NumberOfKeyThatClosest(key);
  if (node->GetChildren().size() > 0
    && ((node->GetKeyelements().size() > number_of_closest
        && node->GetKeyelements()[number_of_closest].GetKey() != key )
        || node->GetKeyelements().size() == number_of_closest)) {
    Node* child_with_this_key = node->GetChildren()[number_of_closest];
    node = (GetReferenceOfNodeForThisKeyFromNode(key, child_with_this_key));
  }
  return node;
}

void BPlusTree::DivideOnTwoNods(Node* node) {
  std::vector<Element> keyelements_of_this_node = node->GetKeyelements();
  int number_of_closest_in_parent = 0;
  if (node->GetType() != Node::kRoot)
    number_of_closest_in_parent =
          node->GetParent()->NumberOfKeyThatClosest(keyelements_of_this_node[kOrder - 1].GetKey());
  if (node->GetType() == Node::kRoot) {
    DivideRoot(node);
  } else {
    DivideNotRoot(node);
  }
  if (node->GetChildren().size() > 0) DivideChildren(node, number_of_closest_in_parent);
  if (node->GetParent()->GetKeyelements().size() == 2 * kOrder) DivideOnTwoNods(node->GetParent());
}

void BPlusTree::DivideRoot(Node* node) {
    std::vector<Element> keyelements = node->GetKeyelements();
    Node::TypeOfNode new_type = (node->GetChildren().size() == 0) ? Node::kList : Node::kNotList;
    root_ = new Node(Node::kRoot);
    root_->SetKeyelements({keyelements[kOrder - 1]});
    node->SetType(new_type);
    root_->SetChildren({node, new Node(new_type)});
    root_->GetChildren()[1]->SetKeyelements(std::vector<Element>(keyelements.begin() + kOrder,
                                                                  keyelements.end()));
    node->SetKeyelements(std::vector<Element>(keyelements.begin(), keyelements.begin() + kOrder - 1));
    node->SetParent(root_);
    root_->GetChildren()[1]->SetParent(root_);
}

void BPlusTree::DivideNotRoot(Node* node) {
    std::vector<Element> keyelements = node->GetKeyelements();
    int number_of_closest = node->GetParent()->NumberOfKeyThatClosest(keyelements[kOrder - 1].GetKey());
    node->GetParent()->InsertElementInNode(keyelements[kOrder - 1], node, new Node(node->GetType()));
    std::vector<Element> keyelements_of_left = std::vector<Element>(keyelements.begin(),
                                                                      keyelements.begin() + kOrder - 1);
    std::vector<Element> keyelements_of_right =
                          std::vector<Element>(keyelements.begin() + kOrder, keyelements.end());
    node->GetParent()->GetChildren()[number_of_closest + 1]->SetKeyelements(keyelements_of_right);
    node->SetKeyelements(keyelements_of_left);
    node->GetParent()->GetChildren()[number_of_closest + 1]->SetParent(node->GetParent());
}

void BPlusTree::DivideChildren(Node* node, int number_of_closest) {
  std::vector<Node*> children = node->GetChildren();
  std::vector<Node*> children_of_left = std::vector<Node*>(children.begin(), children.begin() + kOrder);
  std::vector<Node*> children_of_right = std::vector<Node*>(children.begin() + kOrder, children.end());
  for (auto child : children_of_right)
    child->SetParent(node->GetParent()->GetChildren()[number_of_closest + 1]);
  node->GetParent()->GetChildren()[number_of_closest + 1]->SetChildren(children_of_right);
  node->SetChildren(children_of_left);
}

/* -------------------------------------------------------------------------- */
/*                                   Node                                     */
/* -------------------------------------------------------------------------- */

BPlusTree::Node::Node() {}

BPlusTree::Node::Node(TypeOfNode type) {
  type_ = type;
}

BPlusTree::Node::Node(const Node& other) {
  CopyNode(other);
}

BPlusTree::Node::Node(Node&& other) {
  CopyNode(other);
}
BPlusTree::Node& BPlusTree::Node::operator=(const Node& other) {
  CopyNode(other);
  return *this;
}
BPlusTree::Node& BPlusTree::Node::operator=(const Node&& other) {
  CopyNode(other);
  return *this;
}

BPlusTree::Node::~Node() {}

void BPlusTree::Node::CopyNode(const Node& other) {
  keyelements_ = other.keyelements_;
  SetChildren(other.children_);
  parent_ = other.parent_;
  type_ = other.type_;
}

void BPlusTree::Node::SetKeyelements(const std::vector<Element>& keyelements) {
  keyelements_.clear();
  for (size_t i = 0; i < keyelements.size(); ++i) {
    keyelements_.push_back(keyelements[i]);
  }
}

void BPlusTree::Node::SetChildren(const std::vector<Node*>& children) {
  children_.resize(children.size());
  for (size_t i = 0; i < children.size(); ++i)
    children_[i] = children[i];
}

void BPlusTree::Node::SetParent(Node* parent) {
  parent_ = parent;
}

void BPlusTree::Node::SetType(TypeOfNode type) {
  type_ = type;
}

std::vector<BPlusTree::Node*> BPlusTree::Node::GetChildren() const {
  return children_;
}

std::vector<BPlusTree::Element> BPlusTree::Node::GetKeyelements() const {
  return keyelements_;
}

BPlusTree::Node* BPlusTree::Node::GetParent() {
  return parent_;
}

BPlusTree::Node::TypeOfNode BPlusTree::Node::GetType() const {
  return type_;
}

void BPlusTree::Node::CleanReferencesToChildren() {
  for (auto child : children_) {
    if (child != nullptr) delete child;
    child = nullptr;
  }
}

void BPlusTree::Node::DeleteNodesInTreeWithThisRoot() {
  if (children_.size() != 0 && children_[0]->GetType() != kList) {
    for (auto child : children_) {
      child->DeleteNodesInTreeWithThisRoot();
    }
    CleanReferencesToChildren();
  } else {
    CleanReferencesToChildren();
  }
}

int BPlusTree::Node::NumberOfKeyThatClosest(string key) const {
  int count_of_keyelements = keyelements_.size();
  for (int i = 0; i < count_of_keyelements; ++i) {
    if (key <= keyelements_[i].GetKey()) return i;
  }
  return count_of_keyelements;
}

bool BPlusTree::Node::HasKey(string key) const {
  if (std::any_of(keyelements_.begin(), keyelements_.end(), [&key](const Element& elem) {
    return elem.GetKey() == key;
  })) return true;
  return false;
}

void BPlusTree::Node::InsertElementInNode(const Element& element, Node* left_child, Node* right_child) {
  int number_of_key = NumberOfKeyThatClosest(element.GetKey());
  keyelements_.insert(keyelements_.begin() + number_of_key, element);
  if (left_child && right_child) {
    children_[number_of_key] = left_child;
    children_.insert(children_.begin() + number_of_key + 1, right_child);
  }
}

void BPlusTree::Node::InsertElementInNodeAtEnd(const Element& element, Node* child) {
  int number_of_key = NumberOfKeyThatClosest(element.GetKey());
  keyelements_.insert(keyelements_.begin() + number_of_key, element);
  children_.insert(children_.begin() + number_of_key + 1, child);
}

void BPlusTree::Node::InsertElementInNodeAtBegin(const Element& element, Node* child) {
  int number_of_key = NumberOfKeyThatClosest(element.GetKey());
  keyelements_.insert(keyelements_.begin() + number_of_key, element);
  children_.insert(children_.begin() + number_of_key, child);
}

void BPlusTree::Node::TakeAllKeyelements(std::vector<Element>* result, data_t data) {
  for (auto element : keyelements_) {
    if (data.life_time == kNullData) {
      result->push_back(element);
    } else {
      if (IsDataSiutable(data, element.GetData())) result->push_back(element);
    }
  }
  for (auto child : children_)
    child->TakeAllKeyelements(result, data);
}

void BPlusTree::Node::DeleteKeyFromNotList(int number, Node* root) {
  Node* node_for_change = GetListWithClosestRightKey(number + 1);
  Element element_for_change = node_for_change->GetKeyelements()[0];
  keyelements_[number] = element_for_change;
  node_for_change->DeleteElement(element_for_change.GetKey());
  if ((int)node_for_change->GetKeyelements().size() == kOrder - 2) {
    if (node_for_change->parent_ != this) {
      node_for_change->MakeBalance(0, root);
    } else {
      node_for_change->MakeBalance(number + 1, root);
    }
  }
}

BPlusTree::Node* BPlusTree::Node::GetListWithClosestRightKey(int number) {
  Node* node = children_[number];
  while (node->type_ != kList) node = node->children_[0];
  return node;
}

void BPlusTree::Node::DeleteKeyFromList(string key, Node* root) {
  size_t number_of_child_in_parent = parent_->NumberOfKeyThatClosest(key);
  if (GetKeyelements().size() > kOrder - 1) {
    DeleteElement(key);
  } else {
    DeleteElement(key);
    MakeBalance(number_of_child_in_parent, root);
  }
}

void BPlusTree::Node::MakeBalance(size_t number_of_child_in_parent, Node* root) {
  if (number_of_child_in_parent > 0
    && parent_->GetChildren()[number_of_child_in_parent - 1]->GetKeyelements().size() > kOrder - 1) {
  ChangeElementsWithLeftBrother(number_of_child_in_parent);
  } else if (number_of_child_in_parent < parent_->GetChildren().size() - 1
    && parent_->GetChildren()[number_of_child_in_parent + 1]->GetKeyelements().size() > kOrder - 1) {
    ChangeElementsWithRightBrother(number_of_child_in_parent);
  } else {
    UnitWithBrother(number_of_child_in_parent, root);
  }
}

void BPlusTree::Node::DeleteElement(string key) {
  std::vector<Element> old_keys = GetKeyelements();
  std::vector<Element> new_keys;
  for (auto element : old_keys)
    if (element.GetKey() != key) new_keys.push_back(element);
  SetKeyelements(new_keys);
  if (children_.size() > 0) {
    size_t number_of_closest = NumberOfKeyThatClosest(key);
    std::vector<Node*> new_children;
    for (size_t i = 0; i < children_.size(); ++i) {
      if (i != number_of_closest) {
      new_children.push_back(children_[i]);
      }
    }
    SetChildren(new_children);
  }
}

void BPlusTree::Node::DeleteElementAtLeft(string key) {
  std::vector<Element> old_keys = GetKeyelements();
  std::vector<Element> new_keys;
  for (auto element : old_keys)
    if (element.GetKey() != key) new_keys.push_back(element);
  SetKeyelements(new_keys);
  if (children_.size() > 0) {
    size_t number_of_closest = NumberOfKeyThatClosest(key);
    std::vector<Node*> new_children;
    for (size_t i = 0; i < children_.size(); ++i) {
      if (i != number_of_closest + 1) {
      new_children.push_back(children_[i]);
      }
    }
    SetChildren(new_children);
  }
}

void BPlusTree::Node::ChangeElementsWithLeftBrother(size_t number_of_child_in_parent) {
    size_t number_of_brother = number_of_child_in_parent - 1;
    Node* left_brother = parent_->GetChildren()[number_of_brother];
    if (type_ == Node::kList) {
      Element element_for_insert = parent_->GetKeyelements()[number_of_brother];
      InsertElementInNode(element_for_insert, nullptr, nullptr);
      std::vector<Element> keyelements_of_brother = left_brother->GetKeyelements();
      Element element_for_parent = keyelements_of_brother[keyelements_of_brother.size() - 1];
      parent_->ChangeKeyelement(number_of_brother, element_for_parent);
      left_brother->DeleteElement(element_for_parent.GetKey());
    } else {
      Element element_for_insert = parent_->GetKeyelements()[number_of_brother];
      InsertElementInNodeAtBegin(element_for_insert,
      left_brother->GetChildren()[left_brother->GetChildren().size() - 1]);
      std::vector<Element> keyelements_of_brother = left_brother->GetKeyelements();
      Element element_for_parent = keyelements_of_brother[keyelements_of_brother.size() - 1];
      parent_->ChangeKeyelement(number_of_brother, element_for_parent);
      left_brother->DeleteElementAtLeft(element_for_parent.GetKey());
      children_[0]->SetParent(this);
    }
}

void BPlusTree::Node::ChangeElementsWithRightBrother(size_t number_of_node) {
  Node* right_brother = parent_->GetChildren()[number_of_node + 1];
    if (type_ == Node::kList) {
      Element element_for_insert = parent_->GetKeyelements()[number_of_node];
      InsertElementInNode(element_for_insert, nullptr, nullptr);
      std::vector<Element> keyelements_of_brother = right_brother->GetKeyelements();
      Element element_for_parent = keyelements_of_brother[0];
      parent_->ChangeKeyelement(number_of_node, element_for_parent);
      right_brother->DeleteElement(element_for_parent.GetKey());
    } else {
      Element element_for_insert = parent_->GetKeyelements()[number_of_node];
      InsertElementInNodeAtEnd(element_for_insert, right_brother->GetChildren()[0]);
      std::vector<Element> keyelements_of_brother = right_brother->GetKeyelements();
      Element element_for_parent = keyelements_of_brother[0];

      parent_->ChangeKeyelement(number_of_node, element_for_parent);
      right_brother->DeleteElement(element_for_parent.GetKey());
      children_[children_.size() - 1]->SetParent(this);
    }
}

void BPlusTree::Node::ChangeKeyelement(int number, const Element& element_for_change) {
  keyelements_[number] = element_for_change;
}

void BPlusTree::Node::UnitWithBrother(size_t number_of_child_in_parent, Node* root) {
  if (parent_->GetType() == kRoot && parent_->GetKeyelements().size() == 1) {
    MakeNewRoot(root);
  } else {
    if (number_of_child_in_parent > 0) {
      UnitWithLeftBrother(number_of_child_in_parent, root);
    } else if (type_ != Node::kRoot) {
      UnitWithRightBrother(number_of_child_in_parent, root);
    }
  }
}

void BPlusTree::Node::UnitWithLeftBrother(size_t number_of_child_in_parent, Node* root) {
  int number_of_brother = number_of_child_in_parent - 1;
  Node* left_brother = parent_->GetChildren()[number_of_brother];
  if (type_ == Node::kList) {
    std::vector<Element> elements_of_brother = left_brother->GetKeyelements();
    Element element_from_parent_for_insert = parent_->GetKeyelements()[number_of_brother];
    InsertElementInNode(element_from_parent_for_insert, nullptr, nullptr);
    elements_of_brother.insert(elements_of_brother.end(), keyelements_.begin(), keyelements_.end());
    keyelements_ = elements_of_brother;
    if (parent_->GetKeyelements().size() == kOrder - 1 && parent_->GetType() != Node::kRoot) {
      size_t number_of_parent =
             parent_->GetParent()->NumberOfKeyThatClosest(parent_->GetKeyelements()[0].GetKey());
      parent_->DeleteElement(element_from_parent_for_insert.GetKey());
      parent_->MakeBalance(number_of_parent, root);
    } else {
      parent_->DeleteElement(element_from_parent_for_insert.GetKey());
    }
    delete left_brother;
    left_brother = nullptr;
  } else {
    std::vector<Element> elements_of_brother = left_brother->GetKeyelements();
    std::vector<Node*> children_of_brother = left_brother->GetChildren();
    Element element_from_parent_for_insert = parent_->GetKeyelements()[number_of_brother];
    InsertElementInNode(element_from_parent_for_insert, nullptr, nullptr);
    elements_of_brother.insert(elements_of_brother.end(), keyelements_.begin(), keyelements_.end());
    children_of_brother.insert(children_of_brother.end(), children_.begin(), children_.end());
    keyelements_ = elements_of_brother;
    children_ = children_of_brother;
    for (auto ch : children_)
       ch->SetParent(this);
    if (parent_->GetKeyelements().size() == kOrder - 1 && parent_->GetType() != Node::kRoot) {
      size_t number_of_parent =
             parent_->GetParent()->NumberOfKeyThatClosest(parent_->GetKeyelements()[0].GetKey());
      parent_->DeleteElement(element_from_parent_for_insert.GetKey());
      parent_->MakeBalance(number_of_parent, root);
    } else {
      parent_->DeleteElement(element_from_parent_for_insert.GetKey());
    }
    delete left_brother;
    left_brother = nullptr;
  }
}

void BPlusTree::Node::UnitWithRightBrother(size_t number_of_node, Node* root) {
  Node* right_brother = parent_->GetChildren()[number_of_node + 1];
  if (type_ == Node::kList) {
    std::vector<Element> elements_of_brother = right_brother->GetKeyelements();
    Element element_from_parent_for_insert = parent_->GetKeyelements()[number_of_node];
    InsertElementInNode(element_from_parent_for_insert, nullptr, nullptr);
    keyelements_.insert(keyelements_.end(), elements_of_brother.begin(), elements_of_brother.end());
    if (parent_->GetKeyelements().size() == kOrder - 1 && parent_->GetType() != Node::kRoot) {
      size_t number_of_parent =
             parent_->GetParent()->NumberOfKeyThatClosest(parent_->GetKeyelements()[0].GetKey());
      parent_->DeleteElementAtLeft(element_from_parent_for_insert.GetKey());
      parent_->MakeBalance(number_of_parent, root);
    } else {
      parent_->DeleteElementAtLeft(element_from_parent_for_insert.GetKey());
    }
    delete right_brother;
    right_brother = nullptr;
  } else {
    std::vector<Element> elements_of_brother = right_brother->GetKeyelements();
    std::vector<Node*> children_of_brother = right_brother->GetChildren();
    Element element_from_parent_for_insert = parent_->GetKeyelements()[number_of_node];
    InsertElementInNode(element_from_parent_for_insert, nullptr, nullptr);
    keyelements_.insert(keyelements_.end(), elements_of_brother.begin(), elements_of_brother.end());
    children_.insert(children_.end(), children_of_brother.begin(), children_of_brother.end());
    for (auto ch : children_)
       ch->SetParent(this);
    if (parent_->GetKeyelements().size() == kOrder - 1 && parent_->GetType() != Node::kRoot) {
      size_t number_of_parent =
             parent_->GetParent()->NumberOfKeyThatClosest(parent_->GetKeyelements()[0].GetKey());
      parent_->DeleteElementAtLeft(element_from_parent_for_insert.GetKey());
      parent_->MakeBalance(number_of_parent, root);
    } else {
      parent_->DeleteElementAtLeft(element_from_parent_for_insert.GetKey());
    }
    delete right_brother;
    right_brother = nullptr;
  }
}

void BPlusTree::Node::MakeNewRoot(Node* root) {
  std::vector<Element> elements_in_left = root->GetChildren()[0]->GetKeyelements();
  std::vector<Element> elements_in_right = root->GetChildren()[1]->GetKeyelements();
  std::vector<Node*> children_in_left = root->GetChildren()[0]->GetChildren();
  std::vector<Node*> children_in_right = root->GetChildren()[1]->GetChildren();
  children_in_left.insert(children_in_left.end(), children_in_right.begin(), children_in_right.end());
  elements_in_left.insert(elements_in_left.end(), root->GetKeyelements()[0]);
  elements_in_left.insert(elements_in_left.end(), elements_in_right.begin(), elements_in_right.end());
  delete root->GetChildren()[0];
  delete root->GetChildren()[1];
  *root = Node(kRoot);
  (root)->SetKeyelements(elements_in_left);
  (root)->SetChildren(children_in_left);
  for (auto el : children_in_left) el->SetParent(root);
}

/* -------------------------------------------------------------------------- */
/*                                vizualization                               */
/* -------------------------------------------------------------------------- */

void BPlusTree::TreeViz(string file_name) {
  std::ofstream out_stream;
  out_stream.open(file_name + ".gv", std::ios::trunc);
  out_stream << "digraph {\nnode [margin=0 fontsize=8 width=0.5 shape=box]\n";
  PrintNode(root_, &out_stream);
  out_stream << "}";
}

void BPlusTree::PrintNode(Node* node, std::ofstream* out_stream) {
  if (node && node->GetKeyelements().size() > 0) {
      *out_stream << "\"" << node->GetKeyelements()[0].GetKey() << "\"" << "[label=\"";
      for (auto el : node->GetKeyelements())
        *out_stream << el.GetKey() << "; ";
      *out_stream << "\"color=grey, style=filled, shape=circle]\n";
      if (node->GetChildren().size() > 0) {
        for (auto child : node->GetChildren()) {
          *out_stream << "\"" << node->GetKeyelements()[0].GetKey() << "\"" << "->" "\""
                      << child->GetKeyelements()[0].GetKey() << "\"\n";
        }
      }
      if (node->GetParent()) {
        *out_stream << "\"" << node->GetKeyelements()[0].GetKey() << "\"" << "->" "\""
                    << node->GetParent()->GetKeyelements()[0].GetKey() << "\"[style=dotted, color=grey]\n";
      }
      for (auto child : node->GetChildren()) PrintNode(child, out_stream);
  }
}
}  // namespace s21
