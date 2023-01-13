#ifndef GTKMM_MYWINDOW_H
#define GTKMM_MYWINDOW_H

#include <gtkmm.h>
#include "TCanvas.h"
#include <filesystem>
#include "entryOnlyNumbers.h"

class MyWindow : public Gtk::Window
{
public:
  MyWindow(std::filesystem::path);

protected:
  std::filesystem::path pathFileCSV;
  std::filesystem::path pathCacheFolder;
  std::filesystem::path pathBlankPdf;
  int manuali;
  double *xManuali, *yManuali, *sxManuali, *syManuali, *x, *sx, *y, *sy;

  void on_radioScelta_toggled(Glib::ustring);
  void on_entryNumero_changed();
  void on_bottoneManuale_clicked();
  void on_bottoneFile_clicked();
  void on_bottoneFit_clicked();
  bool scegliFile();
  void importazioneErrorDialog();
  void lunghezzaErrorDialog();
  void pochiDatiErrorDialog();
  void generateBlankPdf();

  Gtk::Box mainBox, boxFirst1, boxScelta, boxManuale, boxNumero, boxInsManuale, boxFile, boxFileScelta;
  Gtk::Box boxRange, boxSetRange, boxFunzione;
  Gtk::Box boxFirst2;
  Gtk::Label labelScelta, labelNumero, LabelFile, labelFile, labelFunzione, labelFunzioneRes, labelRange, labelDaRange, labelARange, labelIntiParam;
  Gtk::Label labelParametriInfo, labelGrafico, labelChiSq, labelPValue, labelDoF;
  Gtk::Button bottoneManuale, bottoneFile, bottoneFit, bottoneSalva;
  Gtk::RadioButton radioManuale, radioFile;
  Gtk::SpinButton entryNumero;
  Gtk::CheckButton checkLogaritmico;
  Gtk::Entry entryFunzione;
  EntryOnlyNumbers entryDaRange, entryARange;
  Gtk::ScrolledWindow parametriScroll, parametriInitScroll;
  Gtk::Image icona;
  Gtk::TreeView treeViewParametri, treeViewInitPars;
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
