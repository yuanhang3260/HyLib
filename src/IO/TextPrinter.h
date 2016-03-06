#ifndef TEXT_PRINTER_
#define TEXT_PRINTER_

#include <vector>
#include <map>
#include <memory>

#include "FileDescriptor.h"

namespace IO {

class TextPrinterImpl;

class TextPrinter {
 public:
  TextPrinter();
  TextPrinter(std::string outputfile);
  ~TextPrinter();
  
  bool Open(std::string outputfile);

  void Print(const std::string& content);
  void Print(const std::string& content,
             const std::vector<std::string>& matches);
  void Print(const std::string& content,
             const std::map<std::string, std::string>& map);
  void Flush();

  void Close();

 protected:
  std::unique_ptr<TextPrinterImpl> text_printer_impl_;
};

}

#endif /* TEXT_PRINTER_ */