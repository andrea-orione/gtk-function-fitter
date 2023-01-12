#ifndef GTKMM_ENTRYONLYNUMBERS_H
#define GTKMM_ENTRYONLYNUMBERS_H

#include <gtkmm.h>

class EntryOnlyNumbers : public Gtk::Entry
{
public:
  EntryOnlyNumbers(bool);

protected:
  void on_insert_text(const Glib::ustring&, int*);

  bool segni;
};

#endif //GTKMM_ENTRYONLYNUMBERS_H