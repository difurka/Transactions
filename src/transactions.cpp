#include "transactions.h"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <regex>
#include <iomanip>
#include <random>
#include <chrono>
#include <deque>
#include "containers/self_balancing_binary_search_tree.h"
#include "containers/hash_table.h"
#include "containers/b_plus_tree.h"

namespace s21 {

void Transactions::Start() {
  Initialize(Holder::Holder::StorageType::kAVL);
  std::string line;
  PrintMenu();
  while (line != "EXIT" && line != "exit" && !std::cin.eof()) {
    try {
      std::getline(std::cin, line);
      ReadCommandFromConsole(line);
    } catch (std::exception& ex) {
      std::cout << "ERROR: " << ex.what() << std::endl;
    }
  }
  Clear();
}

void Transactions::ReadCommandFromConsole(const std::string& command) {
  if ((command == "HELP") || (command == "help")) {
    PrintMenu();
    Print(kShowHelp);
  } else if (command == "HT" || command == "ht") {
    if (Initialize(Holder::StorageType::kHashTable)) {
      SetDefaultPrintSettings();
      Print(kSuccess);
      Print(kStorageHash);
    }
  } else if (command == "AVL" || command == "avl") {
    if (Initialize(Holder::StorageType::kAVL)) {
      SetDefaultPrintSettings();
      Print(kSuccess);
      Print(kStorageAVL);
    }
  } else if (command == "BT" || command == "bt") {
    if (Initialize(Holder::StorageType::kBTree)) {
      SetDefaultPrintSettings();
      Print(kSuccess);
      Print(kStorageBTree);
    }
  } else if (command == "EXIT" || command == "exit") {
    std::cout << "2022 School 21 - Transactions" << std::endl;
  } else {
    TableCommandExecutor(command);
  }
}

void Transactions::TableCommandExecutor(const std::string& command) {
  if (std::regex_search(command, std::regex(regex_[kSet]))) {
    AddElementToStorage(command);
  } else if (std::regex_search(command, std::regex(regex_[kGet]))) {
    GetElement(command);
  } else if (std::regex_search(command, std::regex(regex_[kExists]))) {
    CheckExistsElement(command);
  } else if (std::regex_search(command, std::regex(regex_[kDel]))) {
    DeleteElement(command);
  } else if (std::regex_search(command, std::regex(regex_[kUpdate]))) {
    UpdateElement(command);
  } else if (command == "KEYS" || command == "keys") {
    ShowAllKeys();
  } else if (std::regex_search(command, std::regex(regex_[kRename]))) {
    RenameKey(command);
  } else if (std::regex_search(command, std::regex(regex_[kTtl]))) {
    ShowTtl(command);
  } else if (std::regex_search(command, std::regex(regex_[kFind]))) {
    FindElement(command);
  } else if (command == "SHOWALL" || command == "showall") {
    ShowAllElements();
  } else if (std::regex_search(command, std::regex(regex_[kUpload]))) {
    ImportFromFile(command);
  } else if (std::regex_search(command, std::regex(regex_[kExport]))) {
    ExportToFile(command);
  } else if (std::regex_search(command, std::regex(regex_[kCompare]))) {
    MakeStorageCompare(command);
  } else {
    std::cout << "ERROR: invalid command" << std::endl;
  }
}

void Transactions::AddElementToStorage(const std::string& command) {
  auto tokens = Parser(command);
  if (!storage_->Exists(tokens[1])) {
    int life_time = kDefault_life_time;
    if (IsDigital(tokens[4]) && IsDigital(tokens[6])) {
      if (tokens.size() == 9 && IsDigital(tokens[8])) {
        int time = std::stoi(tokens[8]);
        life_time = time != 0 ? time + std::time(nullptr) : 0;
      }
      if (life_time != 0) {
        Storage::Element element =
          {{tokens[1]}, {tokens[2], tokens[3], tokens[4], tokens[5], tokens[6], life_time}};
        storage_->Set(element);
        ResetLengthPrintSettings(tokens);
        ++size_;
        std::cout << "OK" << std::endl;
      } else {
        std::cout << "invalid argument";
      }
    }
  } else {
    std::cout << "key: \"" << tokens[1] + "\" is already exists." << std::endl;
  }
}

void Transactions::UpdateElement(const std::string& command) {
  auto tokens = Parser(command);
  if ((tokens[4] == "-" || IsDigital(tokens[4])) && (tokens[6] == "-" || IsDigital(tokens[6]))) {
    bool result =
      storage_->Update(tokens[1], {tokens[2], tokens[3], tokens[4], tokens[5], tokens[6],
        kDefault_life_time});
    if (result) {
      std::cout << "OK" << std::endl;
      ResetLengthPrintSettings(tokens);
    } else {
      Print(kNotExist);
    }
  }
}

void Transactions::GetElement(const std::string& command) {
  auto tokens = Parser(command);
  Storage::Element element = storage_->Get(tokens[1]);
  if (element.GetKey() == "") {
    std::cout << "(null)" << std::endl;
  } else {
    PrintElement(element);
  }
}

void Transactions::CheckExistsElement(const std::string& command) {
  auto tokens = Parser(command);
  std::string result = (storage_->Exists(tokens[1])) ? "true" : "false";
  std::cout << result << std::endl;
}

void  Transactions::DeleteElement(const std::string& command) {
  auto tokens = Parser(command);
  std::string result = (storage_->Del(tokens[1])) ? "true" : "false";
  if (result == "true") --size_;
  std::cout << result << std::endl;
}

void Transactions::FindElement(const std::string& command) {
  auto tokens = Parser(command);
  if ((tokens[3] == "-" || IsDigital(tokens[3])) && (tokens[5] == "-" || IsDigital(tokens[5]))) {
    std::vector<std::string> result
      = storage_->Find({tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], kDefault_life_time});
    if (result.size() > 0) {
      size_t num = 1;
      for (auto &element : result) {
        std::cout << num++ << ") " << element << std::endl;
      }
    } else {
      std::cout << "no matches found" << std::endl;
    }
  }
}

void Transactions::ShowAllKeys() {
  auto elements = storage_->AllElements();
  size_t size = elements.size();
  if (size > 0) {
    for (size_t i = 0; i < elements.size(); ++i) {
      std::cout << (i + 1) << ") " << elements[i].GetKey() << std::endl;
    }
  } else {
    std::cout << "No elements" << std::endl;
  }
}

void Transactions::RenameKey(const std::string& command) {
  auto tokens = Parser(command);
  std::string result = (storage_->Rename(tokens[1], tokens[2])) ? "true" : "false";
  std::cout << result << std::endl;
}

void Transactions::ShowTtl(const std::string& command) {
  auto tokens = Parser(command);
  int result = storage_->Ttl(tokens[1]);
  if (result == 0) {
    Print(kNotExist);
  } else if (result > 0) {
    result -= std::time(nullptr);
    std::cout << result << std::endl;
  } else {
    std::cout << "infinity" << std::endl;
  }
}

void Transactions::ShowAllElements() {
  auto elements = storage_->AllElements();
  if (elements.size() > 0) {
    PrintTableHead();
    for (size_t i = 0; i < elements.size(); ++i) {
      std::cout << std::setw(lengths_.number) << std::left << (i + 1);
      PrintElement(elements[i], true);
    }
  } else {
    std::cout << "No elements" << std::endl;
  }
}

void Transactions::ExportToFile(const std::string& command) {
  auto tokens = Parser(command);
  int counter = storage_->Export(tokens[1]);
  if (counter >= 0) {
    std::cout << "OK " << counter << std::endl;;
  }
}

void Transactions::ImportFromFile(const std::string& command) {
  auto tokens = Parser(command);
  int counter = storage_->Upload(tokens[1]);
  if (counter >= 0) {
    size_ = counter;
    std::cout << "OK " << counter << std::endl;
  }
}

std::vector<std::string> Transactions::Parser(const std::string& command) {
  std::vector<std::string> result;

  std::istringstream string_stream(RemoveSpaces(command));
  std::string token;
  while (std::getline(string_stream, token, ' ')) {
    result.push_back(token);
  }
  return result;
}

std::string Transactions::RemoveSpaces(const std::string& command) {
  std::regex sample("[ ]{1,}");
  std::string result;
  std::regex_replace(std::back_inserter(result),
                    command.begin(), command.end(), sample, " ");
  return result;
}

bool Transactions::IsDigital(const std::string& line) {
  for (auto &ch : line) {
    if (!std::isdigit(ch)) {
      std::cout << "ERROR: unable to cast value \"" << line << "\" to type int" << std::endl;
      return false;
    }
  }
  return true;
}

void Transactions::ResetLengthPrintSettings(const std::vector<std::string> &tokens) {
  if (tokens[1].length() > lengths_.surname) lengths_.surname = tokens[1].length();
  if (tokens[2].length() > lengths_.name) lengths_.name = tokens[2].length();
  if (tokens[3].length() > lengths_.year) lengths_.year = tokens[3].length();
  if (tokens[4].length() > lengths_.city) lengths_.city = tokens[4].length();
  if (tokens[5].length() > lengths_.coins) lengths_.coins = tokens[5].length();
}

void Transactions::SetDefaultPrintSettings() {
  lengths_.number = 6;
  lengths_.surname = 15;
  lengths_.name = 15;
  lengths_.year = 6;
  lengths_.city = 15;
  lengths_.coins = 4;
}

void Transactions::PrintElement(const Storage::Element &element, bool is_table_print) {
  auto data = element.GetData();
  if (is_table_print) {
    std::cout
    << std::setw(lengths_.surname) << std::left << data.surname
    << std::setw(lengths_.name) << std::left << data.name
    << std::setw(lengths_.year) << std::left << data.year_of_birth
    << std::setw(lengths_.city) << std::left << data.city
    << std::setw(lengths_.coins) << std::left << data.coins << std::endl;
  } else {
    std::cout
    << data.surname << " "
    << data.name << " "
    << data.year_of_birth << " "
    << data.city << " "
    << data.coins << std::endl;
  }
}

void Transactions::PrintTableHead() {
  {
    const size_t num_length = std::to_string(size_).length();
    if (num_length > lengths_.number) lengths_.number = num_length + 1;
    std::cout
    << std::setw(lengths_.number) << std::left << "№ "
    << std::setw(lengths_.surname) << std::left << "|Surname"
    << std::setw(lengths_.name) << std::left << "|Name"
    << std::setw(lengths_.year) << std::left << "|Year"
    << std::setw(lengths_.city) << std::left << "|City"
    << std::setw(lengths_.coins) << std::left << "|Coints"
    << std::endl;
  }
}

bool Transactions::Initialize(const Holder::StorageType &type) {
  if (storage_) Clear();
  if (type_ != type) {
    storage_ = new Holder(type);
    type_ = type;
    return true;
  }
  return false;
}

void Transactions::Clear() {
  delete storage_;
  storage_ = nullptr;
}

void Transactions::PrintMenu() {
  Print(kWelcome);
  Print(kSwitch);
  Print(kStorageHash);
  Print(kStorageAVL);
  Print(kStorageBTree);
  Print(kMakeCompare);
}

inline void Transactions::Print(const Message &message) {
  std::cout << messages[message];
  if (message == kStorageAVL || message == kStorageHash || message == kStorageBTree) {
    if ((message == kStorageAVL && type_ == Holder::StorageType::kAVL)
    || (message == kStorageHash && type_ == Holder::StorageType::kHashTable)
    || (message == kStorageBTree && type_ == Holder::StorageType::kBTree)) {
      std::cout << messages[kActiv];
    }
  }
  std::cout << std::endl;
}

/* -------------------------------------------------------------------------- */
/*                                Compare test                                */
/* -------------------------------------------------------------------------- */

void Transactions::MakeStorageCompare(const std::string& command) {
  auto tokens = Parser(command);
  if (IsDigital(tokens[1]) && IsDigital(tokens[2])) {
    const int count_of_elements = std::stoi(tokens[1]);
    const int counter = std::stoi(tokens[2]);
    if (count_of_elements > 0 && counter > 0) {
      auto elements = CreateElements(count_of_elements, "key");
      StartTest(counter, elements);
    } else {
      std::cout << "ERROR: invalid size" << std::endl;
    }
  }
}

int Transactions::GetRandomNumber(int min, int max) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> uni(min, max);
  return uni(rng);
}

std::vector<Storage::Element> Transactions::CreateElements(int count_of_elements,
                                                          const std::string& prefix) {
  std::vector<Storage::Element> elements;

  while (count_of_elements > 0) {
    const std::string key = prefix + std::to_string(count_of_elements);
    const std::string surname = std::to_string(GetRandomNumber(0, 999));
    const std::string name = std::to_string(GetRandomNumber(0, 50));
    const std::string year = std::to_string(GetRandomNumber(1922, 2022));
    const std::string city = std::to_string(GetRandomNumber(0, 99));
    const std::string coins = std::to_string(GetRandomNumber(0, 99));
    Storage::Element element = {{key}, {surname, name, year, city, coins, kDefault_life_time}};
    elements.push_back(element);
    --count_of_elements;
  }
  return elements;
}

void Transactions::StartTest(int counter, const std::vector<Storage::Element>& elements) {
  auto samples = CreateElements(counter, "sample");
  std::cout << "\nStart AVL tree test: \n";
  AvlTest(counter, elements, samples);
  std::cout << "\nStart Hash table tree test: \n";
  HashTableTest(counter, elements, samples);
  double avl_average = time_results_.GetAvlAverage();
  double hash_average = time_results_.GetHashAverage();
  std::cout << "\nAverage time:\n";
  std::cout << std::setw(kStringLength) << std::left  << "AVL: " << avl_average << " ms.\n";
  std::cout << std::setw(kStringLength) << std::left  << "Hash Table: " << hash_average << " ms.\n";
}

void Transactions::AvlTest(int counter, const std::vector<Storage::Element>& elements,
                                const std::vector<Storage::Element>& samples) {
  Holder avl_holder(Holder::StorageType::kAVL);
  for (auto &element : elements) {
    avl_holder.Set(element);
  }
  time_results_.avl_add_element = AddTest(&avl_holder, counter, samples);
  time_results_.avl_get_element = GetTest(&avl_holder, counter, elements);
  time_results_.avl_get_all_elements = GetAllElementsTest(&avl_holder, counter);
  time_results_.avl_find_key = FindTest(&avl_holder, counter, elements);
  time_results_.avl_remove_element = RemoveTest(&avl_holder, counter, elements);
}

void Transactions::HashTableTest(int counter, const std::vector<Storage::Element>& elements,
                                const std::vector<Storage::Element>& samples) {
  Holder hash_holder(Holder::StorageType::kHashTable);
  for (auto &element : elements) {
    hash_holder.Set(element);
  }
  time_results_.hash_add_element = AddTest(&hash_holder, counter, samples);
  time_results_.hash_get_element = GetTest(&hash_holder, counter, elements);
  time_results_.hash_get_all_elements = GetAllElementsTest(&hash_holder, counter);
  time_results_.hash_find_key = FindTest(&hash_holder, counter, elements);
  time_results_.hash_remove_element = RemoveTest(&hash_holder, counter, elements);
}

double Transactions::AddTest(Holder* storage, int counter, const std::vector<Storage::Element>& samples) {
  double result = 0;
  auto start_time = std::chrono::steady_clock::now();
  for (int i = 0; i < counter; ++i) {
    storage->Set(samples[i]);
  }
  auto end_time = std::chrono::steady_clock::now();
  result = std::chrono::duration<double, std::milli>(end_time - start_time).count();
  std::cout << std::setw(kStringLength) << std::left << "add test complited ";
  std::cout<< result << " ms." << std::endl;
  return result;
}

double Transactions::GetTest(Holder* storage, int counter, const std::vector<Storage::Element>& elements) {
  double result = 0;
  size_t size = elements.size() - 1;
  auto start_time = std::chrono::high_resolution_clock::now();
  while (counter > 0) {
    int index = GetRandomNumber(0, size);
    storage->Get(elements[index].GetKey());
    --counter;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  result = std::chrono::duration<double, std::milli>(end_time - start_time).count();
  std::cout << std::setw(kStringLength) << std::left << "get test complited ";
  std::cout << result << " ms." << std::endl;
  return result;
}

double Transactions::GetAllElementsTest(Holder* storage, int counter) {
  double result = 0;
  auto start_time = std::chrono::high_resolution_clock::now();
  while (counter > 0) {
    storage->AllElements();
    --counter;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  result = std::chrono::duration<double, std::milli>(end_time - start_time).count();
  std::cout << std::setw(kStringLength) << std::left << "get all test complited ";
  std::cout << result << " ms." << std::endl;
  return result;
}

double Transactions::FindTest(Holder* storage, int counter, const std::vector<Storage::Element>& elements) {
  double result = 0;
  size_t size = elements.size() - 1;
  auto start_time = std::chrono::high_resolution_clock::now();
  while (counter > 0) {
    int index = GetRandomNumber(0, size);
    storage->Find(elements[index].GetData());
    --counter;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  result = std::chrono::duration<double, std::milli>(end_time - start_time).count();
  std::cout << std::setw(kStringLength) << std::left << "find test complited ";
  std::cout << result << " ms." << std::endl;
  return result;
}

double Transactions::RemoveTest(Holder* storage, int counter,
                              const std::vector<Storage::Element>& elements) {
  double result = 0;
  std::deque<Storage::Element> deque;
  std::copy(elements.begin(), elements.end(), std::front_inserter(deque));

  auto start_time = std::chrono::high_resolution_clock::now();
  while (counter > 0 && deque.size() > 0) {
    storage->Del(deque.front().GetKey());
    deque.pop_front();
    --counter;
  }
  auto end_time = std::chrono::high_resolution_clock::now();
  result = std::chrono::duration<double, std::milli>(end_time - start_time).count();
  std::cout << std::setw(kStringLength) << std::left << "delete test complited ";
  std::cout << result << " ms." << std::endl;
  return result;
}

double Transactions::TimeResults::GetAvlAverage() {
  const double total_numbers = 5.0;
  double sum =
    avl_get_element + avl_add_element + avl_remove_element + avl_get_all_elements + avl_find_key;
  return sum / total_numbers;
}

double Transactions::TimeResults::GetHashAverage() {
  const double total_numbers = 5.0;
  double sum =
    hash_get_element + hash_add_element + hash_remove_element + hash_get_all_elements + hash_find_key;
  return sum / total_numbers;
}

}  // namespace s21
