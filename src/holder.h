#ifndef SRC_HOLDER_H_
#define SRC_HOLDER_H_

#include <string>
#include <mutex>
#include <list>
#include "storage.h"

namespace s21 {

class Holder {
class SafeList;
 public:
  using vector = std::vector<std::string>;
  using string = const std::string &;
  using element = const Storage::Element &;

  enum class StorageType {
    kHashTable,
    kAVL,
    kBTree,
    kEmpty
  };

  explicit Holder(const StorageType& type);
  Holder(const Holder& other) = delete;
  Holder(Holder&& other) = delete;
  Holder& operator=(const Holder& other) = delete;
  Holder& operator=(const Holder&& other) = delete;
  ~Holder();

  void Set(element element);
  Storage::Element Get(string key) const;
  bool Exists(string key) const;
  bool Del(string key);
  bool Update(string key, const Storage::Element::Data& data);
  vector Keys();
  bool Rename(string key, string new_key);
  int Ttl(string key) const;
  vector Find(const Storage::Element::Data& data) const;
  std::vector<Storage::Element::Data> ShowAll();
  int Upload(string file_name);
  int Export(string file_name);
  void Init();
  std::vector<Storage::Element> AllElements();

  void LifeTimeRemover(SafeList& list, bool& update, const bool& is_run);


 private:
  class SafeList {
   public:
    SafeList() = default;
    ~SafeList() = default;

    void EraseList(std::list<std::pair<int, std::string>>::iterator iterator);
    void PushFrontList(std::pair<int, std::string> pair);
    void InsertList(std::list<std::pair<int, std::string>>::iterator iterator,
                    std::pair<int, std::string> pair);
    std::pair<int, std::string> FrontList();
    size_t GetListSize();
    std::list<std::pair<int, std::string>>::iterator GetBeginList();
    std::list<std::pair<int, std::string>>::iterator GetEndList();

   private:
    mutable std::mutex mutex;
    std::list<std::pair<int, std::string>> temporary_keys_list_;
  };

  bool update_ = false;
  bool is_run_ = true;
  static const int kDefault_life_time = -1;
  mutable std::mutex mtx_;
  Storage* storage_;
  SafeList safe_list_;

  void AddToTemporaryList(string key, int time);
  void RemoveFromTemporaryList(string key);
  void RenameTemporaryKey(string key, string new_key);
};

}  // namespace s21

#endif  // SRC_HOLDER_H_
