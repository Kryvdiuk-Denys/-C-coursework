// FileException.h
#pragma once

#include <stdexcept>
#include <string>

/// \brief Виключення, пов'язане з помилками роботи з файлами.
class FileException : public std::runtime_error
{
public:
    /// \brief Створює виняток роботи з файлами з повідомленням.
    /// \param msg Опис помилки.
    explicit FileException(const std::string& msg)
       : std::runtime_error("Помилка файлу: " + msg)
    {
    }
};
