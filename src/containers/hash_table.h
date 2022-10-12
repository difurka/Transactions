#ifndef SRC_CONTAINERS_HASH_TABLE_H_
#define SRC_CONTAINERS_HASH_TABLE_H_

#include <list>
#include "../storage.h"

namespace s21 {

class HashTable : public Storage {
 public:
  using data_t = Storage::Element::Data;

 public:
  HashTable();
  HashTable(HashTable const&);
  HashTable(HashTable&&);
  HashTable& operator=(HashTable const&);
  HashTable& operator=(HashTable&&);
  ~HashTable();
  void Set(element element) override;
  Element Get(string key) const override;
  bool Exists(string key) const override;
  bool Del(string key) override;
  bool Update(string key, const data_t& data) override;
  bool Rename(string key, string new_key) override;
  int Ttl(string key) const override;
  vector Find(const data_t& data) const override;
  std::vector<Element> AllElements() const override;
  void Init() override;

 private:
  static const int kSizeOfTable = 100;
  std::vector<std::list<Element>> table_;

  unsigned long HashFunction(const std::string& str) const;
  Element* GetReference(string key) const;
  Element& GetAdress(string key);
  inline void CopyTable(HashTable const& other);
};
}  // namespace s21

#endif  // SRC_CONTAINERS_HASH_TABLE_H_
