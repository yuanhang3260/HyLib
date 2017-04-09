#ifndef IO_FILE_
#define IO_FILE_

#include "Strings/StringPiece.h"

namespace File {

int FileSize(StringPiece filename);

bool GetContent(StringPiece file_path, std::string* content);
bool SetContent(StringPiece file_path, StringPiece content);

}  // namespace File

#endif  /* IO_FILE_ */