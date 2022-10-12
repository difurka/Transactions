#include "hash_table.h"
#include <algorithm>
#include <numeric>

namespace s21 {

using data_t = Storage::Element::Data;

HashTable::HashTable() {
  table_ = std::vector<std::list<Element>>(kSizeOfTable, std::list<Storage::Element>());
}

HashTable::HashTable(HashTable const& other) {
  CopyTable(other);
}

HashTable::HashTable(HashTable&& other) {
  std::list<Element> list_temp;
  for (int i = 0; i <  kSizeOfTable; ++i) {
    list_temp = table_[i];
    table_[i] = other.table_[i];
    other.table_[i] = list_temp;
  }
}

HashTable& HashTable::operator=(HashTable const& other) {
  CopyTable(other);
  return *this;
}

HashTable& HashTable::operator=(HashTable&& other) {
  CopyTable(other);
  return *this;
}

HashTable::~HashTable() {}

inline void HashTable::CopyTable(HashTable const& other) {
  table_ = std::vector<std::list<Element>>(kSizeOfTable, std::list<Storage::Element>());
  if (&other != this) {
    for (int i = 0; i <  kSizeOfTable; ++i) {
      table_[i] = other.table_[i];
    }
  }
}

bool HashTable::Exists(string key) const {
  Element element = Get(key);
  if (element.GetKey() == "") return false;
  return true;
}

void HashTable::Set(element element) {
  unsigned long number_of_bucket = HashFunction(element.GetKey());
  if (std::all_of(table_[number_of_bucket].begin(), table_[number_of_bucket].end(),
          [element](const Element& elem) {return element.GetKey() != elem.GetKey();})) {
    table_[number_of_bucket].push_back(element);
  }
}

Storage::Element HashTable::Get(string key) const {
  Element* element_ref = GetReference(key);
  Element result = *element_ref;
  delete element_ref;
  return result;
}

bool HashTable::Del(string key) {
  unsigned long number_of_bucket = HashFunction(key);
  for (auto element_of_list : table_[number_of_bucket]) {
    if (std::any_of(table_[number_of_bucket].begin(), table_[number_of_bucket].end(),
      [&key](const Element& elem) {
      return elem.GetKey() == key;
    })) {
      table_[number_of_bucket].remove(element_of_list);
      return true;
    }
  }
  return false;
}

bool HashTable::Update(string key, const data_t &data) {
  unsigned long number_of_bucket = HashFunction(key);
  for (auto element_of_list  = table_[number_of_bucket].begin(); element_of_list  !=
                             table_[number_of_bucket].end(); ++element_of_list ) {
    if (element_of_list->GetKey() == key) {
      if (data.surname != "-") element_of_list->SetSurname(data.surname);
      if (data.name != "-") element_of_list->SetName(data.name);
      if (data.year_of_birth != "-") element_of_list->SetYearOfBirth(data.year_of_birth);
      if (data.city != "-") element_of_list->SetCity(data.city);
      if (data.coins != "-") element_of_list->SetCoins(data.coins);
      return true;
    }
  }
  return false;
}

bool HashTable::Rename(string key, string new_key) {
  unsigned long number_of_bucket = HashFunction(key);
  for (auto element_of_list : table_[number_of_bucket]) {
    if (std::any_of(table_[number_of_bucket].begin(), table_[number_of_bucket].end(),
        [&key](const Element& elem) {
    return elem.GetKey() == key;
    })) {
      Element element_for_rename = element_of_list;
      table_[number_of_bucket].remove(element_of_list);
      element_for_rename.SetKey(new_key);
      Set(element_for_rename);
      return true;
    }
  }
  return false;
}

int HashTable::Ttl(string key) const {
  Element element_for_research = Get(key);
  int life_time = 0;
  if (element_for_research.GetKey() != "") life_time = element_for_research.GetLifeTime();
  return life_time;
}

HashTable::vector HashTable::Find(const data_t &data) const {
  HashTable::vector vector_of_key;
  for (auto element_of_table : table_) {
    for (auto element_of_list : element_of_table) {
      if (IsDataSiutable(data, element_of_list.GetData())) vector_of_key.push_back(element_of_list.GetKey());
    }
  }
  return vector_of_key;
}

unsigned long HashTable::HashFunction(const std::string& str) const {
  unsigned long hash = std::accumulate(str.begin(), str.end(), 5381,
                       [](unsigned long currentHash, const char& c) {
                            return currentHash * 128 + currentHash + c;
                        });
  return hash % kSizeOfTable;
}

std::vector<HashTable::Element> HashTable::AllElements() const  {
  std::vector<Element> vector_of_elements;
  for (auto element_of_table : table_) {
     std::transform(element_of_table.begin(), element_of_table.end(), std::back_inserter(vector_of_elements),
                                            [](const Element& elem) {return elem;});
  }
  return vector_of_elements;
}

HashTable::Element* HashTable::GetReference(string key) const {
  unsigned long number_of_bucket = HashFunction(key);
  Element* element = new Element();
  for (auto element_of_list : table_[number_of_bucket]) {
    if (element_of_list.GetKey() == key)
      *element = element_of_list;
  }
  return element;
}

void HashTable::Init() {
  table_ = std::vector<std::list<Element>>(kSizeOfTable, std::list<Storage::Element>());
}
}  // namespace s21
