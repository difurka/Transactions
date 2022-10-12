#ifndef SRC_CONTAINERS_SELF_BALANCING_BINARY_SEARCH_TREE_H_
#define SRC_CONTAINERS_SELF_BALANCING_BINARY_SEARCH_TREE_H_

#include <string>
#include <vector>
#include "../storage.h"

namespace s21 {

class SelfBalancingBinarySearchTree : public Storage {
 public:
  struct Node {
    explicit Node(const Element& key, Node* parent = nullptr);

    Element key_;
    int balance_ = 0;
    Node* parent_ = nullptr;
    Node* right_ = nullptr;
    Node* left_ = nullptr;
  };

  enum Side {
    kLeft = 1,
    kRight = -1
  };

  SelfBalancingBinarySearchTree();
  SelfBalancingBinarySearchTree(const SelfBalancingBinarySearchTree& other);
  SelfBalancingBinarySearchTree(SelfBalancingBinarySearchTree&& other);
  SelfBalancingBinarySearchTree& operator=(const SelfBalancingBinarySearchTree& other);
  SelfBalancingBinarySearchTree& operator=(const SelfBalancingBinarySearchTree&& other);
  inline void CopyTree(const SelfBalancingBinarySearchTree& other);
  ~SelfBalancingBinarySearchTree();

  void Set(element element) override;
  Storage::Element Get(const std::string& key) const override;
  bool Exists(string key) const override;
  bool Del(string key) override;
  bool Update(string key, const Element::Data& data) override;
  bool Rename(string key, string new_key) override;
  int Ttl(string key) const override;
  vector Find(const Storage::Element::Data& data) const override;
  void Init() override;
  std::vector<Element> AllElements() const override;
  void Remove(const std::string& key);
  void TreeViz(const std::string &file_name);

 private:
  Node* root_ = nullptr;
  bool is_balanced_ = true;
  bool is_remove_ = false;

  Node* FindNode(const std::string& key) const;
  void ReplaceWithSingleSonNode(Node* current_node, Node* right_node);
  void RemoveWhenTwoChildren(Node* removable_node);
  void RemoveWhenSingleSon(Node* removable_node);
  void RemoveWhenNoChild(Node* removable_node);
  std::vector<Node*> GetAllNodes() const;

  Node* FindMax(Node* node);
  Node* FindMin(Node* node);

  void RebalanceAfterInsert(Node* node);
  void Clear(Node* node);

  Node* LeftSideBalancing(Node* current_node);
  Node* RightSideBalancing(Node* current_node);
  Node* SimpleRotate(Node* node);
  Node* RightRotate(Node* node);
  Node* LeftRotate(Node* node);
  Node* BigRightRotate(Node* pivot);
  Node* BigLeftRotate(Node* pivot);

  void ChangeParent(const Node* node, Node* parent, Node* previous);
  void UpdateBalance(Node* node_a, Node* node_b, int balance);
  void RemoveNode(Node* removable);
  void RebalanceAfterRemoval(Node* node);
  void CalculateBalance(Node* current_node, const Node* previous_node);

  void GetElement(Node* node, std::vector<Node*>* vector) const;
  void PrintNode(Node* node, std::ofstream* out_stream);
};

}  // namespace s21

#endif  // SRC_CONTAINERS_SELF_BALANCING_BINARY_SEARCH_TREE_H_
