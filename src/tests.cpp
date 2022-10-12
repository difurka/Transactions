#include <set>
#include <algorithm>
#include "gtest/gtest.h"
#include "containers/hash_table.h"
#include "containers/b_plus_tree.h"
#include "containers/self_balancing_binary_search_tree.h"
#include "storage.h"

/* avl have more tests in "containers/avl_tree_tests.cpp" */

static s21::HashTable table = s21::HashTable();
static s21::BPlusTree b_tree = s21::BPlusTree();
static s21::SelfBalancingBinarySearchTree avl_tree = s21::SelfBalancingBinarySearchTree();
bool FirstVectorIncludesSecond(std::vector<s21::Storage::Element::Data> v1,
                    std::vector<s21::Storage::Element::Data> v2);

TEST(Transactions, set_big_file) {
  table.Upload("./sources/test_110.data");
  b_tree.Upload("./sources/test_110.data");
  avl_tree.Upload("./sources/test_110.data");

  std::vector<s21::Storage::Element::Data> vector_of_elements_table = table.ShowAll();
  std::vector<s21::Storage::Element::Data> vector_of_elements_b_tree = b_tree.ShowAll();
  std::vector<s21::Storage::Element::Data> vector_of_elements_avl_tree = avl_tree.ShowAll();

  EXPECT_EQ(vector_of_elements_table.size(), 110);
  EXPECT_EQ(vector_of_elements_b_tree.size(), 110);
  EXPECT_EQ(vector_of_elements_avl_tree.size(), 110);

  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_table, vector_of_elements_b_tree));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_b_tree, vector_of_elements_avl_tree));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_avl_tree, vector_of_elements_table));

  s21::Storage::vector vector_of_keys = table.Keys();
  table.Export("./sources/test_110_export.data");

  for (auto key : vector_of_keys) {
    table.Del(key);
    avl_tree.Del(key);
    b_tree.Del(key);
  }

  vector_of_elements_table = table.ShowAll();
  vector_of_elements_b_tree = b_tree.ShowAll();
  vector_of_elements_avl_tree = avl_tree.ShowAll();

  EXPECT_EQ(vector_of_elements_table.size(), 0);
  EXPECT_EQ(vector_of_elements_b_tree.size(), 0);
  EXPECT_EQ(vector_of_elements_avl_tree.size(), 0);

  table.Upload("./sources/test_110.data");
  b_tree.Upload("./sources/test_110_export.data");
  avl_tree.Upload("./sources/test_110_export.data");

  vector_of_elements_table = table.ShowAll();
  vector_of_elements_b_tree = b_tree.ShowAll();
  vector_of_elements_avl_tree = avl_tree.ShowAll();

  EXPECT_EQ(vector_of_elements_table.size(), 110);
  EXPECT_EQ(vector_of_elements_b_tree.size(), 110);
  EXPECT_EQ(vector_of_elements_avl_tree.size(), 110);

  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_table, vector_of_elements_b_tree));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_b_tree, vector_of_elements_avl_tree));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_avl_tree, vector_of_elements_table));

  for (auto key : vector_of_keys) {
    avl_tree.Del(key);
    b_tree.Del(key);
  }

  vector_of_elements_b_tree = b_tree.ShowAll();
  vector_of_elements_avl_tree = avl_tree.ShowAll();

  EXPECT_EQ(vector_of_elements_b_tree.size(), 0);
  EXPECT_EQ(vector_of_elements_avl_tree.size(), 0);
}

TEST(Transactions, hash_upload) {
  table.Upload("sources/test_5.data");
  b_tree.Upload("sources/test_5.data");
  s21::Storage::vector vector_of_keys_expected = {"one", "two", "three", "four", "five"};
  s21::Storage::vector vector_of_keys_result = table.Keys();
  s21::Storage::vector vector_result_b_tree = b_tree.Keys();
  EXPECT_EQ(vector_of_keys_expected.size(), vector_of_keys_result.size());
  EXPECT_EQ(vector_of_keys_expected.size(), vector_result_b_tree.size());
  std::set<std::string> set_of_keys_expected(vector_of_keys_expected.begin(),
                                             vector_of_keys_expected.end());
  std::set<std::string> set_of_keys_result(vector_of_keys_result.begin(), vector_of_keys_result.end());
  std::set<std::string> set_of_result_b_tree(vector_result_b_tree.begin(), vector_result_b_tree.end());
  EXPECT_TRUE(set_of_keys_expected == set_of_keys_result);
  EXPECT_TRUE(set_of_keys_expected == set_of_result_b_tree);

  std::vector<s21::Storage::Element::Data> vector_of_elements_expect = {
    {"A", "Aa", "1990", "Novos", "1", 1},
    {"B", "Bb", "2002", "Berdsk", "2", 2},
    {"C", "Cc", "23", "Ob", "3", 4},
    {"D", "Dd", "87", "Inya", "4", 5},
    {"E", "Ee", "566666", "Omsk", "5", 6}
  };
  std::vector<s21::Storage::Element::Data> vector_of_elements_result = table.ShowAll();
  std::vector<s21::Storage::Element::Data> vector_of_elements_result_b_tree = b_tree.ShowAll();
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_result, vector_of_elements_expect));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_expect, vector_of_elements_result));

  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_result_b_tree, vector_of_elements_expect));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_expect, vector_of_elements_result_b_tree));
}

TEST(Transactions, hash_set) {
  table.Init();
  b_tree.Init();
  s21::HashTable::data_t info1 = {"q1", "w1", "e1", "r1", "31", 1};
  s21::Storage::Element element1 = s21::Storage::Element("first", info1);
  table.Set(element1);
  b_tree.Set(element1);

  s21::HashTable::data_t info2 = {"q2", "w2", "e2", "r2", "32", 2};
  s21::Storage::Element element2 = s21::Storage::Element("second", info2);
  table.Set(element2);
  b_tree.Set(element2);

  s21::HashTable::data_t info3 = {"q3", "w3", "e3", "r3", "33", 3};
  s21::Storage::Element element3 = s21::Storage::Element("th", info3);
  table.Set(element3);
  b_tree.Set(element3);

  s21::HashTable::data_t info4 = {"q3", "w3", "e3", "r3", "34", 4};
  s21::Storage::Element element4 = s21::Storage::Element("thoo", info4);
  table.Set(element4);
  b_tree.Set(element4);

  s21::HashTable::data_t info5 = {"q4", "w4", "e3", "r3", "34", 5};
  s21::Storage::Element element5 = s21::Storage::Element("foo", info5);
  table.Set(element5);
  b_tree.Set(element5);

  s21::HashTable::data_t info6 = {"q4", "w4", "e4", "r3", "34", 5};
  s21::Storage::Element element6 = s21::Storage::Element("foo1", info6);
  table.Set(element6);
  b_tree.Set(element6);
}

TEST(Transactions, hash_get_exit_del_ttl) {
  /*--------get----------*/
  s21::HashTable::data_t info4 = {"q3", "w3", "e3", "r3", "34", 4};
  s21::Storage::Element element4 = s21::Storage::Element("thoo", info4);
  s21::Storage::Element el_result1 = table.Get("thoo");
  EXPECT_TRUE(element4 == el_result1);

  el_result1 = b_tree.Get("thoo");
  EXPECT_TRUE(element4 == el_result1);

  s21::Storage::Element el_result2 = table.Get("thoo1");
  EXPECT_EQ(el_result2.GetKey(), "");
  el_result2 = b_tree.Get("thoo1");
  EXPECT_EQ(el_result2.GetKey(), "");

  /*--------exist----------*/
  EXPECT_TRUE(table.Exists("th"));
  EXPECT_TRUE(!table.Exists("thh"));

  EXPECT_TRUE(b_tree.Exists("th"));
  EXPECT_TRUE(!b_tree.Exists("thh"));

  /*--------del----------*/
  table.Del("th");
  EXPECT_TRUE(!table.Exists("th"));
  b_tree.Del("th");
  EXPECT_TRUE(!b_tree.Exists("th"));

  /*--------ttll----------*/
  EXPECT_EQ(table.Ttl("second"), 2);
  EXPECT_EQ(table.Ttl("second_no"), 0);
  EXPECT_EQ(table.Ttl("thoo"), 4);

  EXPECT_EQ(b_tree.Ttl("second"), 2);
  EXPECT_EQ(b_tree.Ttl("second_no"), 0);
  EXPECT_EQ(b_tree.Ttl("thoo"), 4);
}

TEST(Transactions, hash_rename) {
  EXPECT_TRUE(table.Exists("second"));
  EXPECT_TRUE(!table.Exists("second_rename"));
  table.Rename("second", "second_rename");
  EXPECT_TRUE(!table.Exists("second"));
  EXPECT_TRUE(table.Exists("second_rename"));

  EXPECT_TRUE(!table.Exists("second_11"));
  EXPECT_TRUE(!table.Exists("second_rename_11"));
  table.Rename("second_11", "second_rename_11");
  EXPECT_TRUE(!table.Exists("second_11"));
  EXPECT_TRUE(!table.Exists("second_rename_11"));

  EXPECT_TRUE(b_tree.Exists("second"));
  EXPECT_TRUE(!b_tree.Exists("second_rename"));
  b_tree.Rename("second", "second_rename");
  EXPECT_TRUE(!b_tree.Exists("second"));
  EXPECT_TRUE(b_tree.Exists("second_rename"));

  EXPECT_TRUE(!b_tree.Exists("second_11"));
  EXPECT_TRUE(!b_tree.Exists("second_rename_11"));
  b_tree.Rename("second_11", "second_rename_11");
  EXPECT_TRUE(!b_tree.Exists("second_11"));
  EXPECT_TRUE(!b_tree.Exists("second_rename_11"));
}

TEST(Transactions, hash_keys) {
  s21::Storage::vector vector_of_keys_expected = {"first", "second_rename", "thoo", "foo", "foo1"};
  s21::Storage::vector vector_of_keys_result = table.Keys();
  EXPECT_EQ(vector_of_keys_expected.size(), vector_of_keys_result.size());
  std::set<std::string> set_of_keys_expected(vector_of_keys_expected.begin(),
                                                vector_of_keys_expected.end());
  std::set<std::string> set_of_keys_result(vector_of_keys_result.begin(), vector_of_keys_result.end());
  s21::Storage::vector vector_of_keys_no_expected = {"first", "second_rename", "thoo", "foo"};
  std::set<std::string> set_of_no_keys(vector_of_keys_no_expected.begin(),
                                          vector_of_keys_no_expected.end());
  EXPECT_TRUE(set_of_no_keys != set_of_keys_result);
  EXPECT_TRUE(set_of_keys_expected == set_of_keys_result);

  s21::Storage::vector vector_of_result_b_tree = b_tree.Keys();
  EXPECT_EQ(vector_of_keys_expected.size(), vector_of_result_b_tree.size());
  std::set<std::string> set_of_result_b_tree(vector_of_result_b_tree.begin(),
                                                vector_of_result_b_tree.end());
  EXPECT_TRUE(set_of_no_keys != set_of_result_b_tree);
  EXPECT_TRUE(set_of_keys_expected == set_of_result_b_tree);
}

TEST(Transactions, hash_update) {
  s21::HashTable::data_t info_update = {"q_new", "-", "e2", "-", "32_new", 2999};
  table.Update("second_rename", info_update);
  s21::Storage::Element el_result3 = table.Get("second_rename");
  s21::HashTable::data_t info_update_expect = {"q_new", "w2", "e2", "r2", "32_new", 2};
  EXPECT_EQ(info_update_expect, el_result3.GetData());

  b_tree.Update("second_rename", info_update);
  s21::Storage::Element el_result3_b_tree = b_tree.Get("second_rename");
  EXPECT_EQ(info_update_expect, el_result3_b_tree.GetData());
}

TEST(Transactions, hash_find) {
  s21::HashTable::data_t info = {"-", "-", "-", "r3", "-"};
  s21::Storage::vector vector_of_keys_expected = {"thoo", "foo", "foo1"};
  s21::Storage::vector vector_of_keys_result = table.Find(info);
  EXPECT_EQ(vector_of_keys_expected.size(), vector_of_keys_result.size());
  std::set<std::string> set_of_keys_expected(vector_of_keys_expected.begin(),
                                                vector_of_keys_expected.end());
  std::set<std::string> set_of_keys_result(vector_of_keys_result.begin(), vector_of_keys_result.end());
  EXPECT_EQ(set_of_keys_expected, set_of_keys_result);

  s21::HashTable::data_t info1 = {"-", "-", "-", "-", "32_new"};
  s21::Storage::vector vector_of_keys_result1 = table.Find(info1);
  EXPECT_EQ(1, vector_of_keys_result1.size());
  EXPECT_EQ(vector_of_keys_result1[0], "second_rename");



  s21::Storage::vector vector_result_b_tree = b_tree.Find(info);
  EXPECT_EQ(vector_of_keys_expected.size(), vector_result_b_tree.size());
  std::set<std::string> set_result_b_tree(vector_result_b_tree.begin(), vector_result_b_tree.end());
  EXPECT_EQ(set_of_keys_expected, set_result_b_tree);

  s21::Storage::vector vector_of_keys_result1_b_tree = b_tree.Find(info1);
  EXPECT_EQ(1, vector_of_keys_result1_b_tree.size());
  EXPECT_EQ(vector_of_keys_result1_b_tree[0], "second_rename");
}

TEST(Transactions, hash_showall) {
  std::vector<s21::Storage::Element::Data> vector_of_elements_expect = {
    {"q1", "w1", "e1", "r1", "31", 1},
    {"q_new", "w2", "e2", "r2", "32_new", 2},
    {"q3", "w3", "e3", "r3", "34", 4},
    {"q4", "w4", "e3", "r3", "34", 5},
    {"q4", "w4", "e4", "r3", "34", 5}
  };
  std::vector<s21::Storage::Element::Data> vector_of_elements_result = table.ShowAll();
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_result, vector_of_elements_expect));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_expect, vector_of_elements_result));

  std::vector<s21::Storage::Element::Data> vector_result_b_tree = b_tree.ShowAll();
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_result_b_tree, vector_of_elements_expect));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_expect, vector_result_b_tree));

  std::vector<s21::Storage::Element::Data> vector_of_elements_not_expect = {
    {"q1", "w1", "e1", "r1", "31", 1},
    {"q_new", "w2", "e2", "r2", "32_new", 2},
    {"q3", "w3", "e3", "r3", "34", 4},
    {"q4", "w4", "e3", "r3", "34", 5}
  };
  EXPECT_TRUE(!FirstVectorIncludesSecond(vector_of_elements_not_expect, vector_result_b_tree));
}

TEST(Transactions, hash_export) {
  table.Init();
  s21::HashTable::data_t info1 = {"q1", "w1", "1", "r1", "31", 1};
  s21::Storage::Element element1 = s21::Storage::Element("first", info1);
  table.Set(element1);

  s21::HashTable::data_t info2 = {"q2", "w2", "2", "r2", "32", 2};
  s21::Storage::Element element2 = s21::Storage::Element("second", info2);
  table.Set(element2);

  s21::HashTable::data_t info3 = {"q3", "w3", "3", "r3", "33", 3};
  s21::Storage::Element element3 = s21::Storage::Element("th", info3);
  table.Set(element3);

  s21::HashTable::data_t info4 = {"q3", "w3", "3", "r3", "34", 4};
  s21::Storage::Element element4 = s21::Storage::Element("thoo", info4);
  table.Set(element4);

  s21::HashTable::data_t info5 = {"q4", "w4", "3", "r3", "34", 5};
  s21::Storage::Element element5 = s21::Storage::Element("foo", info5);
  table.Set(element5);

  s21::HashTable::data_t info6 = {"q4", "w4", "4", "r3", "34", 5};
  s21::Storage::Element element6 = s21::Storage::Element("foo1", info6);
  table.Set(element6);
  table.Export("sources/test_export.data");
  table.Upload("sources/test_export.data");
  std::vector<s21::Storage::Element::Data> vector_of_elements_expect = {
    {"q1", "w1", "1", "r1", "31", 1},
    {"q2", "w2", "2", "r2", "32", 2},
    {"q3", "w3", "3", "r3", "33", 3},
    {"q3", "w3", "3", "r3", "34", 4},
    {"q4", "w4", "3", "r3", "34", 5},
    {"q4", "w4", "4", "r3", "34", 5}
  };
  std::vector<s21::Storage::Element::Data> vector_of_elements_result = table.ShowAll();
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_result, vector_of_elements_expect));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_expect, vector_of_elements_result));

  b_tree.Init();
  b_tree.Set(element1);
  b_tree.Set(element2);
  b_tree.Set(element3);
  b_tree.Set(element4);
  b_tree.Set(element5);
  b_tree.Set(element6);
  b_tree.Export("sources/test_export.data");
  b_tree.Upload("sources/test_export.data");

  std::vector<s21::Storage::Element::Data> vector_result_b_tree = b_tree.ShowAll();
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_result_b_tree, vector_of_elements_expect));
  EXPECT_TRUE(FirstVectorIncludesSecond(vector_of_elements_expect, vector_result_b_tree));
}

/* -------------------------------------------------------------------------- */
/*                            virtual methods test                            */
/* -------------------------------------------------------------------------- */

std::vector<s21::Storage::Element> elements = {
  {"key1", {"surname_1", "name_1", "1999", "City_1", "15", 0}},
  {"key2", {"surname_2", "name_2", "2001", "City_1", "25", 0}},
  {"key3", {"surname_3", "name_3", "2001", "City_3", "35", 0}},
  {"key4", {"surname_4", "name_1", "2005", "City_3", "45", 0}},
  {"key5", {"surname_5", "name_2", "1990", "City_4", "0", 0}},
  {"key6", {"surname_6", "name_3", "1997", "City_5", "15", 0}},
  {"key7", {"surname_7", "name_1", "1998", "City_2", "15", 0}},
  {"key8", {"surname_8", "name_2", "1989", "City_6", "5", 0}},
  {"key9", {"surname_9", "name_3", "1999", "City_1", "5", 0}}
};

TEST(Transactions, avl_move_copy) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::SelfBalancingBinarySearchTree AVL_;

  for (auto &element : elements) {
    AVL.Set(element);
    AVL_.Set(element);
  }
  s21::SelfBalancingBinarySearchTree AVL2 = AVL;
  s21::SelfBalancingBinarySearchTree AVL3(AVL);
  s21::SelfBalancingBinarySearchTree AVL4(std::move(AVL_));

  auto result = AVL.AllElements();
  for (auto &elem : result) {
    ASSERT_TRUE(AVL2.Exists(elem.GetKey()));
    ASSERT_TRUE(AVL3.Exists(elem.GetKey()));
    ASSERT_TRUE(AVL4.Exists(elem.GetKey()));
  }
}

TEST(Transactions, set) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;
  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
  }

  for (size_t k = 0; k < elements.size(); ++k) {
    ASSERT_TRUE(AVL.Exists(elements[k].GetKey()));
    ASSERT_TRUE(hash_table.Exists(elements[k].GetKey()));
    ASSERT_TRUE(b_treee.Exists(elements[k].GetKey()));
  }
}

TEST(Transactions, get) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;
  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
  }

  auto result = AVL.Get("key4");
  ASSERT_EQ(result.GetData(), elements[3].GetData());
  result = AVL.Get("key9");
  ASSERT_EQ(result.GetData(), elements[8].GetData());
  result = AVL.Get("key7");
  ASSERT_EQ(result.GetData(), elements[6].GetData());

  result = hash_table.Get("key4");
  ASSERT_EQ(result.GetData(), elements[3].GetData());
  result = hash_table.Get("key9");
  ASSERT_EQ(result.GetData(), elements[8].GetData());
  result = hash_table.Get("key7");
  ASSERT_EQ(result.GetData(), elements[6].GetData());

  result = b_treee.Get("key4");
  ASSERT_EQ(result.GetData(), elements[3].GetData());
  result = b_treee.Get("key9");
  ASSERT_EQ(result.GetData(), elements[8].GetData());
  result = b_treee.Get("key7");
  ASSERT_EQ(result.GetData(), elements[6].GetData());
}

TEST(Transactions, exists) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;
  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
  }

  ASSERT_FALSE(AVL.Exists("unknown_key"));
  ASSERT_FALSE(AVL.Exists("key"));
  ASSERT_FALSE(AVL.Exists("key 6"));
  ASSERT_TRUE(AVL.Exists("key6"));

  ASSERT_FALSE(hash_table.Exists("unknown_key"));
  ASSERT_FALSE(hash_table.Exists("key"));
  ASSERT_FALSE(hash_table.Exists("key 6"));
  ASSERT_TRUE(hash_table.Exists("key6"));

  ASSERT_FALSE(b_treee.Exists("unknown_key"));
  ASSERT_FALSE(b_treee.Exists("key"));
  ASSERT_FALSE(b_treee.Exists("key 6"));
  ASSERT_TRUE(b_treee.Exists("key6"));
}

TEST(Transactions, del) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;
  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
  }

  for (int n = 8; n >= 0; --n) {
    ASSERT_TRUE(AVL.Exists(elements[n].GetKey()));
    ASSERT_TRUE(AVL.Del(elements[n].GetKey()));
    ASSERT_FALSE(AVL.Exists(elements[n].GetKey()));
    ASSERT_FALSE(AVL.Del(elements[n].GetKey()));

    ASSERT_TRUE(hash_table.Exists(elements[n].GetKey()));
    ASSERT_TRUE(hash_table.Del(elements[n].GetKey()));
    ASSERT_FALSE(hash_table.Exists(elements[n].GetKey()));
    ASSERT_FALSE(hash_table.Del(elements[n].GetKey()));

    ASSERT_TRUE(b_treee.Exists(elements[n].GetKey()));
    ASSERT_TRUE(b_treee.Del(elements[n].GetKey()));
    ASSERT_FALSE(b_treee.Exists(elements[n].GetKey()));
    ASSERT_FALSE(b_treee.Del(elements[n].GetKey()));
  }
}

TEST(Transactions, update) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;

  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
  }

  ASSERT_TRUE(
  AVL.Update("key4", s21::Storage::Element::Data{"change_1", "name_1", "2005", "City_3", "45", 0}));
  auto result = AVL.Get("key4");
  ASSERT_EQ(result.GetSurname(), "change_1");
  ASSERT_EQ(result.GetYearOfBirth(), elements[3].GetYearOfBirth());
  ASSERT_EQ(result.GetName(), elements[3].GetName());
  ASSERT_EQ(result.GetCity(), elements[3].GetCity());
  ASSERT_EQ(result.GetCoins(), elements[3].GetCoins());

  ASSERT_TRUE(
  AVL.Update("key8", s21::Storage::Element::Data{"surname_8", "name_2", "1989", "change_2", "5", 0}));
  result = AVL.Get("key8");
  ASSERT_EQ(result.GetSurname(), elements[7].GetSurname());
  ASSERT_EQ(result.GetYearOfBirth(), elements[7].GetYearOfBirth());
  ASSERT_EQ(result.GetName(), elements[7].GetName());
  ASSERT_EQ(result.GetCity(), "change_2");
  ASSERT_EQ(result.GetCoins(), elements[7].GetCoins());

  ASSERT_TRUE(
  AVL.Update("key2", s21::Storage::Element::Data{"surname_2", "name_2", "2022", "City_1", "25", 0}));
  result = AVL.Get("key2");
  ASSERT_EQ(result.GetSurname(), elements[1].GetSurname());
  ASSERT_EQ(result.GetYearOfBirth(), "2022");
  ASSERT_EQ(result.GetName(), elements[1].GetName());
  ASSERT_EQ(result.GetCity(), elements[1].GetCity());
  ASSERT_EQ(result.GetCoins(), elements[1].GetCoins());

  ASSERT_TRUE(
  hash_table.Update("key4", s21::Storage::Element::Data{"change_1", "name_1", "2005", "City_3", "45", 0}));
  result = hash_table.Get("key4");
  ASSERT_EQ(result.GetSurname(), "change_1");
  ASSERT_EQ(result.GetYearOfBirth(), elements[3].GetYearOfBirth());
  ASSERT_EQ(result.GetName(), elements[3].GetName());
  ASSERT_EQ(result.GetCity(), elements[3].GetCity());
  ASSERT_EQ(result.GetCoins(), elements[3].GetCoins());

  ASSERT_TRUE(
  hash_table.Update("key8", s21::Storage::Element::Data{"surname_8", "name_2", "1989", "change_2", "5", 0}));
  result = hash_table.Get("key8");
  ASSERT_EQ(result.GetSurname(), elements[7].GetSurname());
  ASSERT_EQ(result.GetYearOfBirth(), elements[7].GetYearOfBirth());
  ASSERT_EQ(result.GetName(), elements[7].GetName());
  ASSERT_EQ(result.GetCity(), "change_2");
  ASSERT_EQ(result.GetCoins(), elements[7].GetCoins());

  ASSERT_TRUE(
  hash_table.Update("key2", s21::Storage::Element::Data{"surname_2", "name_2", "2022", "City_1", "25", 0}));
  result = hash_table.Get("key2");
  ASSERT_EQ(result.GetSurname(), elements[1].GetSurname());
  ASSERT_EQ(result.GetYearOfBirth(), "2022");
  ASSERT_EQ(result.GetName(), elements[1].GetName());
  ASSERT_EQ(result.GetCity(), elements[1].GetCity());
  ASSERT_EQ(result.GetCoins(), elements[1].GetCoins());

  ASSERT_TRUE(
  b_treee.Update("key4", s21::Storage::Element::Data{"change_1", "name_1", "2005", "City_3", "45", 0}));
  result = b_treee.Get("key4");
  ASSERT_EQ(result.GetSurname(), "change_1");
  ASSERT_EQ(result.GetYearOfBirth(), elements[3].GetYearOfBirth());
  ASSERT_EQ(result.GetName(), elements[3].GetName());
  ASSERT_EQ(result.GetCity(), elements[3].GetCity());
  ASSERT_EQ(result.GetCoins(), elements[3].GetCoins());

  ASSERT_TRUE(
  b_treee.Update("key8", s21::Storage::Element::Data{"surname_8", "name_2", "1989", "change_2", "5", 0}));
  result = b_treee.Get("key8");
  ASSERT_EQ(result.GetSurname(), elements[7].GetSurname());
  ASSERT_EQ(result.GetYearOfBirth(), elements[7].GetYearOfBirth());
  ASSERT_EQ(result.GetName(), elements[7].GetName());
  ASSERT_EQ(result.GetCity(), "change_2");
  ASSERT_EQ(result.GetCoins(), elements[7].GetCoins());

  ASSERT_TRUE(
  b_treee.Update("key2", s21::Storage::Element::Data{"surname_2", "name_2", "2022", "City_1", "25", 0}));
  result = b_treee.Get("key2");
  ASSERT_EQ(result.GetSurname(), elements[1].GetSurname());
  ASSERT_EQ(result.GetYearOfBirth(), "2022");
  ASSERT_EQ(result.GetName(), elements[1].GetName());
  ASSERT_EQ(result.GetCity(), elements[1].GetCity());
  ASSERT_EQ(result.GetCoins(), elements[1].GetCoins());
}

TEST(Transactions, keys) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;

  std::set<std::string> keys;
  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
    keys.insert(elements[i].GetKey());
  }

  auto all_keys = AVL.Keys();
  std::set<std::string> keys_2(keys);
  for (size_t k = 0; k < elements.size(); ++k) {
    keys.erase(all_keys[k]);
  }
  ASSERT_EQ(keys.size(), 0);

  all_keys = hash_table.Keys();
  keys = keys_2;
  for (size_t k = 0; k < elements.size(); ++k) {
    keys_2.erase(all_keys[k]);
  }
  ASSERT_EQ(keys_2.size(), 0);

  all_keys = b_treee.Keys();
  for (size_t k = 0; k < elements.size(); ++k) {
    keys.erase(all_keys[k]);
  }
  ASSERT_EQ(keys.size(), 0);
}

TEST(Transactions, rename) {
  s21::SelfBalancingBinarySearchTree AVL;
  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
  }

  ASSERT_TRUE(AVL.Exists("key3"));
  ASSERT_FALSE(AVL.Exists("key10"));
  AVL.Rename("key3", "key10");
  ASSERT_TRUE(AVL.Exists("key10"));
  ASSERT_FALSE(AVL.Exists("key3"));
  ASSERT_EQ(elements[2].GetData(), AVL.Get("key10").GetData());

  s21::HashTable hash_table;
  for (size_t i = 0; i < elements.size(); ++i) {
    hash_table.Set(elements[i]);
  }

  ASSERT_TRUE(hash_table.Exists("key3"));
  ASSERT_FALSE(hash_table.Exists("key10"));
  hash_table.Rename("key3", "key10");
  ASSERT_TRUE(hash_table.Exists("key10"));
  ASSERT_FALSE(hash_table.Exists("key3"));
  ASSERT_EQ(elements[2].GetData(), hash_table.Get("key10").GetData());

  s21::BPlusTree b_treee;
  for (size_t i = 0; i < elements.size(); ++i) {
    b_treee.Set(elements[i]);
  }

  ASSERT_TRUE(b_treee.Exists("key3"));
  ASSERT_FALSE(b_treee.Exists("key10"));
  b_treee.Rename("key3", "key10");
  ASSERT_TRUE(b_treee.Exists("key10"));
  ASSERT_FALSE(b_treee.Exists("key3"));
  ASSERT_EQ(elements[2].GetData(), b_treee.Get("key10").GetData());
}

TEST(Transactions, showall) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;

  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
  }
  auto result = AVL.ShowAll();
  auto all_elem = AVL.AllElements();
  for (size_t k = 0; k < elements.size(); ++k) {
    ASSERT_EQ(result[k], all_elem[k].GetData());
  }

  result = hash_table.ShowAll();
  all_elem = hash_table.AllElements();
  for (size_t k = 0; k < elements.size(); ++k) {
    ASSERT_EQ(result[k], all_elem[k].GetData());
  }

  result = b_treee.ShowAll();
  all_elem = b_treee.AllElements();
  for (size_t k = 0; k < elements.size(); ++k) {
    ASSERT_EQ(result[k], all_elem[k].GetData());
  }
}

TEST(Transactions, find) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;

  for (size_t i = 0; i < elements.size(); ++i) {
    AVL.Set(elements[i]);
    hash_table.Set(elements[i]);
    b_treee.Set(elements[i]);
  }

  auto result_a = AVL.Find(s21::Storage::Element::Data{"-", "name_1", "-", "-", "-", 0});
  auto result_h = hash_table.Find(s21::Storage::Element::Data{"-", "name_1", "-", "-", "-", 0});
  auto result_b = b_treee.Find(s21::Storage::Element::Data{"-", "name_1", "-", "-", "-", 0});
  std::vector<std::string> expect = {"key4", "key1", "key7"};
  std::vector<std::string> expect_h = {"key1", "key4", "key7"};
  std::vector<std::string> expect_b = {"key4", "key1", "key7"};

  for (size_t k = 0; k < result_a.size(); ++k) {
    ASSERT_EQ(expect[k], result_a[k]);
    ASSERT_EQ(expect_h[k], result_h[k]);
    ASSERT_EQ(expect_b[k], result_b[k]);
  }

  result_a = AVL.Find(s21::Storage::Element::Data{"-", "name_1", "-", "-", "15", 0});
  result_h = hash_table.Find(s21::Storage::Element::Data{"-", "name_1", "-", "-", "15", 0});
  result_b = b_treee.Find(s21::Storage::Element::Data{"-", "name_1", "-", "-", "15", 0});
  expect = { "key1", "key7"};

  for (size_t k = 0; k < result_a.size(); ++k) {
    ASSERT_EQ(expect[k], result_a[k]);
    ASSERT_EQ(expect[k], result_h[k]);
    ASSERT_EQ(expect[k], result_b[k]);
  }

  result_a = AVL.Find(s21::Storage::Element::Data{"-", "-", "1999", "-", "-", 0});
  result_h = hash_table.Find(s21::Storage::Element::Data{"-", "-", "1999", "-", "-", 0});
  result_b = b_treee.Find(s21::Storage::Element::Data{"-", "-", "1999", "-", "-", 0});
  expect = { "key1", "key9"};

  for (size_t k = 0; k < result_a.size(); ++k) {
    ASSERT_EQ(expect[k], result_a[k]);
    ASSERT_EQ(expect[k], result_h[k]);
    ASSERT_EQ(expect[k], result_b[k]);
  }

  result_a = AVL.Find(s21::Storage::Element::Data{"-", "-", "-", "City_1", "-", 0});
  result_h = hash_table.Find(s21::Storage::Element::Data{"-", "-", "-", "City_1", "-", 0});
  result_b = b_treee.Find(s21::Storage::Element::Data{"-", "-", "-", "City_1", "-", 0});
  expect = {"key2", "key1", "key9"};
  std::vector<std::string> expect_hash = {"key1", "key2", "key9"};

  for (size_t k = 0; k < result_a.size(); ++k) {
    ASSERT_EQ(expect[k], result_a[k]);
    ASSERT_EQ(expect_hash[k], result_h[k]);
    ASSERT_EQ(expect[k], result_b[k]);
  }
}

TEST(Transactions, ttl) {
  s21::SelfBalancingBinarySearchTree AVL;
  s21::HashTable hash_table;
  s21::BPlusTree b_treee;
  std::vector<s21::Storage::Element> time_element {
    {{"tkey1"}, {"sname", "name", "2002", "City1", "10", 200}},
    {{"tkey2"}, {"sname", "name", "2002", "City1", "10", 150}},
    {{"key3"}, {"sname", "name", "2002", "City1", "10", -1}},
    {{"key4"}, {"sname", "name", "2002", "City1", "10", -1}},
    {{"tkey5"}, {"sname", "name", "2002", "City1", "10", 5000}}
  };

  for (size_t i = 0; i < time_element.size(); ++i) {
    AVL.Set(time_element[i]);
    hash_table.Set(time_element[i]);
    b_treee.Set(time_element[i]);
  }
  ASSERT_EQ(AVL.Ttl("tkey5"), 5000);
  ASSERT_EQ(hash_table.Ttl("tkey5"), 5000);
  ASSERT_EQ(b_treee.Ttl("tkey5"), 5000);

  ASSERT_EQ(AVL.Ttl("tkey2"), 150);
  ASSERT_EQ(hash_table.Ttl("tkey2"), 150);
  ASSERT_EQ(b_treee.Ttl("tkey2"), 150);

  ASSERT_EQ(AVL.Ttl("tkey1"), 200);
  ASSERT_EQ(hash_table.Ttl("tkey1"), 200);
  ASSERT_EQ(b_treee.Ttl("tkey1"), 200);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* -------------------------------------------------------------------------- */
/*                              helper functions                              */
/* -------------------------------------------------------------------------- */

bool FirstVectorIncludesSecond(std::vector<s21::Storage::Element::Data> v1,
                    std::vector<s21::Storage::Element::Data> v2) {
  if (v2.size() > v1.size()) return false;
  for (auto el_v2 : v2) {
    bool result_equal_elements = false;
    if (std::any_of(v1.begin(), v1.end(), [&el_v2](const s21::Storage::Element::Data& el_v1) {
       return el_v1 == el_v2;
    })) {
        result_equal_elements = true;
      }
      if (result_equal_elements == false) {
        return false;
      }
    }
  return true;
}
