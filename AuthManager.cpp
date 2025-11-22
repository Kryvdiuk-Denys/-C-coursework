// AuthManager.cpp

#include "AuthManager.h"
#include "FileException.h"
#include "ValidationException.h"

#include <fstream>
#include <iostream>
#include <sstream>

AuthManager::AuthManager(const std::string& usersFile)
   : usersFile(usersFile)
{
}

bool AuthManager::login()
{
   std::string username;
   std::string password;

   std::cout << "Логін: ";
   if (!(std::cin >> username))
   {
      throw ValidationException("Некоректне введення логіна.");
   }

   std::cout << "Пароль: ";
   if (!(std::cin >> password))
   {
      throw ValidationException("Некоректне введення пароля.");
   }

   try
   {
      if (validateCredentials(username, password))
      {
         currentUser = username;
         return true;
      }

      std::cout << "Невірний логін або пароль.\n";
      return false;
   }
   catch (const FileException& ex)
   {
      std::cerr << ex.what() << "\n";
      return false;
   }
}

std::string AuthManager::getCurrentUser() const
{
   return currentUser;
}

bool AuthManager::isAdmin() const
{
   return currentUser == "admin";
}

bool AuthManager::validateCredentials(const std::string& login,
   const std::string& password) const
{
   std::ifstream file(usersFile);
   if (!file)
   {
      throw FileException(
         "Не вдалося відкрити файл користувачів: " + usersFile);
   }

   std::string line;
   while (std::getline(file, line))
   {
      if (line.empty())
      {
         continue;
      }

      std::istringstream iss(line);
      std::string u;
      std::string p;

      if (std::getline(iss, u, ':') && std::getline(iss, p))
      {
         if (u == login && p == password)
         {
            return true;
         }
      }
   }

   return false;
}

std::vector<std::string> AuthManager::listUsers() const
{
   std::vector<std::string> out;

   std::ifstream file(usersFile);
   if (!file)
   {
      throw FileException(
         "Не вдалося відкрити файл користувачів: " + usersFile);
   }

   std::string line;
   while (std::getline(file, line))
   {
      if (line.empty())
      {
         continue;
      }

      auto pos = line.find(':');
      if (pos != std::string::npos)
      {
         out.push_back(line.substr(0, pos));
      }
   }

   return out;
}

bool AuthManager::addUser(const std::string& username,
   const std::string& password)
{
   auto users = listUsers();

   for (const auto& u : users)
   {
      if (u == username)
      {
         std::cout << "Користувач з таким логіном вже існує.\n";
         return false;
      }
   }

   std::ofstream file(usersFile, std::ios::app);
   if (!file)
   {
      throw FileException(
         "Не вдалося відкрити файл користувачів для запису: " + usersFile);
   }

   file << username << ':' << password << '\n';
   return true;
}

bool AuthManager::deleteUser(const std::string& username)
{
   if (username == "admin")
   {
      std::cout << "Неможливо видалити admin.\n";
      return false;
   }

   std::ifstream file(usersFile);
   if (!file)
   {
      throw FileException(
         "Не вдалося відкрити файл користувачів: " + usersFile);
   }

   std::vector<std::string> lines;
   std::string line;
   bool found = false;

   while (std::getline(file, line))
   {
      if (line.empty())
      {
         continue;
      }

      auto pos = line.find(':');
      if (pos != std::string::npos)
      {
         std::string u = line.substr(0, pos);
         if (u == username)
         {
            found = true;
            continue;
         }
      }

      lines.push_back(line);
   }

   file.close();

   if (!found)
   {
      std::cout << "Користувача не знайдено.\n";
      return false;
   }

   std::ofstream out(usersFile, std::ios::trunc);
   if (!out)
   {
      throw FileException(
         "Не вдалося відкрити файл користувачів для запису: " + usersFile);
   }

   for (const auto& l : lines)
   {
      out << l << '\n';
   }

   return true;
}
