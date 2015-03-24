#include "linp_exception.h"

const char *LinpException::what() const throw()
{
    return this->text.c_str();
}
