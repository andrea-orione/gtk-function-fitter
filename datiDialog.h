#ifndef GTKMM_DATIDIALOG_H
#define GTKMM_DATIDIALOG_H

#include <gtkmm.h>

class DatiDialog : public Gtk::Dialog
{
public:
  DatiDialog(Gtk::Window *, int);

protected:
  Gtk::Box *mainBox;
  Gtk::ScrolledWindow datiScroll;

  Gtk::TreeView treeViewDati;
  Gtk::TreeModel::iterator righe;
  Glib::RefPtr<Gtk::ListStore> modelloTreeView;

  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumns()
    { 
      add(datiX);
      add(datiY);
      add(erroriX);
      add(erroriY);
    }
    Gtk::TreeModelColumn<double> datiX;
    Gtk::TreeModelColumn<double> datiY;
    Gtk::TreeModelColumn<double> erroriX;
    Gtk::TreeModelColumn<double> erroriY;
  };

  ModelColumns modelloColonne;

};

#endif //GTKMM_DATIDIALOG_H