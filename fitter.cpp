#include <gtkmm.h>
#include "window.h"
#include "fileDialog.h"
#include "datiDialog.h"
#include "entryOnlyNumbers.h"
#include <cstring>
#include <filesystem>

int my_quit(std::filesystem::path);

int main(int argc, char** argv)
{
  auto app = Gtk::Application::create("org.gtkmm.examples.base");
  std::filesystem::path cacheFolderPath = std::filesystem::current_path().append(".fitterCache");
  std::filesystem::create_directory(cacheFolderPath);
  MyWindow window(cacheFolderPath);
  int statusApp = app->run(window);
  int statusQuit = my_quit(cacheFolderPath);
  return statusApp + statusQuit;
}

int my_quit(std::filesystem::path cacheFolder)
{
  std::filesystem::remove_all(cacheFolder);
  return 0;
}
