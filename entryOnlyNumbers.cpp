#include <gtkmm.h>
#include <cstring>
#include <exception>
#include "entryOnlyNumbers.h"

EntryOnlyNumbers::EntryOnlyNumbers(bool segniInput)
: Gtk::Entry()
{
  segni = segniInput;
}

void EntryOnlyNumbers::on_insert_text(const Glib::ustring& new_text, int* position)
{
  std::string inserente = std::string(new_text);
  int i = 0;
  while ((i = inserente.find(",", i)) != std::string::npos) {
    inserente.replace(i, std::string(",").length(), std::string("."));
    i += std::string(".").length(); // Handles case where 'to' is a substring of 'from'
  }
  std::string vecchiaStringa = std::string(get_text());
  std::string nuovoTest = vecchiaStringa.insert(*position, inserente);
  try {
    double test = std::stod(nuovoTest);
    if (test<0 and !segni) return;
    Gtk::Entry::on_insert_text(inserente, position);
    return;
  } catch (std::invalid_argument er) {
    return;
  }
}