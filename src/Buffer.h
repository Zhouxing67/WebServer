#ifndef BUFFER_H
#define BUFFER_H

#include <string>

using std::string;

class Buffer
{
private:
    string buf_;
public:
    Buffer() {}
    ~Buffer() {}
    const char *str_data() { return buf_.c_str(); };
    int size() { return buf_.size(); }
    
    void append(const char *str, int size);
    void clear() { buf_.clear(); }
};
#endif