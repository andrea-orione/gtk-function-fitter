#include <gtkmm.h>
#include "fileDialog.h"

FileDialog::FileDialog(Gtk::Window *parente)
: Gtk::FileChooserDialog("Scegliere il file con i dati", Gtk::FILE_CHOOSER_ACTION_OPEN)
{
  set_transient_for(*parente);
  add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  add_button("_Open", Gtk::RESPONSE_OK);

  auto filtroCsv = Gtk::FileFilter::create();
  filtroCsv->set_name("Csv");
  filtroCsv->add_mime_type("text/csv");
  add_filter(filtroCsv);

  auto filtroTesto = Gtk::FileFilter::create();
  filtroTesto->set_name("Text files");
  filtroTesto->add_pattern("text/plain");
  add_filter(filtroTesto);
}