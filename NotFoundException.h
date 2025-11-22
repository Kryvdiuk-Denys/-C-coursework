// NotFoundException.h
#pragma once

#include <stdexcept>
#include <string>

/// \file NotFoundException.h
/// \brief Клас виключення для випадків, коли об'єкт не знайдено.

/// \class NotFoundException
/// \brief Виключення, що сигналізує про відсутність потрібного елемента.
class NotFoundException : public std::runtime_error
{
public:
 /// \brief Створює виключення з повідомленням про помилку.
 /// \param msg Опис помилки.
 explicit NotFoundException(const std::string& msg);
};
