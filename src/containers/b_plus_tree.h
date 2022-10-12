#ifndef SRC_CONTAINERS_B_PLUS_TREE_H_
#define SRC_CONTAINERS_B_PLUS_TREE_H_

#include "../storage.h"

namespace s21 {

class BPlusTree: public Storage {
 public:
  using data_t = Storage::Element::Data;
  class Node {
    class BPlusTree;
   public:
    enum TypeOfNode {
      kRoot,
      kNotList,
      kList,
      kNoType
    };

    Node();
    explicit Node(TypeOfNode type);
    Node(const Node&);
    Node(Node&&);
    Node& operator=(const Node&);
    Node& operator=(const Node&&);
    ~Node();

    void SetKeyelements(const std::vector<Element>& keyelements);
    void SetType(TypeOfNode type);
    void SetChildren(const std::vector<Node*>& children);
    void SetParent(Node* parent);

    TypeOfNode GetType() const;
    std::vector<Node*> GetChildren() const;
    std::vector<Element> GetKeyelements() const;
    Node* GetParent();

    void DeleteKeyFromList(string key, Node* root);
    void DeleteKeyFromNotList(int number, Node* root);
    void DeleteElement(string key);
    void TakeAllKeyelements(std::vector<Element>* result, data_t data);
    bool HasKey(string key) const;
    void DeleteNodesInTreeWithThisRoot();
    int NumberOfKeyThatClosest(string key) const;
    void InsertElementInNode(const Element& element, Node* left_child, Node* right_child);
    void CleanReferencesToChildren();
    void UpdateData(int number, const data_t& data);
    static const int kNullData = -1;

   private:
    std::vector<Element> keyelements_;
    std::vector<Node*> children_;
    Node* parent_ = nullptr;
    TypeOfNode type_ = kNoType;

    void CopyNode(const Node& other);
    void MakeBalance(size_t number_of_child_in_parent, Node* root);
    void ChangeElementsWithLeftBrother(size_t number_of_child_in_parent);
    void ChangeElementsWithRightBrother(size_t number_of_child_in_parent);
    void UnitWithBrother(size_t number_of_child_in_parent, Node* root);
    void UnitWithLeftBrother(size_t number_of_child_in_parent, Node* root);
    void UnitWithRightBrother(size_t number_of_child_in_parent, Node* root);
    void MakeNewRoot(Node* root);
    Node* GetListWithClosestRightKey(int number);
    void InsertElementInNodeAtBegin(const Element& element, Node* child);
    void InsertElementInNodeAtEnd(const Element& element, Node* child);
    void UnitWithBrotherForList(size_t number_of_child_in_parent, Node* root, Node* brother);
    void UnitWithBrotherForNotList(size_t number_of_child_in_parent, Node* root, Node* brother);
    void DeleteElementAtLeft(string key);
    void ChangeKeyelement(int number, const Element& element_for_change);
  };

  BPlusTree();
  BPlusTree(const BPlusTree&);
  BPlusTree(BPlusTree&&);
  BPlusTree& operator=(const BPlusTree&);
  BPlusTree& operator=(const BPlusTree&&);
  ~BPlusTree();
  void Set(element element) override;
  Element Get(string key) const override;
  bool Exists(string key) const override;
  bool Del(string key) override;
  bool Update(string key, const data_t& data) override;
  bool Rename(string key, string new_key) override;
  int Ttl(string key) const override;
  vector Find(const data_t& data) const override;
  void Init() override;
  void TreeViz(string file_name);
  std::vector<Element> AllElements() const override;

 private:
  static const int kOrder = 2;
  Node* root_ = nullptr;
  Node* GetReferenceOfNodeForThisKeyFromNode(string key, Node* node) const;
  void PrintNode(Node* node, std::ofstream* out_stream);
  void DivideOnTwoNods(Node* node);
  void DivideRoot(Node* node);
  void DivideNotRoot(Node* node);
  void DivideChildren(Node* node, int number_of_closest);
  inline void CopyTree(const BPlusTree& other);
};
}  // namespace s21

#endif  // SRC_CONTAINERS_B_PLUS_TREE_H_
