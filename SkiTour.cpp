/// \file SkiTour.cpp
/// \brief Реалізація класу SkiTour — гірськолижний тур.

#include "SkiTour.h"
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

SkiTour::SkiTour(const std::string& csvLine)
{
   std::istringstream iss(csvLine);
   std::string token;

   if (!std::getline(iss, country, ','))
   {
      throw FileException("Немає поля country у SkiTour.");
   }

   if (!std::getline(iss, resort, ','))
   {
      throw FileException("Немає поля resort у SkiTour.");
   }

   if (!std::getline(iss, difficulty, ','))
   {
      throw FileException("Немає поля difficulty у SkiTour.");
   }

   if (!std::getline(iss, token, ','))
   {
      throw FileException("Немає поля equipmentIncluded у SkiTour.");
   }
   equipmentIncluded = (token == "1");

   if (!std::getline(iss, token, ','))
   {
      throw FileException("Немає поля insuranceIncluded у SkiTour.");
   }
   insuranceIncluded = (token == "1");

   if (!std::getline(iss, departureDate, ','))
   {
      throw FileException("Немає поля departureDate у SkiTour.");
   }

   if (!std::getline(iss, returnDate, ','))
   {
      throw FileException("Немає поля returnDate у SkiTour.");
   }

   if (!std::getline(iss, token))
   {
      throw FileException("Немає поля price у SkiTour.");
   }

   try
   {
      price = std::stod(token);
   }
   catch (...)
   {
      throw FileException("Некоректна ціна у SkiTour.");
   }
}

SkiTour::SkiTour(const SkiTour& other)
   : country(other.country),
     resort(other.resort),
     difficulty(other.difficulty),
     equipmentIncluded(other.equipmentIncluded),
     insuranceIncluded(other.insuranceIncluded),
     departureDate(other.departureDate),
     returnDate(other.returnDate),
     price(other.price)
{
   std::cout << "[Copy] SkiTour скопійовано\n";
}

SkiTour::SkiTour(SkiTour&& other) noexcept
   : country(std::move(other.country)),
     resort(std::move(other.resort)),
     difficulty(std::move(other.difficulty)),
     equipmentIncluded(other.equipmentIncluded),
     insuranceIncluded(other.insuranceIncluded),
     departureDate(std::move(other.departureDate)),
     returnDate(std::move(other.returnDate)),
     price(other.price)
{
   std::cout << "[Move] SkiTour переміщено\n";
}

SkiTour::~SkiTour()
{
   std::cout << "[Destructor] SkiTour видалено\n";
}

void SkiTour::input()
{
   std::string tmp;

   std::cout << "Країна: ";
   std::getline(std::cin >> std::ws, country);

   std::cout << "Гірськолижний курорт: ";
   std::getline(std::cin, resort);

   while (true)
   {
      std::cout << "Складність:\n"
                   " 1. Easy\n"
                   " 2. Medium\n"
                   " 3. Hard\n"
                   "Вибір: ";
      std::getline(std::cin, tmp);

      if (tmp == "1")
      {
         difficulty = "Easy";
         break;
      }
      else if (tmp == "2")
      {
         difficulty = "Medium";
         break;
      }
      else if (tmp == "3")
      {
         difficulty = "Hard";
         break;
      }
      else
      {
         std::cout << "Некоректний вибір. Введіть 1, 2 або 3.\n";
      }
   }

   while (true)
   {
      std::cout << "Спорядження включено?\n"
                   " 1. Так\n"
                   " 2. Ні\n"
                   "Вибір: ";
      std::getline(std::cin, tmp);

      if (tmp == "1")
      {
         equipmentIncluded = true;
         break;
      }
      else if (tmp == "2")
      {
         equipmentIncluded = false;
         break;
      }
      else
      {
         std::cout << "Некоректний вибір. Введіть 1 або 2.\n";
      }
   }

   while (true)
   {
      std::cout << "Страхування включено?\n"
                   " 1. Так\n"
                   " 2. Ні\n"
                   "Вибір: ";
      std::getline(std::cin, tmp);

      if (tmp == "1")
      {
         insuranceIncluded = true;
         break;
      }
      else if (tmp == "2")
      {
         insuranceIncluded = false;
         break;
      }
      else
      {
         std::cout << "Некоректний вибір. Введіть 1 або 2.\n";
      }
   }

   while (true)
   {
      std::cout << "Дата відправлення (YYYY-MM-DD): ";
      std::getline(std::cin, departureDate);

      if (!isValidDate(departureDate))
      {
         std::cout
            << "Некоректний формат дати. "
               "Використовуйте формат YYYY-MM-DD, наприклад 2024-01-15.\n";
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
      std::cout << "Ціна туру: ";
      std::getline(std::cin, tmp);

      double parsedPrice = 0.0;
      if (!tryParsePrice(tmp, parsedPrice))
      {
         std::cout
            << "Некоректна ціна. Введіть додатне число, "
               "наприклад 15000 або 999.99.\n";
         continue;
      }

      price = parsedPrice;
      break;
   }
}

void SkiTour::display() const
{
   std::cout << "[SKI] " << country << ", " << resort
             << " | Складність: " << difficulty
             << " | Спорядження: " << (equipmentIncluded ? "так" : "ні")
             << " | Страхування: " << (insuranceIncluded ? "так" : "ні")
             << " | " << departureDate << " -> " << returnDate
             << " | " << price << " грн\n";
}

std::string SkiTour::toCSV() const
{
   std::ostringstream oss;

   oss << country << ','
       << resort << ','
       << difficulty << ','
       << (equipmentIncluded ? "1" : "0") << ','
       << (insuranceIncluded ? "1" : "0") << ','
       << departureDate << ','
       << returnDate << ','
       << price;

   return oss.str();
}

void SkiTour::editInteractive()
{
   std::string tmp;

   std::cout << "Редагування Ski туру. "
                "Залиште порожнім, щоб не змінювати.\n";

   std::cout << "Країна (" << country << "): ";
   std::getline(std::cin >> std::ws, tmp);
   if (!tmp.empty())
   {
      country = tmp;
   }

   std::cout << "Курорт (" << resort << "): ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      resort = tmp;
   }

   std::cout << "Складність (поточна: " << difficulty
             << "). Введіть 1-Easy, 2-Medium, 3-Hard або порожньо, щоб не змінювати: ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      if (tmp == "1")
      {
         difficulty = "Easy";
      }
      else if (tmp == "2")
      {
         difficulty = "Medium";
      }
      else if (tmp == "3")
      {
         difficulty = "Hard";
      }
      else
      {
         std::cerr
            << "Некоректний вибір складності, старе значення збережено.\n";
      }
   }

   std::cout << "Спорядження (зараз: "
             << (equipmentIncluded ? "так" : "ні")
             << "). Введіть 1 - так, 2 - ні або порожньо, щоб не змінювати: ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      if (tmp == "1")
      {
         equipmentIncluded = true;
      }
      else if (tmp == "2")
      {
         equipmentIncluded = false;
      }
      else
      {
         std::cerr
            << "Некоректний вибір, значення спорядження не змінено.\n";
      }
   }

   std::cout << "Страхування (зараз: "
             << (insuranceIncluded ? "так" : "ні")
             << "). Введіть 1 - так, 2 - ні або порожньо, щоб не змінювати: ";
   std::getline(std::cin, tmp);
   if (!tmp.empty())
   {
      if (tmp == "1")
      {
         insuranceIncluded = true;
      }
      else if (tmp == "2")
      {
         insuranceIncluded = false;
      }
      else
      {
         std::cerr
            << "Некоректний вибір, значення страхування не змінено.\n";
      }
   }

   std::cout << "Дата відправлення (" << departureDate
             << ") [YYYY-MM-DD]: ";
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

   std::cout << "Дата повернення (" << returnDate
             << ") [YYYY-MM-DD]: ";
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

   std::cout << "Ціна (" << price << "): ";
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
