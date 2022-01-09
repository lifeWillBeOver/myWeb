#include "util.hpp"
#include <string.h>

namespace util {

void util::ErrorMsg::PrintErrorMsg(const char *filename, const char *funcName,
                                   int line, const char *msg, int errCode) {
    snprintf(this->infomation, this->info_size, this->temp.c_str(), filename,
             funcName, line, msg, strerror(errno), errCode);
    printf("%s\n", this->infomation);
}

} // namespace util
