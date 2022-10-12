#include <fstream>
#include <sstream>
#include <iostream>
#include "storage.h"

namespace s21 {

Storage::Element::Element()
  : key_(""), data_() {}

Storage::Element::Element(string key, const Data& data)
  : key_(key), data_(data) {}

Storage::Element::Element(const Element& other)
  : key_(other.key_), data_(other.data_) {}

Storage::Element::Element(Element&& other) {
  std::swap(key_, other.key_);
  std::swap(data_, other.data_);
}

Storage::Element::~Element() {}

bool Storage::Element::operator==(const Element& other) {
  return key_ == other.key_;
}

bool Storage::Element::operator>(const Element& other) {
  return key_ > other.key_;
}

bool Storage::Element::operator<(const Element& other) {
  return key_ < other.key_;
}

bool Storage::Element::operator!=(const Element& other) {
  return key_ != other.key_;
}

void Storage::Element::operator=(const Data &data) {
  data_ = data;
}

bool Storage::Element::operator>=(const Element& other) {
  return key_ >= other.key_;
}

bool Storage::Element::operator<=(const Element& other) {
  return key_ <= other.key_;
}

Storage::Element& Storage::Element::operator=(const Element& other) {
  key_ = other.key_;
  data_ = other.data_;
  return *this;
}

Storage::Element& Storage::Element::operator=(const Element&& other) {
  key_ = other.key_;
  data_ = other.data_;
  return *this;
}

Storage::Element::Data& Storage::Element::operator*() {
  return data_;
}

const std::string Storage::Element::GetKey() const {
  return key_;
}

Storage::Element::Data Storage::Element::GetData() const {
  return data_;
}

std::string Storage::Element::GetSurname() {
  return data_.surname;
}

std::string Storage::Element::GetName() {
  return data_.name;
}

std::string Storage::Element::GetYearOfBirth() {
  return data_.year_of_birth;
}
std::string Storage::Element::GetCity() {
  return data_.city;
}

std::string Storage::Element::GetCoins() {
  return data_.coins;
}

int Storage::Element::GetLifeTime() {
  return data_.life_time;
}

void Storage::Element::SetKey(string key) {
  key_ = key;
}
void Storage::Element::SetData(const Data& data) {
  data_ = data;
}

void Storage::Element::SetSurname(string surname) {
  data_.surname = surname;
}

void Storage::Element::SetName(string name) {
  data_.name = name;
}

void Storage::Element::SetYearOfBirth(string year_of_birth) {
  data_.year_of_birth = year_of_birth;
}

void Storage::Element::SetCity(string city) {
  data_.city = city;
}

void Storage::Element::SetCoins(string coins) {
  data_.coins = coins;
}

void Storage::Element::PrintElement() const {
  std::cout <<"key = " << key_ << "; " << data_.surname << "; " << data_.name << "; " << data_.year_of_birth
            << "; " << data_.city << "; " << data_.coins << std::endl;
}

Storage::vector Storage::Keys() {
  vector vector_of_keys;
  std::vector<Storage::Element> vector_of_elements = AllElements();
  for (auto element : vector_of_elements) {
    vector_of_keys.push_back(element.GetKey());
  }
  return vector_of_keys;
}

std::vector<Storage::Element::Data> Storage::ShowAll() {
  std::vector<Storage::Element::Data> vector_of_datas;
  std::vector<Storage::Element> vector_of_elements = AllElements();
  for (auto element : vector_of_elements) vector_of_datas.push_back(element.GetData());
  return vector_of_datas;
}

int Storage::Upload(string file_name) {
  if (!CheckFileType(file_name)) throw std::invalid_argument("File format error");
  std::ifstream file_stream;
  file_stream.open(file_name);
  if (!file_stream.is_open()) throw std::invalid_argument("file is not exist");
  Init();
  return FillElementsFromFile(&file_stream);
}

bool Storage::CheckFileType(string file_name) {
  const std::string format = ".data";
  const int format_length = 5;
  const int length = file_name.length();
  if (length <= format_length || file_name.substr(length - format_length, length) != format) {
    return false;
  }
  return true;
}

int Storage::FillElementsFromFile(std::ifstream* input) {
  std::string line;
  int counter = 0;
  while (std::getline(*input, line)) {
    std::istringstream iss(line);
    Element::Data data;
    std::string key;
    int year_of_birth, coins;
    if (!(iss >> key >> data.surname >> data.name >> year_of_birth >> data.city >> coins)) {
      throw std::invalid_argument("invalid data");
    }
    if (year_of_birth  < 0 || coins < 0) throw std::invalid_argument("years or coins are negative");
    data.year_of_birth = std::to_string(year_of_birth);
    data.coins = std::to_string(coins);
    const int kDefault_life_time = -1;
    data.life_time = kDefault_life_time;
    Element element = Element(key, data);
    Set(element);
    ++counter;
  }
  return counter;
}

int Storage::Export(std::string file_name) {
  int counter = 0;
  if (!CheckFileType(file_name)) {
    file_name += ".data";
  }
  if (file_name.length() > 0) {
    std::ofstream out;
    out.open(file_name, std::ios::trunc);
    if (!out.is_open()) throw std::invalid_argument("Export file error: file not exist or corrupted");
    std::vector<Element> vector_of_elements = AllElements();
    for (auto element : vector_of_elements) {
      out << element.GetKey() << " " << element.GetSurname() << " "
          << element.GetName() << " " << element.GetYearOfBirth() << " "
          << element.GetCity() << " " << element.GetCoins() << "\n";
      ++counter;
    }
  }
  return counter;
}

bool Storage::IsDataSiutable(const Element::Data &need_data, const Element::Data &exist_data) {
  if ((need_data.surname != "-" && need_data.surname != exist_data.surname)
    || (need_data.name != "-" && need_data.name != exist_data.name)
    || (need_data.year_of_birth != "-" && need_data.year_of_birth != exist_data.year_of_birth)
    || (need_data.city != "-" && need_data.city != exist_data.city)
    || (need_data.coins!= "-" && need_data.coins != exist_data.coins)) {
    return false;
  }
  return true;
}

void Storage::Element::Data::PrintData() const {
  std::cout << surname << "; " << name << "; " << year_of_birth <<
                "; " << city << "; " << coins << std::endl;
}
}  // namespace s21
