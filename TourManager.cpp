// TourManager.cpp

#include "TourManager.h"
#include "CityTour.h"
#include "SkiTour.h"
#include "FileException.h"
#include "ValidationException.h"
#include "NotFoundException.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <cctype>
#include <string>

namespace
{
bool readStrictInt(int& value)
{
   std::string token;

   if (!(std::cin >> token))
   {
      return false;
   }

   if (token.empty())
   {
      return false;
   }

   std::size_t start = 0;
   bool isNegative = false;

   if (token[0] == '-')
   {
      isNegative = true;
      start = 1;

      if (token.size() == 1)
      {
         return false;
      }
   }

   for (std::size_t i = start; i < token.size(); ++i)
   {
      unsigned char ch = static_cast<unsigned char>(token[i]);
      if (!std::isdigit(ch))
      {
         return false;
      }
   }

   try
   {
      value = std::stoi(token);
   }
   catch (...)
   {
      return false;
   }

   return true;
}
}

TourManager::TourManager(const std::string& dataFile)
   : dataFile(dataFile)
{
}

void TourManager::load()
{
   tours.clear();

   std::ifstream file(dataFile);
   if (!file)
   {
      throw FileException("Не вдалося відкрити файл турів: " + dataFile);
   }

   std::string header;
   if (!std::getline(file, header))
   {
      throw FileException("Файл турів порожній: " + dataFile);
   }

   std::string line;
   while (std::getline(file, line))
   {
      if (line.empty())
      {
         continue;
      }

      std::istringstream iss(line);
      std::string type;
      if (!std::getline(iss, type, ','))
      {
         std::cerr << "Пропущено рядок (немає типу).\n";
         continue;
      }

      std::string rest;
      std::getline(iss, rest);

      try
      {
         if (type == "city")
         {
            tours.push_back(std::make_shared<CityTour>(rest));
         }
         else if (type == "ski")
         {
            tours.push_back(std::make_shared<SkiTour>(rest));
         }
         else
         {
            std::cerr << "Невідомий тип туру: " << type << "\n";
         }
      }
      catch (const FileException& ex)
      {
         std::cerr << "Помилка читання туру: " << ex.what() << "\n";
      }
      catch (const std::exception& ex)
      {
         std::cerr << "Загальна помилка парсингу туру: "
                   << ex.what() << "\n";
      }
   }
}

void TourManager::save() const
{
   std::ofstream file(dataFile, std::ios::trunc);
   if (!file)
   {
      throw FileException(
         "Не вдалося відкрити файл турів для запису: " + dataFile);
   }

   file << "type,data\n";

   for (auto& tourPtr : tours)
   {
      if (auto cityTour =
              std::dynamic_pointer_cast<CityTour>(tourPtr))
      {
         file << "city," << cityTour->toCSV() << "\n";
      }
      else if (auto skiTour =
                  std::dynamic_pointer_cast<SkiTour>(tourPtr))
      {
         file << "ski," << skiTour->toCSV() << "\n";
      }
      else
      {

      }
   }
}

void TourManager::mainMenu()
{
   try
   {
      load();
   }
   catch (const FileException& ex)
   {
      std::cout << ex.what() << "\n";
      std::cout << "Робота буде продовжена з порожнім списком турів.\n";
   }

   int choice = -1;
   std::string lastErrorMessage;

   while (true)
   {
      if (!lastErrorMessage.empty())
      {
         std::cout << "\nПомилка введення: "
                   << lastErrorMessage << "\n\n";
         lastErrorMessage.clear();
      }

      std::cout << "\n_________________________________________________\n";
      std::cout <<  "|                                                |\n";
      std::cout <<  "|            ГОЛОВНЕ МЕНЮ ТУРІВ (ADMIN)          |\n";
      std::cout <<  "|                                                |\n";
      std::cout <<  "| 1. Переглянути всі тури                        |\n";
      std::cout <<  "| 2. Додати тур                                  |\n";
      std::cout <<  "| 3. Пошук турів                                 |\n";
      std::cout <<  "| 4. Сортування                                  |\n";
      std::cout <<  "| 5. Фільтрація                                  |\n";
      std::cout <<  "| 6. Редагувати тур                              |\n";
      std::cout <<  "| 7. Видалити тур                                |\n";
      std::cout <<  "| 8. Зберегти у файл                             |\n";
      std::cout <<  "| 9. Допомога                                    |\n";
      std::cout <<  "| 0. Вийти                                       |\n";
      std::cout <<  "|                                                |\n";
      std::cout <<   "_________________________________________________\n";
      std::cout <<   "\n Вибір: ";

      if (!readStrictInt(choice))
      {
         std::cin.clear();
         std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

         lastErrorMessage = "очікувалося ціле число без зайвих символів.";
         continue;
      }

      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(), '\n');

      try
      {
         switch (choice)
         {
            case 1:
               displayAll();
               break;

            case 2:
               addTour();
               break;

            case 3:
               searchMenu();
               break;

            case 4:
               sortMenu();
               break;

            case 5:
               filterMenu();
               break;

            case 6:
               editTour();
               break;

            case 7:
               deleteTour();
               break;

            case 8:
               save();
               std::cout << "Збережено.\n";
               break;

            case 9:
               helpInfoAdmin();
               break;

            case 0:
               save();
               std::cout << "Вихід. Збережено.\n";
               return;

            default:
               lastErrorMessage =
                  "Невірний вибір пункту меню адміністратора.";
               break;
         }
      }
      catch (const FileException& ex)
      {
         lastErrorMessage = ex.what();
      }
      catch (const ValidationException& ex)
      {
         lastErrorMessage = ex.what();
      }
      catch (const NotFoundException& ex)
      {
         lastErrorMessage = ex.what();
      }
   }
}


void TourManager::displayAll() const
{
   if (tours.empty())
   {
      std::cout << "Немає турів.\n";
      return;
   }

   int index = 0;
   for (auto& tourPtr : tours)
   {
      std::cout << index++ << ") ";
      tourPtr->display();
   }
}

void TourManager::addTour()
{

   std::cout << "\n__________________________________\n";
   std::cout << "|                                 |\n";
   std::cout << "|           Додати тур:           |\n";
   std::cout << "| 1. Міський тур (CityTour)       |\n";
   std::cout << "| 2. Гірськолижний тур (SkiTour)  |\n";
   std::cout << "|                                 |\n";
   std::cout << "__________________________________\n";
   std::cout <<   "\n Вибір: ";
   int typeChoice = 0;

   if (!readStrictInt(typeChoice))
   {
      std::cin.clear();
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw ValidationException("Некоректний вибір типу туру.");
   }

   std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(),
      '\n');

   std::shared_ptr<Tour> tourPtr;

   if (typeChoice == 1)
   {
      tourPtr = std::make_shared<CityTour>();
   }
   else if (typeChoice == 2)
   {
      tourPtr = std::make_shared<SkiTour>();
   }
   else
   {
      throw ValidationException("Невідомий тип туру.");
   }

   tourPtr->input();
   tours.push_back(tourPtr);

   std::cout << "Тур додано в пам'ять. "
                "Збережіть у файл для постійного зберігання.\n";
}

void TourManager::searchMenu() const
{
   if (tours.empty())
   {
      std::cout << "Список турів порожній.\n";
      return;
   }

   std::cout << "\n_______________________________\n";
   std::cout << "|                             |\n";
   std::cout << "|           Пошук за:         |\n";
   std::cout << "| 1. країна                   |\n";
   std::cout << "| 2. місто/курорт             |\n";
   std::cout << "| 3. дата (в межах)           |\n";
   std::cout << "|                             |\n";
   std::cout << "_______________________________\n";
   std::cout << "\n Вибір: ";

   int searchChoice = 0;

   if (!readStrictInt(searchChoice))
   {
      std::cin.clear();
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(), '\n');
      throw ValidationException(
         "Помилка введення: Некоректне введення пункту меню пошуку.");
   }

   std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(), '\n');

   int foundCount = 0;

   if (searchChoice == 1)
   {
      std::string country;
      std::cout << "\nКраїна: ";
      std::getline(std::cin, country);

      int index = 0;
      for (const auto& tourPtr : tours)
      {
         if (tourPtr->getCountry() == country)
         {
            std::cout << index << ") ";
            tourPtr->display();
            ++foundCount;
         }
         ++index;
      }

      if (foundCount == 0)
      {
         throw NotFoundException(
            "Помилка введення: Турів для країни '" + country + "' не знайдено.");
      }
   }
   else if (searchChoice == 2)
   {
      std::string city;
      std::cout << "\nМісто/курорт: ";
      std::getline(std::cin, city);

      int index = 0;
      for (const auto& tourPtr : tours)
      {
         if (tourPtr->getCity() == city)
         {
            std::cout << index << ") ";
            tourPtr->display();
            ++foundCount;
         }
         ++index;
      }

      if (foundCount == 0)
      {
         throw NotFoundException(
            "Помилка введення: Турів для міста/курорту '" + city + "' не знайдено.");
      }
   }
   else if (searchChoice == 3)
   {


      std::string fromDate;
      std::string toDate;

      std::cout << "\nПочаткова дата (YYYY-MM-DD, можна залишити порожньою): ";
      std::getline(std::cin, fromDate);

      std::cout << "Кінцева дата   (YYYY-MM-DD, можна залишити порожньою): ";
      std::getline(std::cin, toDate);

      int index = 0;
      for (const auto& tourPtr : tours)
      {
         const std::string date = tourPtr->getDepartureDate();

         bool okFrom = fromDate.empty() || date >= fromDate;
         bool okTo   = toDate.empty()   || date <= toDate;

         if (okFrom && okTo)
         {
            std::cout << index << ") ";
            tourPtr->display();
            ++foundCount;
         }
         ++index;
      }

      if (foundCount == 0)
      {
         throw NotFoundException(
            "Помилка введення: Турів у вказаному діапазоні дат не знайдено.");
      }
   }
   else
   {
      throw ValidationException(
         "Помилка введення: Некоректне введення пункту меню пошуку.");
   }
}


void TourManager::sortMenu()
{
   if (tours.empty())
   {
      std::cout << "Список турів порожній.\n";
      return;
   }

   std::cout << "\n_______________________________\n";
   std::cout << "|                             |\n";
   std::cout << "|         Сортувати за:       |\n";
   std::cout << "| 1. ціною                    |\n";
   std::cout << "| 2. датою відправлення       |\n";
   std::cout << "|                             |\n";
   std::cout << "_______________________________\n";
   std::cout <<   "\n Вибір: ";
   int sortChoice = 0;

   if (!readStrictInt(sortChoice))
   {
      std::cin.clear();
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw ValidationException(
         "Некоректне введення пункту меню сортування.");
   }

   std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(),
      '\n');

   if (sortChoice == 1)
   {
      std::sort(
         tours.begin(),
         tours.end(),
         [](const std::shared_ptr<Tour>& a,
            const std::shared_ptr<Tour>& b)
         {
            return a->getPrice() < b->getPrice();
         });

      std::cout << "Відсортовано за ціною.\n";
      displayAll();
   }
   else if (sortChoice == 2)
   {
      std::sort(
         tours.begin(),
         tours.end(),
         [](const std::shared_ptr<Tour>& a,
            const std::shared_ptr<Tour>& b)
         {
            return a->getDepartureDate()
                 < b->getDepartureDate();
         });

      std::cout << "Відсортовано за датою відправлення.\n";
      displayAll();
   }
   else
   {
      throw ValidationException(
         "Некоректне введення пункту меню сортування.");
   }
}

void TourManager::filterMenu() const
{
   if (tours.empty())
   {
      std::cout << "Список турів порожній.\n";
      return;
   }

   std::cout << "\n_______________________________\n";
   std::cout << "|                             |\n";
   std::cout << "|        Фільтрувати за:      |\n";
   std::cout << "| 1. рівнем готелю/складності |\n";
   std::cout << "| 2. макс ціна                |\n";
   std::cout << "|                             |\n";
   std::cout << "_______________________________\n";
   std::cout << "\n Вибір: ";

   int filterChoice = 0;

   if (!readStrictInt(filterChoice))
   {
      std::cin.clear();
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(), '\n');
      throw ValidationException("Некоректне введення пункту фільтрації.");
   }

   std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(),
      '\n');

   if (filterChoice == 1)
   {
      std::string level;
      std::cout << "Рівень готелю або складність "
                   "(наприклад 3* або Hard): ";
      std::getline(std::cin, level);

      int index = 0;
      for (const auto& tourPtr : tours)
      {
         if (tourPtr->getHotelLevel() == level)
         {
            std::cout << index << ") ";
            tourPtr->display();
         }
         ++index;
      }
   }
   else if (filterChoice == 2)
   {
      std::string input;
      std::cout << "Макс ціна: ";
      std::getline(std::cin, input);

      double maxPrice = 0.0;
      try
      {
         maxPrice = std::stod(input);
      }
      catch (...)
      {
         throw ValidationException("Некоректна максимальна ціна.");
      }

      int index = 0;
      for (const auto& tourPtr : tours)
      {
         if (tourPtr->getPrice() <= maxPrice)
         {
            std::cout << index << ") ";
            tourPtr->display();
         }
         ++index;
      }
   }
   else
   {
      throw ValidationException("Некоректне введення пункту фільтрації.");
   }
}


void TourManager::editTour()
{
   if (tours.empty())
   {
      std::cout << "Немає турів для редагування.\n";
      return;
   }

   displayAll();
   std::cout << "Введіть індекс туру для редагування: ";

   int index = 0;

   if (!readStrictInt(index))
   {
      std::cin.clear();
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw ValidationException("Некоректний формат індексу.");
   }

   if (index < 0 || index >= static_cast<int>(tours.size()))
   {
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw NotFoundException("Тур з таким індексом не існує.");
   }

   std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(),
      '\n');

   tours[static_cast<std::size_t>(index)]->editInteractive();

   std::cout << "Тур оновлено в пам'яті. "
                "Не забудьте зберегти у файл.\n";
}

void TourManager::deleteTour()
{
   if (tours.empty())
   {
      std::cout << "Немає турів для видалення.\n";
      return;
   }

   displayAll();
   std::cout << "Введіть індекс туру для видалення: ";

   int index = 0;

   if (!readStrictInt(index))
   {
      std::cin.clear();
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw ValidationException("Некоректний формат індексу.");
   }

   if (index < 0 || index >= static_cast<int>(tours.size()))
   {
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw NotFoundException("Тур з таким індексом не існує.");
   }

   std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(),
      '\n');

   tours.erase(tours.begin() + index);

   std::cout << "Тур видалено з пам'яті. "
                "Не забудьте зберегти у файл.\n";
}

bool TourManager::dateInRange(const std::string& date,
   const std::string& from,
   const std::string& to)
{
   if (from.empty() && to.empty())
   {
      return true;
   }

   if (!from.empty() && date < from)
   {
      return false;
   }

   if (!to.empty() && date > to)
   {
      return false;
   }

   return true;
}

void TourManager::userMenu(const std::string& username)
{
   int choice = -1;
   std::string lastErrorMessage;

   while (true)
   {
      if (!lastErrorMessage.empty())
      {
         std::cout << "\nПомилка введення: "
                   << lastErrorMessage << "\n\n";
         lastErrorMessage.clear();
      }

      std::cout << "\n____________________________\n";
      std::cout <<   "|                          |\n";
      std::cout <<   "|      МЕНЮ КОРИСТУВАЧА    |\n";
      std::cout <<   "|                          |\n";
      std::cout <<   "| 1. Переглянути усі тури  |\n";
      std::cout <<   "| 2. Сортувати тури        |\n";
      std::cout <<   "| 3. Фільтрувати тури      |\n";
      std::cout <<   "| 4. Замовити квиток       |\n";
      std::cout <<   "| 5. Допомога              |\n";
      std::cout <<   "| 0. Вийти                 |\n";
      std::cout <<   "|                          |\n";
      std::cout <<   "____________________________\n";
      std::cout <<   "\n Виберіть опцію: ";

      if (!readStrictInt(choice))
      {
         std::cin.clear();
         std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(), '\n');

         lastErrorMessage = "очікувалося ціле число без зайвих символів.";
         continue;
      }

      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(), '\n');

      try
      {
         switch (choice)
         {
            case 1:
               listAllTours();
               break;

            case 2:
               sortMenu();
               break;

            case 3:
               filterMenu();
               break;

            case 4:
               bookTicket(username);
               break;

            case 5:
               helpInfoUser();
               break;

            case 0:
               std::cout << "Вихід у головне меню...\n";
               return;

            default:
               lastErrorMessage =
                  "Невірний вибір пункту меню користувача.";
               break;
         }
      }
      catch (const ValidationException& ex)
      {

         lastErrorMessage = ex.what();
      }
      catch (const FileException& ex)
      {
         lastErrorMessage = ex.what();
      }
      catch (const NotFoundException& ex)
      {
         lastErrorMessage = ex.what();
      }
   }
}


void TourManager::bookTicket(const std::string& username)
{
   if (tours.empty())
   {
      std::cout << "Немає доступних турів для замовлення.\n";
      return;
   }

   std::cout << "\n=== Замовлення квитка ===\n";
   displayAll();

   std::cout
      << "Введіть індекс туру для бронювання (починаючи з 0): ";

   int index = 0;

   if (!readStrictInt(index))
   {
      std::cin.clear();
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw ValidationException(
         "Некоректний формат індексу для бронювання.");
   }

   if (index < 0 || index >= static_cast<int>(tours.size()))
   {
      std::cin.ignore(
         std::numeric_limits<std::streamsize>::max(),
         '\n');
      throw NotFoundException("Тур з таким індексом не існує.");
   }

   std::cin.ignore(
      std::numeric_limits<std::streamsize>::max(),
      '\n');

   try
   {
      std::ofstream file("data/tickets.txt", std::ios::app);
      if (!file)
      {
         throw FileException(
            "Помилка відкриття файлу data/tickets.txt.");
      }

      file.seekp(0, std::ios::end);
      if (file.tellp() == 0)
      {
         file << "username,country,city,"
                 "departureDate,returnDate,price\n";
      }

      auto& tour = tours[static_cast<std::size_t>(index)];

      file << username << ","
           << tour->getCountry() << ","
           << tour->getCity() << ","
           << tour->getDepartureDate() << ","
           << tour->getReturnDate() << ","
           << tour->getPrice() << "\n";

      std::cout << "Тур \"" << tour->getCity()
                << "\" успішно заброньовано!\n";
   }
   catch (const FileException& ex)
   {
      std::cerr << ex.what() << "\n";
   }
}

void TourManager::helpInfoAdmin() const
{
   std::cout << "\n_________________________________________________\n";
   std::cout <<   "|                                               |\n";
   std::cout <<   "|                 ДОПОМОГА (ADMIN)              |\n";
   std::cout <<   "|                                               |\n";
   std::cout <<   "| 1. Переглянути усі тури — показує список усіх |\n";
   std::cout <<   "| доступних турів.                              |\n";
   std::cout <<   "| 2. Додати тур — створити City або Ski тур.    |\n";
   std::cout <<   "| 3. Пошук турів — по країні, місту/курорту або |\n";
   std::cout <<   "| діапазону дат.                                |\n";
   std::cout <<   "| 4. Сортування — за ціною або датою            |\n";
   std::cout <<   "| відправлення.                                 |\n";
   std::cout <<   "| 5. Фільтрація — за рівнем готелю/складністю   |\n";
   std::cout <<   "| або максимальною ціною.                       |\n";
   std::cout <<   "| 6. Редагувати тур — змінити поля вибраного    |\n";
   std::cout <<   "| туру.                                         |\n";
   std::cout <<   "| 7. Видалити тур — вилучити тур зі списку.     |\n";
   std::cout <<   "| 8. Зберегти у файл — записати всі тури в CSV. |\n";
   std::cout <<   "| 9. Допомога — показує це меню.                |\n";
   std::cout <<   "| 0. Вийти — вихід з меню турів зі збереженням. |\n";
   std::cout <<   "|                                               |\n";
   std::cout <<   "_________________________________________________\n";
}

void TourManager::helpInfoUser() const
{
   std::cout << "\n_________________________________________________\n";
   std::cout <<  "|                                                |\n";
   std::cout <<  "|                 ДОПОМОГА (USER)                |\n";
   std::cout <<  "|                                                |\n";
   std::cout <<  "| 1. Переглянути усі тури — перегляд доступних   |\n";
   std::cout <<  "| City та Ski турів.                             |\n";
   std::cout <<  "| 2. Сортувати тури — впорядкування за ціною     |\n";
   std::cout <<  "| або датою.                                     |\n";
   std::cout <<  "| 3. Фільтрувати тури — відбір за рівнем         |\n";
   std::cout <<  "| готелю/складністю чи ціною.                    |\n";
   std::cout <<  "| 4. Замовити квиток — бронювання туру, дані     |\n";
   std::cout <<  "| записуються у tickets.txt.                     |\n";
   std::cout <<  "| 5. Допомога — це пояснення.                    |\n";
   std::cout <<  "| 0. Вийти — повернення у головне меню           |\n";
   std::cout <<  "| програми.                                      |\n";
   std::cout <<  "|                                                |\n";
   std::cout <<   "_________________________________________________";
}
