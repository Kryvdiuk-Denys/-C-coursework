// Tour.h
#pragma once

#include <string>

/// \file Tour.h
/// \brief Абстрактний базовий клас для всіх видів турів.

/// \class Tour
/// \brief Абстрактний інтерфейс туристичного туру.
/// \details Визначає обов'язкові методи, які повинні бути реалізовані
/// у всіх похідних класах: введення, виведення, серіалізація,
/// доступ до характеристик та редагування.
class Tour
{
public:
   /// \brief Віртуальний деструктор базового класу туру.
   virtual ~Tour() = default;

   // --- Методи введення та виведення. ---

   /// \brief Інтерактивне введення даних туру.
   virtual void input() = 0;

   /// \brief Відображає інформацію про тур.
   virtual void display() const = 0;

   /// \brief Серіалізує об'єкт у CSV формат.
   /// \return Рядок з даними туру у форматі CSV.
   virtual std::string toCSV() const = 0;

   // --- Гетери характеристик туру. ---

   /// \brief Повертає країну туру.
   /// \return Назва країни.
   virtual std::string getCountry() const = 0;

   /// \brief Повертає місто або курорт туру.
   /// \return Назва міста або курорту.
   virtual std::string getCity() const = 0;

   /// \brief Повертає дату відправлення.
   /// \return Дата відправлення у текстовому форматі.
   virtual std::string getDepartureDate() const = 0;

   /// \brief Повертає дату повернення.
   /// \return Дата повернення у текстовому форматі.
   virtual std::string getReturnDate() const = 0;

   /// \brief Повертає вартість туру.
   /// \return Ціна туру.
   virtual double getPrice() const = 0;

   /// \brief Повертає рівень готелю або аналогічний показник.
   /// \return Рівень готелю або відповідний опис.
   virtual std::string getHotelLevel() const = 0;

   // --- Редагування. ---

   /// \brief Інтерактивне редагування параметрів туру.
   virtual void editInteractive() = 0;
};
