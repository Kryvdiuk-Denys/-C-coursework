// CityTour.h
#pragma once

#include "Tour.h"
#include "ISerializable.h"

#include <iostream>
#include <string>

/// \class CityTour
/// \brief Представляє міський тур.
class CityTour : public Tour, public ISerializable
{
private:
   std::string country;
   std::string city;
   std::string accommodation;
   std::string transport;
   std::string departureDate;
   std::string returnDate;
   std::string hotelLevel;
   std::string food;
   std::string extras;
   double price = 0.0;

public:
   /// \brief Створює порожній міський тур із значеннями за замовчуванням.
   CityTour() = default;

   /// \brief Створює міський тур на основі CSV-рядка.
   /// \param csvLine Рядок з даними туру у форматі CSV.
   explicit CityTour(const std::string& csvLine);

   /// \brief Створює копію міського туру.
   /// \param other Інший об’єкт CityTour для копіювання.
   CityTour(const CityTour& other);

   /// \brief Переміщує дані іншого міського туру.
   /// \param other Інший об’єкт CityTour для переміщення.
   CityTour(CityTour&& other) noexcept;

   /// \brief Звільняє ресурси, пов’язані з міським туром.
   ~CityTour() override;

   /// \brief Зчитує дані міського туру з консолі.
   void input() override;

   /// \brief Виводить коротку інформацію про міський тур.
   void display() const override;

   /// \brief Серіалізує міський тур у CSV-рядок.
   /// \return Рядок з даними туру у форматі CSV.
   std::string toCSV() const override;

   /// \brief Повертає країну туру.
   /// \return Назва країни.
   std::string getCountry() const override
   {
      return country;
   }

   /// \brief Повертає місто туру.
   /// \return Назва міста.
   std::string getCity() const override
   {
      return city;
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

   /// \brief Повертає вартість туру.
   /// \return Вартість у грошових одиницях.
   double getPrice() const override
   {
      return price;
   }

   /// \brief Повертає рівень готелю.
   /// \return Позначення рівня готелю.
   std::string getHotelLevel() const override
   {
      return hotelLevel;
   }

   /// \brief Дозволяє змінити параметри туру у інтерактивному режимі.
   void editInteractive() override;

   /// \brief Повертає назву туру для відображення.
   /// \return Назва туру (місто).
   std::string getName() const
   {
      return city;
   }
};
