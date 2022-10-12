#include "self_balancing_binary_search_tree.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace s21 {

/* -------------------------------------------------------------------------- */
/*                                  override                                  */
/* -------------------------------------------------------------------------- */

SelfBalancingBinarySearchTree::Element SelfBalancingBinarySearchTree::Get(string key) const {
  Node* node = FindNode(key);
  if (node) return node->key_;
  return SelfBalancingBinarySearchTree::Element("", {});
}

bool SelfBalancingBinarySearchTree::Exists(string key) const {
  if (FindNode(key)) return true;
  return false;
}

bool SelfBalancingBinarySearchTree::Del(string key) {
  if (FindNode(key)) {
    Remove(key);
    return true;
  } else {
    return false;
  }
}

bool SelfBalancingBinarySearchTree::Update(string key, const Element::Data& data) {
  Node* node = FindNode(key);
  if (node) {
    if (data.surname != "-") node->key_.SetSurname(data.surname);
    if (data.name != "-") node->key_.SetName(data.name);
    if (data.year_of_birth != "-") node->key_.SetYearOfBirth(data.year_of_birth);
    if (data.city != "-") node->key_.SetCity(data.city);
    if (data.coins != "-") node->key_.SetCoins(data.coins);
    return true;
  }
  return false;
}

bool SelfBalancingBinarySearchTree::Rename(string key, string new_key) {
  Node* node = FindNode(key);
  if (node) {
    Element element = node->key_;
    element.SetKey(new_key);
    Remove(key);
    Set(element);
    return true;
  }
  return false;
}

int SelfBalancingBinarySearchTree::Ttl(string key) const {
  Node* node = FindNode(key);
  if (node) {
    return node->key_.GetLifeTime();
  }
  return 0;
}

std::vector<std::string> SelfBalancingBinarySearchTree::Find(const Element::Data& data) const {
  std::vector<std::string> vector_of_key;
  auto nodes = GetAllNodes();
  for (auto node : nodes) {
    if (IsDataSiutable(data, node->key_.GetData())) {
      vector_of_key.push_back(node->key_.GetKey());
    }
  }
  return vector_of_key;
}

void SelfBalancingBinarySearchTree::Init() {
  Clear(root_);
  root_ = nullptr;
}


std::vector<SelfBalancingBinarySearchTree::Element> SelfBalancingBinarySearchTree::AllElements() const {
  std::vector<SelfBalancingBinarySearchTree::Element> result;
  auto nodes = GetAllNodes();
  std::transform(nodes.begin(), nodes.end(), std::back_inserter(result),
                                            [](Node* node) {return node->key_;});
  return result;
}

std::vector<SelfBalancingBinarySearchTree::Node*> SelfBalancingBinarySearchTree::GetAllNodes() const {
  std::vector<Node*> result;
  GetElement(root_, &result);
  return result;
}

void SelfBalancingBinarySearchTree::GetElement(SelfBalancingBinarySearchTree::Node* node,
                                              std::vector<Node*>* vector) const {
  if (node) {
    vector->push_back(node);
    GetElement(node->left_, vector);
    GetElement(node->right_, vector);
  }
}

/* -------------------------------------------------------------------------- */
/*                                 class Tree                                 */
/* -------------------------------------------------------------------------- */

SelfBalancingBinarySearchTree::SelfBalancingBinarySearchTree() { }

SelfBalancingBinarySearchTree::SelfBalancingBinarySearchTree(const SelfBalancingBinarySearchTree& other) {
  CopyTree(other);
}

SelfBalancingBinarySearchTree::SelfBalancingBinarySearchTree(SelfBalancingBinarySearchTree&& other) {
  std::swap(root_, other.root_);
}

SelfBalancingBinarySearchTree&
SelfBalancingBinarySearchTree::operator=(const SelfBalancingBinarySearchTree& other) {
  CopyTree(other);
  return *this;
}

SelfBalancingBinarySearchTree&
SelfBalancingBinarySearchTree::operator=(const SelfBalancingBinarySearchTree&& other) {
  CopyTree(other);
  return *this;
}

inline void SelfBalancingBinarySearchTree::CopyTree(const SelfBalancingBinarySearchTree& other) {
  if (&other != this) {
    for (auto element : other.GetAllNodes()) {
      Set(element->key_);
    }
  }
}

SelfBalancingBinarySearchTree::~SelfBalancingBinarySearchTree() {
  if (root_) {
    Clear(root_);
  }
}

void SelfBalancingBinarySearchTree::Clear(Node* node) {
  if (node) {
    if (node->right_) {
      Clear(node->right_);
    }
    if (node->left_) {
      Clear(node->left_);
    }
    delete node;
  }
}

void SelfBalancingBinarySearchTree::Set(element key) {
  if (!Exists(key.GetKey())) {
    is_remove_ = false;
    Node* current_node = root_;
    Node* previous_node = nullptr;
    bool is_left = true;
    while (current_node) {
      previous_node = current_node;
      if (key.GetKey() > current_node->key_.GetKey()) {
        current_node = current_node->right_;
        is_left = false;
      } else {
        current_node = current_node->left_;
        is_left = true;
      }
    }

    current_node = new Node(key, previous_node);
    is_balanced_ = false;
    if (is_left && root_) {
      previous_node->left_ = current_node;
    } else if (root_) {
      previous_node->right_ = current_node;
    }
    if (!root_) root_ = current_node;
    RebalanceAfterInsert(current_node);
  }
}

void SelfBalancingBinarySearchTree::RebalanceAfterInsert(Node* node) {
  Node* previous_node = node;
  Node* current_node = nullptr;

  while (previous_node->parent_ && !is_balanced_) {
    current_node = previous_node->parent_;

    Side side = (current_node->left_ == previous_node) ? kLeft : kRight;
    if (side == kLeft) {
      ++current_node->balance_;
      if (current_node->balance_ == 2) {
        LeftSideBalancing(current_node);
      } else if (current_node->balance_ == 0) {
        is_balanced_ = true;
      }
    } else if (side == kRight) {
      --current_node->balance_;
      if (current_node->balance_ == -2) {
        RightSideBalancing(current_node);
      } else if (current_node->balance_ == 0) {
        is_balanced_ = true;
      }
    }

    previous_node = current_node;
  }
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::LeftSideBalancing(Node* current_node) {
  Node* current_rotate_root_ = current_node;
  Node* left_son = current_node->left_;
  if (left_son->balance_ == 1 || left_son->balance_ == 0) {
    current_rotate_root_ = SimpleRotate(current_node);
    if (current_node->balance_ == 0 && !is_remove_) is_balanced_ = true;
  } else if (left_son->balance_ == -1) {
    current_rotate_root_ = BigRightRotate(current_node);
  }
  return current_rotate_root_;
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::BigRightRotate(Node* current) {
  Node* pivot = current->left_;
  Node* grandson = pivot->right_;
  Node* current_rotate_root_ = current;
  if (grandson) {
    if (grandson->balance_ == 1) {
      current->balance_ = -1;
      pivot->balance_ = 0;
      grandson->balance_ = 0;
    } else if (grandson->balance_ == -1) {
      current->balance_ = 0;
      pivot->balance_ = 1;
      grandson->balance_ = 0;
    } else if (grandson->balance_ == 0) {
      current->balance_ = 0;
      pivot->balance_ = 0;
      grandson->balance_ = 0;
    }
    LeftRotate(current->left_);
    current_rotate_root_ = RightRotate(current);
    if (!is_remove_) is_balanced_ = true;
  }
    return current_rotate_root_;
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::RightSideBalancing(Node* current_node) {
  Node* current_rotate_root_ = current_node;
  Node* right_son = current_node->right_;
  if (right_son->balance_ == -1 || right_son->balance_ == 0) {
    current_rotate_root_ = SimpleRotate(current_node);
  } else if (right_son->balance_ == 1) {
    current_rotate_root_ = BigLeftRotate(current_node);
  }
  return current_rotate_root_;
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::BigLeftRotate(Node* current) {
  Node* current_rotate_root_ = current;
  Node* pivot = current->right_;
  Node* grandson = pivot->left_;
  if (grandson) {
    if (grandson->balance_ == 1) {
      current->balance_ = 0;
      pivot->balance_ = -1;
      grandson->balance_ = 0;
    } else if (grandson->balance_ == -1) {
      current->balance_ = 1;
      pivot->balance_ = 0;
      grandson->balance_ = 0;
    } else if (grandson->balance_ == 0) {
      current->balance_ = 0;
      pivot->balance_ = 0;
      grandson->balance_ = 0;
    }
    RightRotate(current->right_);
    current_rotate_root_ = LeftRotate(current);
    if (!is_remove_) is_balanced_ = true;
  }
  return current_rotate_root_;
}


SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::SimpleRotate(Node* node) {
  Node* current_rotate_root_ = node;
  if (node->balance_ == 2) {
    UpdateBalance(node, node->left_, 2);
    current_rotate_root_ = RightRotate(node);
    if (node->balance_ == 0 && !is_remove_)
    is_balanced_ = true;
  } else if (node->balance_ == -2) {
    UpdateBalance(node, node->right_, -2);
    current_rotate_root_ = LeftRotate(node);
    if (node->balance_ == 0 && !is_remove_)
    is_balanced_ = true;
  } else if (node->balance_ == 0 && !is_remove_) {
    is_balanced_ = true;
  }
  if (node->balance_ == 0 && !is_remove_) is_balanced_ = true;
  return current_rotate_root_;
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::LeftRotate(Node* node) {
  Node* parent = node->parent_;
  Node* previous = node->right_;

  if (node->right_->left_) node->right_->left_->parent_ = node->right_;
  node->right_ = node->right_->left_;
  node->parent_ = previous;
  if (previous->left_ ) previous->left_->parent_ = node;
  previous->left_ = node;
  previous->parent_ = parent;

  ChangeParent(node, parent, previous);
  return node->parent_;
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::RightRotate(Node* node) {
  Node* parent = node->parent_;
  Node* previous = node->left_;

  if (node->left_->right_) node->left_->right_->parent_ = node->left_;
  node->left_ = node->left_->right_;
  node->parent_ = previous;
  if (previous->right_ ) previous->right_->parent_ = node;
  previous->right_ = node;
  previous->parent_ = parent;

  ChangeParent(node, parent, previous);
  return node->parent_;
}

void SelfBalancingBinarySearchTree::ChangeParent(const Node* node, Node* parent, Node* previous) {
  if (parent && parent->left_ == node) {
    parent->left_ = previous;
  } else if (parent && parent->right_ == node) {
    parent->right_ = previous;
  } else {
    root_ = previous;
  }
}

void SelfBalancingBinarySearchTree::UpdateBalance(Node* node_a, Node* node_b, int balance) {
  if (node_a->balance_ == balance) {
    if (node_b->balance_ == -1 || node_b->balance_ == 1) {
      node_a->balance_ = 0;
      node_b->balance_ = 0;
      if (!is_remove_)is_balanced_ = true;
    } else if (node_b->balance_ == 0) {
      if (balance == -2) {
        node_a->balance_ = -1;
        node_b->balance_ = 1;
      } else if (balance == 2) {
        node_a->balance_ = 1;
        node_b->balance_ = -1;
      }
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                                 remove node                                */
/* -------------------------------------------------------------------------- */

void SelfBalancingBinarySearchTree::Remove(const std::string& key) {
  is_remove_ = true;
  Node* removable_node = FindNode(key);
  if (removable_node) {
    if (!removable_node->left_ && !removable_node->right_) {
      RemoveWhenNoChild(removable_node);
    } else if (removable_node->right_ || removable_node->left_) {
      if (removable_node->left_ && removable_node->right_) {
        RemoveWhenTwoChildren(removable_node);
      } else {
        RemoveWhenSingleSon(removable_node);
      }
    }
  }
}

SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::FindNode(const std::string &key) const {
  Node* current_node = root_;

  while (current_node) {
    if (key != current_node->key_.GetKey()) {
      if (key > current_node->key_.GetKey()) {
        current_node = current_node->right_;
      } else {
        current_node = current_node->left_;
      }
    } else {
      return current_node;
    }
  }
  return nullptr;
}

void SelfBalancingBinarySearchTree::RemoveWhenNoChild(Node* removable_node) {
  Node* parent = removable_node->parent_;
  if (!parent && removable_node == root_) {
    delete removable_node;
    root_ = nullptr;
  } else {
    RemoveNode(removable_node);
    RebalanceAfterRemoval(parent);
  }
}

void SelfBalancingBinarySearchTree::RemoveWhenTwoChildren(Node* removable_node) {
  int balance = removable_node->balance_;
  if (balance == 1 || balance == 0) {
    auto *replacement_node = FindMax(removable_node);
    Element temp_key = replacement_node->key_;
    Remove(temp_key.GetKey());
    removable_node->key_ = temp_key;
  } else if (balance == -1) {
    auto *replacement_node = FindMin(removable_node);
    Element temp_key = replacement_node->key_;
    Remove(temp_key.GetKey());
    removable_node->key_ = temp_key;
  }
}

  SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::FindMax(Node* node) {
    Node* max_node = node->left_;
    while (max_node->right_) {
      max_node = max_node->right_;
    }
    return max_node;
  }

  SelfBalancingBinarySearchTree::Node* SelfBalancingBinarySearchTree::FindMin(Node* node) {
    Node* min_node = node->right_;
    while (min_node->left_) {
      min_node = min_node->left_;
    }
    return min_node;
  }

void SelfBalancingBinarySearchTree::RemoveWhenSingleSon(Node* removable_node) {
  Node* parent = removable_node->parent_;
  if (parent)
    parent->balance_ += (parent->left_ == removable_node) ?  -1 : 1;
  if (removable_node->left_) {
    ReplaceWithSingleSonNode(removable_node, removable_node->left_);
  } else if (removable_node->right_) {
    ReplaceWithSingleSonNode(removable_node, removable_node->right_);
  }
  RebalanceAfterRemoval(parent);
}

void SelfBalancingBinarySearchTree::ReplaceWithSingleSonNode(Node* current_node, Node* replace_node) {
  current_node->key_ = replace_node->key_;
  current_node->left_ = replace_node->left_;
  current_node->right_ = replace_node->right_;
  current_node->balance_ = 0;
  delete replace_node;
}

void SelfBalancingBinarySearchTree::RemoveNode(Node* removable) {
  Node* parent = removable->parent_;
  if (removable->left_ || removable->right_) throw std::runtime_error("removable node has son");
  if (parent) {
    if (parent->left_ == removable) {
      delete parent->left_;
      parent->left_ = nullptr;
      --parent->balance_;
    } else if (parent->right_ == removable) {
      delete parent->right_;
      parent->right_ = nullptr;
      ++parent->balance_;
    }
  }
}

void SelfBalancingBinarySearchTree::RebalanceAfterRemoval(Node* node) {
  is_balanced_ = false;
  Node* previous_node = nullptr;
  Node* current_node = node;
  while (current_node && !is_balanced_) {
    if (previous_node) CalculateBalance(current_node, previous_node);
    if (current_node->balance_ == 2) {
      current_node = LeftSideBalancing(current_node);
    } else if (current_node->balance_ == -2) {
      current_node = RightSideBalancing(current_node);
    }
    if ((current_node->balance_ == 1 || current_node->balance_ == -1)) {
      is_balanced_ = true;
    }
    previous_node = current_node;
    current_node = current_node->parent_;
  }
}

void SelfBalancingBinarySearchTree::CalculateBalance(Node* current_node, const Node* previous_node) {
  Side side = (current_node->left_ == previous_node) ? kLeft : kRight;
  if (side == kLeft) {
    --current_node->balance_;
  } else if (side == kRight) {
    ++current_node->balance_;
  }
}

/* -------------------------------------------------------------------------- */
/*                                 class Node                                 */
/* -------------------------------------------------------------------------- */

SelfBalancingBinarySearchTree::Node::Node(const Element& key, Node* parent)
: key_(key)
, parent_(parent) { }

/* -------------------------------------------------------------------------- */
/*                                vizualization                               */
/* -------------------------------------------------------------------------- */

void SelfBalancingBinarySearchTree::TreeViz(const std::string &file_name) {
  std::ofstream out_stream;
  out_stream.open(file_name + ".gv", std::ios::trunc);
  out_stream << "digraph {\nnode [margin=0 fontsize=8 width=0.5 shape=box]\n";
  PrintNode(root_, &out_stream);
  out_stream << "}";
}

void SelfBalancingBinarySearchTree::PrintNode(Node* node, std::ofstream* out_stream) {
  if (node) {
    *out_stream << "\"" << node->key_.GetKey() << "\"" << "[label=\"" << node->key_.GetKey() <<
      " [" << node->balance_ << "]\", color=grey, style=filled, shape=circle]\n";
    if (node->left_) {
      *out_stream << "\"" << node->key_.GetKey() << "\"" << "->" "\"" <<
        node->left_->key_.GetKey() << "\"\n";
    } else {
      *out_stream << "\"" << node->key_.GetKey() << "\"" << "->" "\"" << "left:" <<
        node->key_.GetKey() << "\"\n";
    }
    if (node->right_) {
      *out_stream << "\"" << node->key_.GetKey() << "\"" << "->" "\"" <<
      node->right_->key_.GetKey() << "\"\n";
    } else {
      *out_stream << "\"" << node->key_.GetKey() << "\"" << "->" "\"" << "right:" <<
        node->key_.GetKey() << "\"\n";
    }

    if (node->parent_) {
      *out_stream << "\"" << node->key_.GetKey() << "\"" << "->" "\"" << node->parent_->key_.GetKey() <<
        "\"[style=dotted, color=grey]\n";
    }
    PrintNode(node->right_, out_stream);
    PrintNode(node->left_, out_stream);
  }
}

}  // namespace s21
