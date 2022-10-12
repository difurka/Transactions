#include <set>
#include "gtest/gtest.h"
#include "self_balancing_binary_search_tree.h"

void CheckAVL(const std::vector<std::string> &expect,
  const std::vector<s21::SelfBalancingBinarySearchTree::Node*> &result);

/* Move GetAllNodes() from private to public */

/* -------------------------------------------------------------------------- */
/*                               AVL tree test                                */
/* -------------------------------------------------------------------------- */

s21::Storage::Element::Data empty_data;

TEST(Transactions, insert_simple_right_rotate) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));

  std::vector<std::string> expected = {
    "60", "0", "50", "70", "0",
    "50", "60", "0", "0", "0",
    "70", "60", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, insert_simple_left_rotate) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));

  std::vector<std::string> expected = {
    "60", "0", "50", "70", "0",
    "50", "60", "0", "0", "0",
    "70", "60", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, insert_1) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));

  std::vector<std::string> expected = {
    "60", "0", "50", "70", "0",
    "50", "60", "0", "0", "0",
    "70", "60", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, insert_2) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("60", empty_data));

  std::vector<std::string> expected = {
    "50", "0", "0", "60", "-1",
    "60", "50", "0", "0", "0",
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, insert_3) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));

  std::vector<std::string> expected = {
    "60", "0", "50", "0", "1",
    "50", "60", "0", "0", "0",
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, insert_4) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("80", empty_data));
  tree.Set(s21::Storage::Element("90", empty_data));
  tree.Set(s21::Storage::Element("100", empty_data));
  tree.Set(s21::Storage::Element("120", empty_data));
  tree.Set(s21::Storage::Element("121", empty_data));
  tree.Set(s21::Storage::Element("119", empty_data));

  std::vector<std::string> expected = {
    "60", "0", "120", "80", "1",
    "120", "60", "100", "50", "0",
    "100", "120", "0", "119", "-1",
    "119", "100", "0", "0", "0",
    "50", "120", "121", "0", "1",
    "121", "50", "0", "0", "0",
    "80", "60", "70", "90", "0",
    "70", "80", "0", "0", "0",
    "90", "80", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, insert_5) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("80", empty_data));
  tree.Set(s21::Storage::Element("90", empty_data));
  tree.Set(s21::Storage::Element("100", empty_data));
  tree.Set(s21::Storage::Element("40", empty_data));
  tree.Set(s21::Storage::Element("55", empty_data));

  std::vector<std::string> expected = {
    "60", "0", "40", "80", "1",
    "40", "60", "100", "50", "-1",
    "100", "40", "0", "0", "0",
    "50", "40", "0", "55", "-1",
    "55", "50", "0", "0", "0",
    "80", "60", "70", "90", "0",
    "70", "80", "0", "0", "0",
    "90", "80", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, insert_6) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("100", empty_data));
  tree.Set(s21::Storage::Element("150", empty_data));
  tree.Set(s21::Storage::Element("80", empty_data));
  tree.Set(s21::Storage::Element("120", empty_data));
  tree.Set(s21::Storage::Element("110", empty_data));

  std::vector<std::string> expected = {
    "150", "0", "110", "80", "1",
    "110", "150", "100", "120", "0",
    "100", "110", "0", "0", "0",
    "120", "110", "0", "0", "0",
    "80", "150", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, bit_right) {
  s21::SelfBalancingBinarySearchTree tree;

  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("45", empty_data));
  tree.Set(s21::Storage::Element("40", empty_data));
  tree.Set(s21::Storage::Element("48", empty_data));
  tree.Set(s21::Storage::Element("47", empty_data));

  std::vector<std::string> expected = {
    "48", "0", "45", "50", "0",
    "45", "48", "40", "47", "0",
    "40", "45", "0", "0", "0",
    "47", "45", "0", "0", "0",
    "50", "48", "0", "60", "-1",
    "60", "50", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, big) {
  s21::SelfBalancingBinarySearchTree tree;

  std::vector<std::string> values = {
    "34", "5346", "323", "76", "45", "235", "2233", "212", "345", "3",
    "36", "765", "43", "457", "3289", "342", "124", "216", "824", "363",
    "92", "3965", "86", "986", "763", "618", "358", "2356", "272", "7892",
    "317", "3682", "4677", "3623", "42", "4", "682", "3408", "1", "412",
    "214", "116"
  };

  for (auto &key : values) {
    tree.Set(s21::Storage::Element(key, empty_data));
  }

  std::vector<std::string> expected = {
    "45", "0", "34", "765", "1",
    "34", "45", "235", "36", "-1",
    "235", "34", "212", "3", "0",
    "212", "235", "116", "216", "0",
    "116", "212", "1", "124", "0",
    "1", "116", "0", "0", "0",
    "124", "116", "0", "0", "0",
    "216", "212", "214", "2233", "0",
    "214", "216", "0", "0", "0",
    "2233", "216", "0", "0", "0",
    "3", "235", "272", "323", "0",
    "272", "3", "2356", "0", "1",
    "2356", "272", "0", "0", "0",
    "323", "3", "317", "3289", "0",
    "317", "323", "0", "0", "0",
    "3289", "323", "0", "0", "0",
    "36", "34", "345", "3965", "-1",
    "345", "36", "342", "358", "1",
    "342", "345", "3408", "0", "1",
    "3408", "342", "0", "0", "0",
    "358", "345", "0", "0", "0",
    "3965", "36", "363", "42", "-1",
    "363", "3965", "3623", "3682", "0",
    "3623", "363", "0", "0", "0",
    "3682", "363", "0", "0", "0",
    "42", "3965", "4", "43", "1",
    "4", "42", "0", "412", "-1",
    "412", "4", "0", "0", "0",
    "43", "42", "0", "0", "0",
    "765", "45", "5346", "86", "1",
    "5346", "765", "457", "76", "-1",
    "457", "5346", "0", "4677", "-1",
    "4677", "457", "0", "0", "0",
    "76", "5346", "618", "763", "1",
    "618", "76", "0", "682", "-1",
    "682", "618", "0", "0", "0",
    "763", "76", "0", "0", "0",
    "86", "765", "824", "92", "0",
    "824", "86", "7892", "0", "1",
    "7892", "824", "0", "0", "0",
    "92", "86", "0", "986", "-1",
    "986", "92", "0", "0", "0"
  };
  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, simple_left_remove) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Remove("50");

  std::vector<std::string> expected = {
    "60", "0", "0", "70", "-1",
    "70", "60", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, simple_right_remove) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Remove("70");

  std::vector<std::string> expected = {
    "60", "0", "50", "0", "1",
    "50", "60", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, simple_left_remove_2) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("55", empty_data));
  tree.Set(s21::Storage::Element("45", empty_data));
  tree.Set(s21::Storage::Element("65", empty_data));
  tree.Set(s21::Storage::Element("75", empty_data));
  tree.Remove("65");


  std::vector<std::string> expected = {
    "60", "0", "50", "70", "0",
    "50", "60", "45", "55", "0",
    "45", "50", "0", "0", "0",
    "55", "50", "0", "0", "0",
    "70", "60", "0", "75", "-1",
    "75", "70", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, simple_right_remove_2) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("55", empty_data));
  tree.Set(s21::Storage::Element("45", empty_data));
  tree.Set(s21::Storage::Element("65", empty_data));
  tree.Set(s21::Storage::Element("75", empty_data));
  tree.Remove("75");

  std::vector<std::string> expected = {
    "60", "0", "50", "70", "0",
    "50", "60", "45", "55", "0",
    "45", "50", "0", "0", "0",
    "55", "50", "0", "0", "0",
    "70", "60", "65", "0", "1",
    "65", "70", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, two_sons_remove_1) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("55", empty_data));
  tree.Set(s21::Storage::Element("45", empty_data));
  tree.Set(s21::Storage::Element("65", empty_data));
  tree.Set(s21::Storage::Element("75", empty_data));
  tree.Remove("70");

  std::vector<std::string> expected = {
    "60", "0", "50", "65", "0",
    "50", "60", "45", "55", "0",
    "45", "50", "0", "0", "0",
    "55", "50", "0", "0", "0",
    "65", "60", "0", "75", "-1",
    "75", "65", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, two_sons_remove_2) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("55", empty_data));
  tree.Set(s21::Storage::Element("45", empty_data));
  tree.Set(s21::Storage::Element("65", empty_data));
  tree.Set(s21::Storage::Element("75", empty_data));
  tree.Remove("50");

  std::vector<std::string> expected = {
    "60", "0", "45", "70", "0",
    "45", "60", "0", "55", "-1",
    "55", "45", "0", "0", "0",
    "70", "60", "65", "75", "0",
    "65", "70", "0", "0", "0",
    "75", "70", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, two_sons_root_remove) {
  s21::SelfBalancingBinarySearchTree tree;
  tree.Set(s21::Storage::Element("60", empty_data));
  tree.Set(s21::Storage::Element("50", empty_data));
  tree.Set(s21::Storage::Element("70", empty_data));
  tree.Set(s21::Storage::Element("55", empty_data));
  tree.Set(s21::Storage::Element("45", empty_data));
  tree.Set(s21::Storage::Element("65", empty_data));
  tree.Set(s21::Storage::Element("75", empty_data));
  tree.Remove("60");

  std::vector<std::string> expected = {
    "55", "0", "50", "70", "0",
    "50", "55", "45", "0", "1",
    "45", "50", "0", "0", "0",
    "70", "55", "65", "75", "0",
    "65", "70", "0", "0", "0",
    "75", "70", "0", "0", "0"
  };

  auto result = tree.GetAllNodes();
  CheckAVL(expected, result);
}

TEST(Transactions, big_remove_test_1) {
  s21::SelfBalancingBinarySearchTree tree;
  std::set<std::string> set = {
    "34", "5346", "323", "76", "45", "235", "2233", "212", "345", "3",
    "36", "765", "43", "457", "3289", "342", "124", "216", "824", "363",
    "92", "3965", "86", "986", "763", "618", "358", "2356", "272", "7892",
    "317", "3682", "4677", "3623", "42", "4", "682", "3408", "1", "412",
    "214", "116"
  };

  std::set<std::string> values = {
    "34", "5346", "323", "76", "45", "235", "2233", "212", "345", "3",
    "36", "765", "43", "457", "3289", "342", "124", "216", "824", "363",
    "92", "3965", "86", "986", "763", "618", "358", "2356", "272", "7892",
    "317", "3682", "4677", "3623", "42", "4", "682", "3408", "1", "412",
    "214", "116"
  };

  for (auto &key : values) {
    tree.Set(s21::Storage::Element(key, empty_data));
  }

  for (auto &str : values) {
    set.erase(str);
    tree.Remove(str);
    ASSERT_FALSE(tree.Exists(str));
    for (auto &key : set) {
      ASSERT_TRUE(tree.Exists(key));
    }
  }
}

TEST(Transactions, big_remove_test_2) {
  s21::SelfBalancingBinarySearchTree tree;
  std::set<std::string> set = {
    "100", "24", "356", "23", "66", "3", "5", "99", "42", "11",
    "35", "13", "85", "79", "1", "45", "54", "12", "76", "6"
  };

  std::vector<std::string> values = {
    "100", "24", "356", "23", "66", "76", "6", "3", "99",
    "35", "13", "85", "79", "1", "45", "54", "12", "5", "42", "11"
  };

  for (auto &key : values) {
    tree.Set(s21::Storage::Element(key, empty_data));
  }

  for (auto &str : values) {
    set.erase(str);
    tree.Remove(str);
    ASSERT_FALSE(tree.Exists(str));
    for (auto &key : set) {
      ASSERT_TRUE(tree.Exists(key));
    }
  }
}

TEST(Transactions, big_remove_test_3) {
  s21::SelfBalancingBinarySearchTree tree;
  std::set<std::string> set;
  std::set<std::string> values = {"212", "3", "567", "12", "125", "99", "3", "515", "356"};

  for (int num = 0; num < 100000; ++num) {
    std::string key = std::to_string(num);
    set.insert(key);
    tree.Set(s21::Storage::Element(key, empty_data));
  }

  for (auto &str : values) {
    ASSERT_TRUE(tree.Exists(str));
  }

  for (auto &str : values) {
    set.erase(str);
    tree.Remove(str);
    ASSERT_FALSE(tree.Exists(str));
    for (auto &key : set) {
      ASSERT_TRUE(tree.Exists(key));
    }
  }
}

void CheckAVL(const std::vector<std::string> &expect,
  const std::vector<s21::SelfBalancingBinarySearchTree::Node*> &result) {
  for (size_t i = 0, c = 0; i < result.size(); ++i) {
    auto key = result[i]->key_.GetKey();
    auto hight = std::to_string(result[i]->balance_);
    std::string parent = "0";
    std::string left = "0";
    std::string right = "0";

    if (result[i]->parent_) parent = result[i]->parent_->key_.GetKey();
    if (result[i]->left_)   left = result[i]->left_->key_.GetKey();
    if (result[i]->right_)  right = result[i]->right_->key_.GetKey();

    ASSERT_EQ(expect[c++], key);
    ASSERT_EQ(expect[c++], parent);
    ASSERT_EQ(expect[c++], left);
    ASSERT_EQ(expect[c++], right);
    ASSERT_EQ(expect[c++], hight);
  }
}
