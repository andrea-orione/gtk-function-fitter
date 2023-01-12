#ifndef GTKMM_MYWINDOW_H
#define GTKMM_MYWINDOW_H

#include <gtkmm.h>
#include "TCanvas.h"
#include <filesystem>

class MyWindow : public Gtk::Window
{
public:
  MyWindow(std::filesystem::path);

protected:
  std::filesystem::path pathFileCSV;
  std::filesystem::path pathCacheFolder;
  int manuali;
  double *xManuali, *yManuali, *sxManuali, *syManuali, *x, *sx, *y, *sy;

  TCanvas *tela;

  void on_radioScelta_toggled(Glib::ustring);
  void on_entryNumero_changed();
  void on_bottoneManuale_clicked();
  void on_bottoneFile_clicked();
  void on_bottoneFit_clicked();
  bool scegliFile();
  void importazioneErrorDialog();
  void lunghezzaErrorDialog();
  void pochiDatiErrorDialog();

  Gtk::Box mainBox, boxFirst1, boxScelta, boxManuale, boxNumero, boxInsManuale, boxFile, boxFileScelta, boxFunzione;
  Gtk::Box boxFirst2;
  Gtk::Label labelScelta, labelNumero, LabelFile, labelFile, labelFunzione, labelFunzioneRes, labelParametriInfo, labelGrafico, labelChiSq, labelPValue, labelDoF;
  Gtk::Button bottoneManuale, bottoneFile, bottoneFit, bottoneSalva;
  Gtk::RadioButton radioManuale, radioFile;
  Gtk::SpinButton entryNumero;
  Gtk::CheckButton checkCompr;
  Gtk::Entry entryFunzione;
  Gtk::ScrolledWindow parametriScroll;
  Gtk::Image icona;
  Gtk::EventBox ebox;
  Gtk::TreeView treeViewParametri;
  Glib::RefPtr<Gtk::ListStore> modelloTreeView;

  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumns()
    { 
      add(columnParName);
      add(columnParValue);
    }
    Gtk::TreeModelColumn<Glib::ustring> columnParName;
    Gtk::TreeModelColumn<Glib::ustring> columnParValue;
  };

  ModelColumns modelloColonne;


};

#endif //GTKMM_MYWINDOW_H
