#ifndef SRC_STORAGE_H_
#define SRC_STORAGE_H_

#include <string>
#include <vector>

namespace s21 {

class Storage {
 public:
  struct Data;
  class Element;
  using vector = std::vector<std::string>;
  using string = const std::string &;
  using element = const Element &;

 public:
  class Element {
   public:
    struct Data {
      std::string surname;
      std::string name;
      std::string year_of_birth;
      std::string city;
      std::string coins;
      int life_time = 0;
      friend bool operator==(const Data& data_left, const Data& data_right) {
        if (data_left.surname != data_right.surname ||
            data_left.name != data_right.name ||
            data_left.year_of_birth != data_right.year_of_birth ||
            data_left.city != data_right.city ||
            data_left.coins != data_right.coins) return false;
        return true;
      }
      void PrintData() const;
    };

    Element();
    Element(string key, const Data& data);
    Element(const Element& other);
    Element(Element&& other);
    ~Element();

    Element& operator=(const Element& other);
    Element& operator=(const Element&& other);
    bool operator==(const Element& other);
    bool operator>(const Element& other);
    bool operator<(const Element& other);
    bool operator!=(const Element& other);
    bool operator>=(const Element& other);
    bool operator<=(const Element& other);
    void operator=(const Data &data);
    Data& operator*();

    const std::string GetKey() const;
    Data GetData() const;
    std::string GetSurname();
    std::string GetName();
    std::string GetYearOfBirth();
    std::string GetCity();
    std::string GetCoins();
    int GetLifeTime();

    void SetKey(string key);
    void SetData(const Data& data);
    void SetSurname(string surname);
    void SetName(string name);
    void SetYearOfBirth(string year_of_birth);
    void SetCity(string city);
    void SetCoins(string coins);
    void PrintElement() const;

   private:
    std::string key_;
    Data data_;
  };

  Storage() = default;
  virtual ~Storage() = default;

  virtual void Set(element element) = 0;
  virtual Element Get(string key) const = 0;
  virtual bool Exists(string key) const = 0;
  virtual bool Del(string key) = 0;
  virtual bool Update(string key, const Element::Data& data) = 0;
  vector Keys();
  virtual bool Rename(string key, string new_key) = 0;
  virtual int Ttl(string key) const = 0;
  virtual vector Find(const Element::Data& data) const = 0;
  std::vector<Element::Data> ShowAll();
  int Upload(string file_name);
  int Export(std::string file_name);
  virtual void Init() = 0;
  virtual std::vector<Element> AllElements() const  = 0;

 protected:
  static bool IsDataSiutable(const Element::Data &need_data, const Element::Data &exist_data);

 private:
  bool CheckFileType(const std::string &file_name);
  int FillElementsFromFile(std::ifstream* input);
};

}  // namespace s21

#endif  // SRC_STORAGE_H_
