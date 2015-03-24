#ifndef LINP_EXCEPTION_H
#define LINP_EXCEPTION_H

#include <exception>
#include <string>

class LinpException : public std::exception
{
protected:
    std::string text;
    bool is_critical;
public:
    const char *what() const throw();
    LinpException(const std::string &text_, bool is_critical_=true):
        text(text_), is_critical(is_critical_) {};
    bool isCritical() const throw() {return is_critical;}
    ~LinpException() throw() {}
};

class DatabaseException : public LinpException
{
public:
    DatabaseException(const std::string &text_, bool is_critical_=true):
        LinpException(text_, is_critical_) {};
};

class ApplicationException : public LinpException
{
public:
    ApplicationException(const std::string &text_, bool is_critical_=true):
        LinpException(text_, is_critical_) {};
};

#endif // LINP_EXCEPTION_H
