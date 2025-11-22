/// \file main.cpp
/// \brief Точка входу до програми «Довідник туриста».

#include <iostream>
#include <locale>
#include <limits>
#include <windows.h>
#include <cctype>
#include <string>

#include "AuthManager.h"
#include "TourManager.h"
#include "ValidationException.h"
#include "FileException.h"


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

   if (token[0] == '-')
   {
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

/// \brief Головна функція, що запускає застосунок.
/// \return Код завершення програми.
int main()
{
   SetConsoleCP(CP_UTF8);
   SetConsoleOutputCP(CP_UTF8);
   std::setlocale(LC_ALL, ".UTF8");

   AuthManager auth("data/users.txt");

   try
   {
      while (!auth.login())
      {
         std::cout << "Авторизація неуспішна, спробуйте ще раз.\n";
      }
   }
   catch (const ValidationException& ex)
   {
      std::cerr << ex.what() << "\n";
      return 1;
   }

   std::string currentUser = auth.getCurrentUser();
   std::cout << "Ласкаво просимо, " << currentUser << "!\n";

   TourManager tourManager("data/tours.csv");

   if (auth.isAdmin())
   {
      std::cout << "Ви увійшли як admin.\n";

      bool done = false;
      int choice = -1;
      std::string lastErrorMessage;

      while (!done)
      {
         if (!lastErrorMessage.empty())
         {
            std::cout << "\nПомилка введення: "
                      << lastErrorMessage << "\n\n";
            lastErrorMessage.clear();
         }

         std::cout <<   "___________________________\n";
         std::cout <<   "|                         |\n";
         std::cout <<   "|        Адмін меню       |\n";
         std::cout <<   "|                         |\n";
         std::cout <<   "| 1. Керувати турами      |\n";
         std::cout <<   "| 2. Список користувачів  |\n";
         std::cout <<   "| 3. Додати користувача   |\n";
         std::cout <<   "| 4. Видалити користувача |\n";
         std::cout <<   "| 0. Вийти                |\n";
         std::cout <<   "|                         |\n";
         std::cout <<   "___________________________\n";
         std::cout <<   "\n Вибір: ";

         if (!readStrictInt(choice))
         {
            std::cin.clear();
            std::cin.ignore(
               std::numeric_limits<std::streamsize>::max(),
               '\n');

            lastErrorMessage =
               "очікувалося ціле число без зайвих символів.";
            continue;
         }

         std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n');

         try
         {
            switch (choice)
            {
               case 1:
                  tourManager.mainMenu();
                  break;

               case 2:
               {
                  auto users = auth.listUsers();
                  std::cout << "Зареєстровані користувачі:\n";
                  for (const auto& u : users)
                  {
                     std::cout << " - " << u << "\n";
                  }
                  break;
               }

               case 3:
               {
                  std::string login;
                  std::string pass;

                  std::cout << "Новий логін: ";
                  std::getline(std::cin, login);

                  std::cout << "Пароль: ";
                  std::getline(std::cin, pass);

                  if (auth.addUser(login, pass))
                  {
                     std::cout << "Користувача додано.\n";
                  }
                  else
                  {
                     lastErrorMessage =
                        "Не вдалося додати користувача (можливо, вже існує).";
                  }
                  break;
               }

               case 4:
               {
                  std::string login;
                  std::cout << "Логін для видалення: ";
                  std::getline(std::cin, login);

                  if (auth.deleteUser(login))
                  {
                     std::cout << "Користувача видалено.\n";
                  }
                  else
                  {
                     lastErrorMessage =
                        "Не вдалося видалити (можливо не існує або це admin).";
                  }
                  break;
               }

               case 0:
                  done = true;
                  break;

               default:
                  lastErrorMessage = "Невірний вибір пункту адмін-меню.";
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
      }
   }
   else
   {
      try
      {
         tourManager.load();
      }
      catch (const FileException& ex)
      {
         std::cerr << ex.what() << "\n";
         std::cout << "Робота без попередніх турів.\n";
      }

      tourManager.userMenu(currentUser);

      try
      {
         tourManager.save();
      }
      catch (const FileException& ex)
      {
         std::cerr << ex.what() << "\n";
      }
   }

   return 0;
}
