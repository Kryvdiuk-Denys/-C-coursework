// SkiTour.h
#pragma once

#include "Tour.h"
#include "ISerializable.h"

#include <iostream>
#include <string>

/// \file SkiTour.h
/// \brief Оголошення класу SkiTour — гірськолижний тур.

/// \class SkiTour
/// \brief Клас, що представляє гірськолижний тур.
/// \details Містить інформацію про країну, курорт, складність,
/// наявність спорядження та страхування, дати подорожі та ціну.
class SkiTour : public Tour, public ISerializable
{
private:
   std::string country;
   std::string resort;
   std::string difficulty;
   bool equipmentIncluded = false;
   bool insuranceIncluded = false;
   std::string departureDate;
   std::string returnDate;
   double price = 0.0;

public:
   /// \brief Створює порожній гірськолижний тур.
   SkiTour() = default;

   /// \brief Створює гірськолижний тур на основі CSV-рядка.
   /// \param csvLine Рядок з даними туру у форматі CSV.
   explicit SkiTour(const std::string& csvLine);

   /// \brief Створює копію гірськолижного туру.
   /// \param other Інший об’єкт SkiTour для копіювання.
   SkiTour(const SkiTour& other);

   /// \brief Переміщує дані з іншого гірськолижного туру.
   /// \param other Інший об’єкт SkiTour для переміщення.
   SkiTour(SkiTour&& other) noexcept;

   /// \brief Звільняє ресурси, пов’язані з гірськолижним туром.
   ~SkiTour() override;

   /// \brief Інтерактивно зчитує дані про тур з консолі.
   void input() override;

   /// \brief Виводить інформацію про тур у консоль.
   void display() const override;

   /// \brief Серіалізує тур у формат CSV.
   /// \return Рядок з даними туру у форматі CSV.
   std::string toCSV() const override;

   /// \brief Повертає країну туру.
   /// \return Назва країни.
   std::string getCountry() const override
   {
      return country;
   }

   /// \brief Повертає назву курорту (як місто для базового інтерфейсу).
   /// \return Назва курорту.
   std::string getCity() const override
   {
      return resort;
   }

   /// \brief Повертає дату відправлення.
   /// \return Дата відправлення у текстовому форматі.
   std::string getDepartureDate() const override
   {
      return departureDate;
   }

   /// \brief Повертає дату повернення.
   /// \return Дата повернення у текстовому форматі.
   std::string getReturnDate() const override
   {
      return returnDate;
   }

   /// \brief Повертає ціну туру.
   /// \return Вартість туру.
   double getPrice() const override
   {
      return price;
   }

   /// \brief Повертає рівень складності туру як рівень готелю.
   /// \return Позначення складності (Easy / Medium / Hard).
   std::string getHotelLevel() const override
   {
      return difficulty;
   }

   /// \brief Дозволяє змінити параметри туру у інтерактивному режимі.
   void editInteractive() override;
};
