// ISerializable.h
#pragma once

#include <string>

/// \brief Інтерфейс для класів, що підтримують серіалізацію у формат CSV.
/// \details Кожен клас, який реалізує цей інтерфейс, повинен визначити власний спосіб
/// перетворення об’єкта у рядок CSV для збереження у файл.
class ISerializable
{
public:
 /// \brief Віртуальний деструктор інтерфейсу серіалізації.
 virtual ~ISerializable() = default;

 /// \brief Отримує рядкове представлення даних об’єкта у форматі CSV.
 /// \return Рядок у форматі "value1,value2,value3,...".
 virtual std::string toCSV() const = 0;
};
