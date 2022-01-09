#include <iostream>

namespace util {
class ErrorMsg {
  public:
    ErrorMsg() { infomation = (char *)malloc(sizeof(char) * info_size); }
    ErrorMsg(unsigned int sz) : info_size(sz) { ErrorMsg(); }
    virtual ~ErrorMsg() {
        if (infomation)
            free(infomation);
    }

    virtual void PrintErrorMsg(const char *, const char *, int, const char *msg,
                               int errCode);

  private:
    char *infomation = nullptr;
    unsigned int info_size = 200;
    std::string temp = "<%s@%s:line %d>   errorMsg = %s, %s, (errno: %d)\n";
};
} // namespace util