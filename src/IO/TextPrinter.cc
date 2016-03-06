#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "TextPrinter.h"
#include "../Utility/BufferedDataWriter.h"
#include "../Utility/StringBuilder.h"


namespace IO {

// TextPrinter Implementor
class TextPrinterImpl {
 public:
  friend class TextPrinter;
  TextPrinterImpl() {};
  TextPrinterImpl(std::string outputfile);
  bool Open_Impl(std::string outputfile);
  int Print_Impl(const std::string&);
  int Print_Impl(const std::string&, const std::vector<std::string>&);
  int Print_Impl(const std::string&, const std::map<std::string, std::string>&);
  int DoPrint(const char* piece, const int size);
  int DoPrint(std::string piece);
  void Flush_Impl();
  void Close_Impl();

 private:
  std::string outputfile_;
  std::unique_ptr<Utility::BufferedDataWriter> writer_;
  int ValidateContentFormat(std::string content);
};

bool TextPrinterImpl::Open_Impl(std::string outputfile) {
  outputfile_ = outputfile;
  FileDescriptor* fd =
    new FileDescriptor(outputfile, FileDescriptor::WRITE_ONLY);
  if (fd->closed()) {
    return false;
  }
  writer_.reset(new Utility::BufferedDataWriter(fd));
  return true;
}

void TextPrinterImpl::Close_Impl() {
  writer_.reset(NULL);
}

TextPrinterImpl::TextPrinterImpl(std::string outputfile) :
    outputfile_(outputfile) {
  FileDescriptor* fd =
    new FileDescriptor(outputfile, FileDescriptor::WRITE_ONLY);
  writer_.reset(new Utility::BufferedDataWriter(fd));
}

int TextPrinterImpl::Print_Impl(const std::string& content) {
  DoPrint(content.c_str(), content.length());
  return content.length();
}

int TextPrinterImpl::Print_Impl(
    const std::string& content, const std::vector<std::string>& matches) {
  std::vector<std::string> pieces;
  unsigned last_index = 0, matched_num = 0;
  int printed_size = 0;
  for (unsigned int i = 0; i < content.length(); i++) {
    if (content[i] == '$') {
      printed_size += DoPrint(content.substr(last_index, i - last_index));
      if (i < content.length() - 1 && content[i + 1] == '$') {
        printed_size += DoPrint("$");
        last_index = (++i) + 1;
        continue;
      }
      else {
        if (matched_num < matches.size()) {
          printed_size += DoPrint(matches.at(matched_num++));
        }
        last_index = i + 1;
      }
    }
  }
  if (last_index < content.length()) {
    printed_size += DoPrint(content.substr(last_index));
  }

  return printed_size;
}

int TextPrinterImpl::Print_Impl(
    const std::string& content,
    const std::map<std::string, std::string>& matches) {
  // if (!ValidateContentFormat(content)) {
  //   fprintf(stderr, "ERROR in %s(): Incorrect bracket format of \"%s\"\n",
  //           __FUNCTION__, content.c_str());
  //   return -1;
  // }

  int printed_size = 0;
  int matching = 0;
  Utility::StringBuilder strbuf;
  for (unsigned int i = 0; i < content.length(); i++) {
    // Matching or not
    if (!matching) {
      if (i < content.length() - 1 &&
          content[i] == '$' &&
          content[i + 1] == '{') {
        printed_size += DoPrint(strbuf.CharArray(), strbuf.size());
        strbuf.Clear();
        matching = 1;
        i++;
      }
      else {
        strbuf.Append(content[i]);
      }
    }
    else {
      if (content[i] == '}') {
        std::string key = strbuf.ToString();
        strbuf.Clear();
        if (matches.find(key) != matches.end()) {
          printed_size += DoPrint(matches.at(key));
        }
        matching = 0;
      }
      else {
        strbuf.Append(content[i]);
      }
    }
  }
  if (!strbuf.IsEmpty()) {
    printed_size += DoPrint(strbuf.CharArray(), strbuf.size());
  }

  return printed_size;
}

int TextPrinterImpl::ValidateContentFormat(std::string content) {
  int matching = 0;
  for (unsigned int i = 0; i < content.length(); i++) {
    if (content[i] == '{') {
      if (i < content.length() - 1 && content[i+1] == '{') {
        i++;
        continue;
      }
      if (matching) {
        fprintf(stderr,
                "ERROR in %s(): \"{\" mismatch at index %d\n",
                __FUNCTION__, i);
        return 0;
      }
      matching = 1;
    }
    else if (content[i] == '}') {
      if (i < content.length() - 1 && content[i+1] == '}') {
        i++;
        continue;
      }
      if (!matching) {
        fprintf(stderr,
                "ERROR in %s(): \"}\" mismatch at index %d\n",
                __FUNCTION__, i);
        return 0;
      }
      matching = 0;
    }
  }
  if(matching) {
    fprintf(stderr, "ERROR in %s(): bracket mismatch at end\n", __FUNCTION__);
    return 0;
  }
  return 1;
}

int TextPrinterImpl::DoPrint(const char* piece, const int size) {
  if (!piece) {
    return 0;
  }
  // std::cout << "printing " << piece << std::endl;
  int nwrite = writer_->Write(piece, 0, size);
  if (nwrite < 0) {
    fprintf(stderr, "ERROR: DoPrint %d chars failed\n", size);
    return 0;
  }
  return nwrite;
}

int TextPrinterImpl::DoPrint(std::string piece) {
  return DoPrint(piece.c_str(), piece.length());
}

void TextPrinterImpl::Flush_Impl() {
  writer_->Flush();
}

// TextPrinter functions
TextPrinter::TextPrinter() {
  text_printer_impl_.reset(new TextPrinterImpl());
}

TextPrinter::TextPrinter(std::string outputfile) {
  text_printer_impl_.reset(new TextPrinterImpl(outputfile));
}

TextPrinter::~TextPrinter() {
}

bool TextPrinter::Open(std::string outputfile) {
  return text_printer_impl_->Open_Impl(outputfile);
}

void TextPrinter::Print(const std::string& content) {
  text_printer_impl_->Print_Impl(content);
}

void
TextPrinter::Print(const std::string& content,
                   const std::vector<std::string>& matches) {
  text_printer_impl_->Print_Impl(content, matches);
}

void TextPrinter::Print(
    const std::string& content,
    const std::map<std::string, std::string>& matches) {
  text_printer_impl_->Print_Impl(content, matches);
}

void TextPrinter::Flush() {
  text_printer_impl_->Flush_Impl();
}

void TextPrinter::Close() {
  text_printer_impl_->Close_Impl();
}


}  // namespace IO
