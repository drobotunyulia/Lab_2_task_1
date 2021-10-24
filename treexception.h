#ifndef TREEXCEPTION_H
#define TREEXCEPTION_H

#include <iostream>

class tree_exception : public std::exception
{
protected:
    std::string exception_message;
public:
    tree_exception(std::string exception_message);
    const char *what() const noexcept;
};

tree_exception::tree_exception(std::string exception_message)
{
    this->exception_message = exception_message;
}

const char* tree_exception::what() const noexcept
{
    return exception_message.c_str();
}


#endif // TREEXCEPTION_H
