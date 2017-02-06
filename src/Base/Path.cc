#include "Base/Log.h"
#include "Strings/Utils.h"
#include "Base/Path.h"

namespace Path {

std::string JoinPathImpl(
                std::initializer_list<StringPiece> args_list) {
  if (args_list.size() == 0) {
    return "";
  }

  std::string result = "";
  auto it = args_list.begin();
  if ((*it)[0] == '/') {
    result = "/";
  }
  result += Strings::Strip(it->as_string(), "/");
  it++;

  for (; it != args_list.end(); it++) {
    if (it->empty()) {
      continue;
    }
    auto str = Strings::Strip(it->as_string(), "/");
    result += ("/" + str);
  }
  return result;
}

}
