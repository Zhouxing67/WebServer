#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <iostream>

using std::string;
using std::ostream;
using std::istream;

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
    void getline(istream &is = std::cin);
    void set_buf(const char *);
};
#endif