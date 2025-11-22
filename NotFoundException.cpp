// NotFoundException.cpp

#include "NotFoundException.h"

NotFoundException::NotFoundException(const std::string& msg)
   : std::runtime_error("Не знайдено: " + msg)
{
}
