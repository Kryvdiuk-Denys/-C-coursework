// CityTour.cpp

#include "CityTour.h"
#include "FileException.h"

#include <iostream>
#include <sstream>
#include <cctype>
#include <string>

namespace
{
bool isValidDate(const std::string& date)
{
   if (date.size() != 10)
   {
      return false;
   }

   for (int i = 0; i < 10; ++i)
   {
      if (i == 4 || i == 7)
      {
         if (date[i] != '-')
         {
            return false;
         }
      }
      else
      {
         if (!std::isdigit(static_cast<unsigned char>(date[i])))
         {
            return false;
         }
      }
   }

   int year = 0;
   int month = 0;
   int day = 0;

   try
   {
      year  = std::stoi(date.substr(0, 4));
      month = std::stoi(date.substr(5, 2));
      day   = std::stoi(date.substr(8, 2));
   }
   catch (...)
   {
      return false;
   }

   if (month < 1 || month > 12)
   {
      return false;
   }

   int daysInMonth[12] =
   {
      31, 28, 31, 30, 31, 30,
      31, 31, 30, 31, 30, 31
   };

   bool isLeap =
      (year % 4 == 0 && year % 100 != 0) ||
      (year % 400 == 0);

   if (isLeap && month == 2)
   {
      daysInMonth[1] = 29;
   }

   if (day < 1 || day > daysInMonth[month - 1])
   {
      return false;
   }

   return true;
}

bool isValidHotelLevel(const std::string& level)
{
   if (level.size() < 2)
   {
      return false;
   }

   if (level.back() != '*')
   {
      return false;
   }

   std::string digits = level.substr(0, level.size() - 1);
   for (unsigned char ch : digits)
   {
      if (!std::isdigit(ch))
      {
         return false;
      }
   }

   int stars = 0;
   try
   {
      stars = std::stoi(digits);
   }
   catch (...)
   {
      return false;
   }

   if (stars < 1 || stars > 5)
   {
      return false;
   }

   return true;
}

bool tryParsePrice(const std::string& text, double& value)
{
   if (text.empty())
   {
      return false;
   }

   try
   {
      std::size_t pos = 0;
      double tmp = std::stod(text, &pos);

      if (pos != text.size())
      {
         return false;
      }

      if (tmp < 0.0)
      {
         return false;
      }

      value = tmp;
      return true;
   }
   catch (...)
   {
      return false;
   }
}
}

CityTour::CityTour(const CityTour& other)
   : country(other.country),
     city(other.city),
     accommodation(other.accommodation),
     transport(other.transport),
     departureDate(other.departureDate),
     returnDate(other.returnDate),
     hotelLevel(other.hotelLevel),
     food(other.food),
     extras(other.extras),
     price(other.price)
{
   std::cout << "[Copy] CityTour скопійовано\n";
}

CityTour::CityTour(CityTour&& other) noexcept
   : country(std::move(other.country)),
     city(std::move(other.city)),
     accommodation(std::move(other.accommodation)),
     transport(std::move(other.transport)),
     departureDate(std::move(other.departureDate)),
     returnDate(std::move(other.returnDate)),
     hotelLevel(std::move(other.hotelLevel)),
     food(std::move(other.food)),
     extras(std::move(other.extras)),
     price(other.price)
{
   std::cout << "[Move] CityTour переміщено\n";
}

CityTour::~CityTour()
{
   std::cout << "[Destructor] CityTour видалено\n";
}

CityTour::CityTour(const std::string& csvLine)
{
   std::istringstream iss(csvLine);
   std::string token;

   if (!std::getline(iss, country, ','))
   {
      throw FileException("Немає поля country у CityTour.");
   }

   if (!std::getline(iss, city, ','))
   {
      throw FileException("Немає поля city у CityTour.");
   }

   if (!std::getline(iss, accommodation, ','))
   {
      throw FileException("Немає поля accommodation у CityTour.");
   }

   if (!std::getline(iss, transport, ','))
   {
      throw FileException("Немає поля transport у CityTour.");
   }

   if (!std::getline(iss, departureDate, ','))
   {
      throw FileException("Немає поля departureDate у CityTour.");
   }

   if (!std::getline(iss, returnDate, ','))
   {
      throw FileException("Немає поля returnDate у CityTour.");
   }

   if (!std::getline(iss, hotelLevel, ','))
   {
      throw FileException("Немає поля hotelLevel у CityTour.");
   }

   if (!std::getline(iss, food, ','))
   {
      throw FileException("Немає поля food у CityTour.");
   }

   if (!std::getline(iss, extras, ','))
   {
      throw FileException("Немає поля extras у CityTour.");
   }

   if (!std::getline(iss, token))
   {
      throw FileException("Немає поля price у CityTour.");
   }

   try
   {
      price = std::stod(token);
   }
   catch (...)
   {
      throw FileException("Некоректне значення price у CityTour.");
   }
}

void CityTour::input()
{
   std::string p;

   std::cout << "Країна: ";
   std::getline(std::cin >> std::ws, country);

   std::cout << "Місто: ";
   std::getline(std::cin, city);

   std::cout << "Умови проживання: ";
   std::getline(std::cin, accommodation);

   std::cout << "Транспорт: ";
   std::getline(std::cin, transport);

   while (true)
   {
      std::cout << "Дата відправлення (YYYY-MM-DD): ";
      std::getline(std::cin, departureDate);

      if (!isValidDate(departureDate))
      {
         std::cout
            << "Некоректний формат дати. "
               "Використовуйте формат YYYY-MM-DD, наприклад 2024-05-17.\n";
         continue;
      }

      break;
   }

   while (true)
   {
      std::cout << "Дата повернення (YYYY-MM-DD): ";
      std::getline(std::cin, returnDate);

      if (!isValidDate(returnDate))
      {
         std::cout
            << "Некоректний формат дати. "
               "Використовуйте формат YYYY-MM-DD.\n";
         continue;
      }

      if (returnDate < departureDate)
      {
         std::cout
            << "Дата повернення не може бути раніше дати відправлення.\n";
         continue;
      }

      break;
   }

   while (true)
   {
      std::cout << "Рівень готелю (наприклад 3*): ";
      std::getline(std::cin, hotelLevel);

      if (!isValidHotelLevel(hotelLevel))
      {
         std::cout
            << "Некоректний рівень готелю. "
               "Використовуйте формат на кшталт 3*, 4*, 5*.\n";
         continue;
      }

      break;
   }

   std::cout << "Харчування: ";
   std::getline(std::cin, food);

   std::cout << "Додаткові вигоди: ";
   std::getline(std::cin, extras);

   while (true)
   {
      std::cout << "Вартість путівки: ";
      std::getline(std::cin, p);

      double parsedPrice = 0.0;
      if (!tryParsePrice(p, parsedPrice))
      {
         std::cout
            << "Некоректна ціна. Введіть додатне число, "
               "наприклад 12345 або 999.99.\n";
         continue;
      }

      price = parsedPrice;
      break;
   }
}

void CityTour::display() const
{
   std::cout << "[CITY] "
             << country << ", " << city
             << " | " << departureDate << " -> " << returnDate
             << " | " << hotelLevel
             << " | " << price << " грн\n";
}

std::string CityTour::toCSV() const
{
   std::ostringstream oss;

   oss << country << ','
       << city << ','
       << accommodation << ','
       << transport << ','
       << departureDate << ','
       << returnDate << ','
       << hotelLevel << ','
       << food << ','
       << extras << ','
       << price;

   return oss.str();
}

void CityTour::editInteractive()
{
   std::string tmp;

   std::cout << "Редагування міського туру. "
                "Залиште поле порожнім, щоб не змінювати.\n";

   std::cout << "Країна (" << country << "): ";
   std::getline(std::cin >> std::ws, tmp);
   if (!tmp.empty())
   {
      country = tmp;
   }

   std::cout << "Місто (" << city << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      city = tmp;
   }

   std::cout << "Умови проживання (" << accommodation << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      accommodation = tmp;
   }

   std::cout << "Транспорт (" << transport << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      transport = tmp;
   }

   std::cout << "Дата відправлення (" << departureDate << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      if (!isValidDate(tmp))
      {
         std::cerr
            << "Некоректний формат дати. "
               "Старе значення дати відправлення збережено.\n";
      }
      else if (!returnDate.empty() && tmp > returnDate)
      {
         std::cerr
            << "Нова дата відправлення не може бути пізніше дати повернення. "
               "Старе значення збережено.\n";
      }
      else
      {
         departureDate = tmp;
      }
   }

   std::cout << "Дата повернення (" << returnDate << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      if (!isValidDate(tmp))
      {
         std::cerr
            << "Некоректний формат дати. "
               "Старе значення дати повернення збережено.\n";
      }
      else if (!departureDate.empty() && tmp < departureDate)
      {
         std::cerr
            << "Дата повернення не може бути раніше дати відправлення. "
               "Старе значення збережено.\n";
      }
      else
      {
         returnDate = tmp;
      }
   }

   std::cout << "Рівень готелю (" << hotelLevel << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      if (!isValidHotelLevel(tmp))
      {
         std::cerr
            << "Некоректний рівень готелю. "
               "Старе значення залишено без змін.\n";
      }
      else
      {
         hotelLevel = tmp;
      }
   }

   std::cout << "Харчування (" << food << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      food = tmp;
   }

   std::cout << "Додаткові вигоди (" << extras << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      extras = tmp;
   }

   std::cout << "Вартість (" << price << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      double newPrice = 0.0;
      if (!tryParsePrice(tmp, newPrice))
      {
         std::cerr
            << "Помилка: некоректна ціна, старе значення збережено.\n";
      }
      else
      {
         price = newPrice;
      }
   }
}
