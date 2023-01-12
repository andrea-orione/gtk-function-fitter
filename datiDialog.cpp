#include <gtkmm.h>
#include "datiDialog.h"

DatiDialog::DatiDialog(Gtk::Window *parente, int numero)
: Gtk::Dialog("Sceta dati"),
 datiScroll(),
 treeViewDati()
{
  set_transient_for(*parente);
  set_default_size(300, 500);
  set_resizable(false);
  set_deletable(false);
  mainBox = get_content_area();
  mainBox->set_orientation(Gtk::ORIENTATION_VERTICAL);
  mainBox->pack_start(datiScroll, Gtk::PACK_SHRINK, 0);
  modelloTreeView = Gtk::ListStore::create(modelloColonne);
  treeViewDati.set_model(modelloTreeView);
  treeViewDati.append_column_editable("X", modelloColonne.datiX);
  treeViewDati.append_column_editable("Y", modelloColonne.datiY);
  treeViewDati.append_column_editable("sX", modelloColonne.erroriX);
  treeViewDati.append_column_editable("sY", modelloColonne.erroriY);
  for (int i = 0; i < numero; i++)
  {
    righe = modelloTreeView->append();
    Gtk::TreeRow riga = *righe;
    riga[modelloColonne.datiX] = 0.0;
    riga[modelloColonne.datiY] = 0.0;
    riga[modelloColonne.erroriX] = 0.0;
    riga[modelloColonne.erroriY] = 0.0;
  }  
  datiScroll.add(treeViewDati);

  show_all();
  
  add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  add_button("_Ok", Gtk::RESPONSE_OK);
}