#ifndef SRC_TRANSACTIONS_H_
#define SRC_TRANSACTIONS_H_

#include "storage.h"
#include <vector>
#include <string>
#include "holder.h"

namespace s21 {

class Transactions {
 public:
  Transactions() = default;
  ~Transactions() = default;
  void Start();

  enum Message {
    kWelcome,
    kStorageHash,
    kStorageAVL,
    kStorageBTree,
    kMakeCompare,
    kShowHelp,
    kActiv,
    kSwitch,
    kSuccess,
    kNotExist
  };

  enum Command {
    kSet,
    kGet,
    kExists,
    kDel,
    kUpdate,
    kRename,
    kTtl,
    kFind,
    kUpload,
    kExport,
    kCompare,
    kKeys,
    kShowall
  };

 private :
  struct MaxLength {
    size_t number = 6;
    size_t surname = 15;
    size_t name = 15;
    size_t year = 6;
    size_t city = 15;
    size_t coins = 4;
  } lengths_;

  struct TimeResults {
    double hash_get_element = 0;
    double hash_add_element = 0;
    double hash_remove_element = 0;
    double hash_get_all_elements = 0;
    double hash_find_key = 0;

    double avl_get_element = 0;
    double avl_add_element = 0;
    double avl_remove_element = 0;
    double avl_get_all_elements = 0;
    double avl_find_key = 0;
    double GetHashAverage();
    double GetAvlAverage();
  } time_results_;

  static const int kDefault_life_time = -1;
  static const int kStringLength = 30;
  size_t size_ = 0;

  Holder *storage_ = nullptr;
  Holder::StorageType type_ = Holder::StorageType::kEmpty;

  bool Initialize(const Holder::StorageType &type);
  void ReadCommandFromConsole(const std::string& command);
  void TableCommandExecutor(const std::string& command);
  std::vector<std::string> Parser(const std::string& command);
  std::string RemoveSpaces(const std::string& command);
  bool IsDigital(const std::string& line);

  void AddElementToStorage(const std::string& command);
  void UpdateElement(const std::string& command);
  void FindElement(const std::string& command);
  void GetElement(const std::string& command);
  void CheckExistsElement(const std::string& command);
  void DeleteElement(const std::string& command);
  void ShowAllKeys();
  void RenameKey(const std::string& command);
  void ShowTtl(const std::string& command);
  void ShowAllElements();

  void MakeStorageCompare(const std::string& command);
  std::vector<Storage::Element> CreateElements(int count_of_elements, const std::string& prefix);
  void StartTest(int counter, const std::vector<Storage::Element>& elements);
  void AvlTest(int counter, const std::vector<Storage::Element>& elements,
              const std::vector<Storage::Element>& samples);
  void HashTableTest(int counter, const std::vector<Storage::Element>& elements,
                    const std::vector<Storage::Element>& samples);

  double AddTest(Holder* storage, int counter, const std::vector<Storage::Element>& elements);
  double GetTest(Holder* storage, int counter, const std::vector<Storage::Element>& elements);
  double FindTest(Holder* storage, int counter, const std::vector<Storage::Element>& elements);
  double RemoveTest(Holder* storage, int counter, const std::vector<Storage::Element>& elements);
  double GetAllElementsTest(Holder* storage, int counter);


  int GetRandomNumber(int min, int max);

  void ExportToFile(const std::string& command);
  void ImportFromFile(const std::string& command);

  inline void Print(const Message &message);
  void PrintMenu();
  void PrintElement(const Storage::Element &element, bool is_table_print = false);
  void PrintTableHead();
  void ResetLengthPrintSettings(const std::vector<std::string> &tokens);
  void SetDefaultPrintSettings();
  void Clear();

  const std::vector<std::string> messages {
    "(EXIT)                     Enter to exit the program\n"\
    "(HELP)                     Get help",
    "(HT)                       Hash table",
    "(AVL)                      Self balancing binary search tree",
    "(BT)                       B tree",
    "(COMPARE N1 N2)            make storage compare. N1 - number of elements, N2 - number of repeats",
    "       Storage commands:\n"\
    "(SET S1 S2 S3 N1 S4 N2 EX N3) add element. S1 - key, S2 - surname, S3 - name, N1 - year, "\
      "S4 - city, N2 - coins, N3 - life time\n"\
    "(UPDATE S1 S2 S3 N1 S4 N2) update elements data. S1 - key, S2 - surname, "\
      "S3 - name, N1 - year, S4 - city, N2 - coins \n"\
    "(FIND S1 S2 N1 S3 N2)      find element. S1 - surname, S2 - name, N1 - year, "\
      "S3 - city, N2 - coins, or '-'. \n"\
    "(RENAME S1 S2)             rename key. S1 - old keys name, S2 - new keys name.\n"\
    "(GET S1)                   show element. S1 - key\n"\
    "(EXISTS S1)                check element. S1 - key\n"\
    "(DEL S1)                   remove element. S1 - key\n"\
    "(KEYS)                     show all keys.\n"\
    "(SHOWALL)                  show all elements table.\n"\
    "(UPLOAD S1)                load data from file. S1 - file path.\n"\
    "(EXPORT S1)                Save data to file. S1 - file path.\n"\
    "(TTL S1)                   show element current life time. S1 - key.",
    " [ACTIV] ",
    "       Enter type name to switch storage type",
    "Successfully switched",
    "Key is not exist"
  };

  const std::vector<std::string> regex_ {
    "^((SET|set){1}[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[0-9]{1,14}[ ]+[^ ]+[ ]+[0-9]{1,14}[ ]{0,}"\
    "([ ]+(EX|ex){1}[ ]+[0-9]{1,14}[ ]{0,})?$)",
    "^(GET|get)[ ]+[^ ]{1,}[ ]{0,}$",
    "^(EXISTS|exists)[ ]+[^ ]{1,}[ ]{0,}$",
    "^(DEL|del)[ ]+[^ ]{1,}[ ]{0,}$",
    "^((UPDATE|update){1}[ ]+[^ ]+[ ]+[^ ]+[ ]+[^ ]+[ ]+[0-9-]{1,14}[ ]+[^ ]+[ ]+[0-9-]{1,14}[ ]{0,})$",
    "^(RENAME|rename)[ ]+[^ ]+[ ]+[^ ]+[ ]{0,}$",
    "^(TTL|ttl)[ ]+[^ ]{1,}[ ]{0,}$",
    "^((FIND|find){1}[ ]+[^ ]+[ ]+[^ ]+[ ]+[0-9-]{1,14}[ ]+[^ ]+[ ]+[0-9-]{1,14}[ ]{0,})$",
    "^(UPLOAD|upload)[ ]+[^ ]{1,}[ ]{0,}$",
    "^(EXPORT|export)[ ]+[^ ]{1,}[ ]{0,}$",
    "^(COMPARE|compare)[ ]+[0-9]{1,10}+[ ]+[0-9]{1,10}+[ ]{0,}$"
  };
};

}  // namespace s21

#endif  // SRC_TRANSACTIONS_H_
