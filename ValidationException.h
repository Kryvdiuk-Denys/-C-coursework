// ValidationException.h
#pragma once

#include <stdexcept>
#include <string>

/// \file ValidationException.h
/// \brief Клас виключення для помилок введення користувача.

/// \class ValidationException
/// \brief Виключення для некоректних вхідних даних.
/// \details Використовується у випадках, коли введені дані не відповідають
/// очікуваному формату або містять хибні значення.
class ValidationException : public std::runtime_error
{
public:
    /// \brief Створює об'єкт виключення з описом помилки.
    /// \param message Текст повідомлення про помилку.
    explicit ValidationException(const std::string& message);
};
