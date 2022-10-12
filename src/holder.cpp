#include "holder.h"
#include <ctime>
#include <thread>
#include "containers/self_balancing_binary_search_tree.h"
#include "containers/hash_table.h"
#include "containers/b_plus_tree.h"

namespace s21 {

Holder::Holder(const StorageType& type) {
  if (type == Holder::StorageType::kHashTable) {
    storage_ = new HashTable();
  } else if (type == Holder::StorageType::kAVL) {
    storage_ = new SelfBalancingBinarySearchTree();
  } else if (type == Holder::StorageType::kBTree) {
    storage_ = new BPlusTree();
  }
  std::thread cleaner = std::thread(&Holder::LifeTimeRemover, this, std::ref(safe_list_),
                      std::ref(update_), std::ref(is_run_));
  cleaner.detach();
}

Holder::~Holder() {
  if (storage_) {
    is_run_ = false;
    delete storage_;
    storage_ = nullptr;
  }
}

void Holder::Set(element element) {
  std::lock_guard lock(mtx_);
  const int life_time = element.GetData().life_time;
  if (life_time != kDefault_life_time) {
    AddToTemporaryList(element.GetKey(), life_time);
    update_ = true;
  }
  storage_->Set(element);
}

bool Holder::Del(string key) {
  std::lock_guard lock(mtx_);
  const int ttl = storage_->Ttl(key);
  if (ttl != 0 && ttl != kDefault_life_time) {
    RemoveFromTemporaryList(key);
    update_ = true;
  }
  return storage_->Del(key);
}

bool Holder::Rename(string key, string new_key) {
  std::lock_guard lock(mtx_);
  const int ttl = storage_->Ttl(key);
  if (ttl != 0 && ttl != kDefault_life_time) {
    RenameTemporaryKey(key, new_key);
    update_ = true;
  }
  return storage_->Rename(key, new_key);
}

Storage::Element Holder::Get(string key) const {
  std::lock_guard lock(mtx_);
  return storage_->Get(key);
}

bool Holder::Exists(string key) const {
  std::lock_guard lock(mtx_);
  return storage_->Exists(key);
}

bool Holder::Update(string key, const Storage::Element::Data& data) {
  std::lock_guard lock(mtx_);
  return storage_->Update(key, data);
}

std::vector<std::string> Holder::Keys() {
  std::lock_guard lock(mtx_);
  return storage_->Keys();
}

int Holder::Ttl(string key) const {
  std::lock_guard lock(mtx_);
  return storage_->Ttl(key);
}

std::vector<std::string> Holder::Find(const Storage::Element::Data& data) const {
  std::lock_guard lock(mtx_);
  return storage_->Find(data);
}

std::vector<Storage::Element::Data> Holder::ShowAll() {
  std::lock_guard lock(mtx_);
  return storage_->ShowAll();
}

int Holder::Upload(string file_name) {
  std::lock_guard lock(mtx_);
  return storage_->Upload(file_name);
}

int Holder::Export(string file_name) {
  std::lock_guard lock(mtx_);
  return storage_->Export(file_name);
}

void Holder::Init() {
  std::lock_guard lock(mtx_);
  storage_->Init();
}

std::vector<Storage::Element> Holder::AllElements() {
  std::lock_guard lock(mtx_);
  return storage_->AllElements();
}

void Holder::AddToTemporaryList(string key, int time) {
  std::pair<int, std::string> pair(time, key);
  if (safe_list_.GetListSize() > 0) {
    auto it = safe_list_.GetBeginList();
    for (; it != safe_list_.GetEndList(); ++it) {
      if ((*it).first >= time) {
        break;
      }
    }
    safe_list_.InsertList(it, pair);
  } else {
    safe_list_.PushFrontList(pair);
  }
}

void Holder::RemoveFromTemporaryList(string key) {
  for (auto it = safe_list_.GetBeginList(); it != safe_list_.GetEndList(); ++it) {
    if ((*it).second == key) {
      safe_list_.EraseList(it);
      break;
    }
  }
}

void Holder::RenameTemporaryKey(string key, string new_key) {
  for (auto it = safe_list_.GetBeginList(); it != safe_list_.GetEndList(); ++it) {
    if ((*it).second == key) {
      (*it).second = new_key;
      break;
    }
  }
}

void Holder::LifeTimeRemover(Holder::SafeList& list, bool& update, const bool& is_run) {
  const int kMaxInt = 0x7fffffff;
  int time = kMaxInt;
  std::pair<int, std::string> element;

  while (is_run) {
    int current_time = std::time(nullptr);
    if (update) {
      if (list.GetListSize() > 0) {
        element = list.FrontList();
        update = false;
        time = element.first;
      }
    }

    if (current_time >= time) {
      Del(element.second);
      if (list.GetListSize() > 0) {
        element = list.FrontList();
        time = element.first;
      } else {
        time = kMaxInt;
      }
    }
  }
}

/* -------------------------------------------------------------------------- */
/*                               SafeList class                               */
/* -------------------------------------------------------------------------- */

void Holder::SafeList::EraseList(std::list<std::pair<int, std::string>>::iterator iterator) {
  std::lock_guard lock(mutex);
  temporary_keys_list_.erase(iterator);
}

void Holder::SafeList::PushFrontList(std::pair<int, std::string> pair) {
  std::lock_guard lock(mutex);
  temporary_keys_list_.push_front(pair);
}

void Holder::SafeList::InsertList(std::list<std::pair<int, std::string>>::iterator iterator,
                                  std::pair<int, std::string> pair) {
  std::lock_guard lock(mutex);
  temporary_keys_list_.insert(iterator, pair);
}

size_t Holder::SafeList::GetListSize() {
  std::lock_guard lock(mutex);
  return temporary_keys_list_.size();
}

std::list<std::pair<int, std::string>>::iterator Holder::SafeList::GetBeginList() {
  std::lock_guard lock(mutex);
  return temporary_keys_list_.begin();
}

std::list<std::pair<int, std::string>>::iterator Holder::SafeList::GetEndList() {
  std::lock_guard lock(mutex);
  return temporary_keys_list_.end();
}

std::pair<int, std::string> Holder::SafeList::FrontList() {
  std::lock_guard lock(mutex);
  return temporary_keys_list_.front();
}

}  // namespace s21
