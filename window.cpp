#include <gtkmm.h>
#include <gdk/gdkx.h>
#include "window.h"
#include "fileDialog.h"
#include "datiDialog.h"
#include "entryOnlyNumbers.h"
#include "TCanvas.h"
#include "TH1.h"
#include <cstring>
#include <exception>
#include <filesystem>

namespace fs = std::filesystem;

struct AnnullaError : public std::exception {
  const char * what() const throw() {
    return "Annulla error";
  }
};
struct LunghezzaError : public std::exception {
  const char * what() const throw() {
    return "Lunghezza error";
  }
};
struct PochiDatiError : public std::exception {
  const char * what() const throw() {
    return "Pochi dati error";
  }
};
struct ImportazioneError : public std::exception {
  const char * what() const throw() {
    return "Importazione error";
  }
};

MyWindow::MyWindow(fs::path cachePath)
: mainBox(Gtk::ORIENTATION_HORIZONTAL),
 boxFirst1(Gtk::ORIENTATION_VERTICAL),
 boxScelta(Gtk::ORIENTATION_VERTICAL),
 boxManuale(Gtk::ORIENTATION_VERTICAL),
 boxNumero(Gtk::ORIENTATION_HORIZONTAL),
 boxInsManuale(Gtk::ORIENTATION_HORIZONTAL),
 boxFile(Gtk::ORIENTATION_VERTICAL),
 boxFileScelta(Gtk::ORIENTATION_HORIZONTAL),
 boxRange(Gtk::ORIENTATION_VERTICAL),
 boxSetRange(Gtk::ORIENTATION_HORIZONTAL),
 boxFunzione(Gtk::ORIENTATION_VERTICAL),
 boxFirst2(Gtk::ORIENTATION_VERTICAL),

 labelScelta("Scegliere il tipo di input:"),
 labelNumero("Numero di dati:"),
 labelFile("File: "),
 labelRange("Selezionare il range in cui fittare"),
 labelDaRange("Da: "),
 labelARange("A: "),
 labelFunzione("Scrivere la fuzione da fittare"),
 labelIntiParam("Valori inizializzazione parametri"),

 labelParametriInfo("Parametri stimati"),
 labelGrafico("Grafico"),
 labelChiSq("Chi quadro: "),
 labelPValue("P-value: "),
 labelDoF("Gradi di libertà"),

 bottoneManuale("Scegli dati"),
 bottoneFile("Apri File"),
 bottoneFit("Esegui fit"),
 bottoneSalva("Salva grafico"),

 radioManuale("Inserimento manuale"),
 radioFile("Dati da File"),

 checkLogaritmico("Scala logaritmica"),

 entryNumero(),
 entryFunzione(),

 entryDaRange(true),
 entryARange(true),

 icona(),
 //grafico(),

 parametriScroll(),
 parametriInitScroll(),
 treeViewInitPars(),
 treeViewParametri()
{
  // Setting start-up variables
  manuali = -1;
  pathFileCSV = fs::path("");
  pathCacheFolder = cachePath;
  pathBlankPdf = pathCacheFolder;
  pathBlankPdf.append("BlankGraph.pdf");

  generateBlankPdf();

  // WINDOW STRUCTURE
  set_title("Fitter");

  // set_default_size(400, 400);
  mainBox.set_homogeneous(false);
  mainBox.set_spacing(10);
  mainBox.set_margin_end(15);
  mainBox.set_margin_start(10);
  mainBox.set_margin_top(10);
  mainBox.set_margin_bottom(10);
  add(mainBox);

  //Parte Sinistra
  boxFirst1.set_homogeneous(false);
  boxFirst1.set_size_request(300, -1);
  mainBox.pack_start(boxFirst1, Gtk::PACK_SHRINK, 1);

  boxScelta.set_homogeneous(false);
  boxScelta.set_margin_bottom(40);
  boxFirst1.pack_start(boxScelta, Gtk::PACK_SHRINK, 1);

  labelScelta.set_xalign(0);
  boxScelta.pack_start(labelScelta, Gtk::PACK_SHRINK, 1);

  boxManuale.set_homogeneous(false);
  boxManuale.set_margin_top(5);
  boxManuale.set_margin_bottom(5);
  boxScelta.pack_start(boxManuale, Gtk::PACK_SHRINK, 1);

  radioManuale.signal_clicked().connect(sigc::bind<-1, Glib::ustring>(sigc::mem_fun(*this, &MyWindow::on_radioScelta_toggled), "manuale"));
  boxManuale.pack_start(radioManuale, Gtk::PACK_SHRINK, 1);
  boxNumero.set_homogeneous(false);
  boxNumero.set_margin_start(30);
  boxManuale.pack_start(boxNumero, Gtk::PACK_SHRINK, 1);
  labelNumero.set_margin_end(10);
  boxNumero.pack_start(labelNumero, Gtk::PACK_SHRINK, 1);
  entryNumero.set_range(2,99);
  entryNumero.set_increments(1,2);
  entryNumero.set_value(2);
  entryNumero.signal_value_changed().connect(sigc::mem_fun(*this, &MyWindow::on_entryNumero_changed));
  boxNumero.pack_start(entryNumero, Gtk::PACK_SHRINK, 1);
  boxManuale.pack_start(boxInsManuale, Gtk::PACK_SHRINK, 1);
  bottoneManuale.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_bottoneManuale_clicked));
  bottoneManuale.set_margin_start(30);
  bottoneManuale.set_margin_end(10);
  boxInsManuale.pack_start(bottoneManuale, Gtk::PACK_SHRINK, 1);
  icona.set_from_icon_name("dialog-error-symbolic",Gtk::IconSize(2.));
  boxInsManuale.pack_start(icona, Gtk::PACK_SHRINK, 1);

  boxScelta.pack_start(boxFile, Gtk::PACK_SHRINK, 1);
  radioFile.join_group(radioManuale);
  radioFile.signal_clicked().connect(sigc::bind<-1, Glib::ustring>(sigc::mem_fun(*this, &MyWindow::on_radioScelta_toggled), "file"));
  boxFile.set_homogeneous(false);
  boxFile.pack_start(radioFile, Gtk::PACK_SHRINK, 1);
  boxFileScelta.set_homogeneous(false);
  boxFileScelta.set_margin_start(30);
  boxFile.pack_start(boxFileScelta, Gtk::PACK_SHRINK, 1);
  bottoneFile.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_bottoneFile_clicked));
  bottoneFile.set_sensitive(false);
  bottoneFile.set_margin_end(10);
  boxFileScelta.pack_start(bottoneFile, Gtk::PACK_SHRINK, 1);
  boxFileScelta.pack_start(labelFile, Gtk::PACK_SHRINK, 1);

  boxRange.set_homogeneous(false);
  boxRange.set_margin_bottom(20);
  boxFirst1.pack_start(boxRange, Gtk::PACK_SHRINK, 1);
  labelRange.set_xalign(0);
  labelRange.set_margin_bottom(4);
  boxRange.pack_start(labelRange, Gtk::PACK_SHRINK);
  boxSetRange.set_homogeneous(false);
  boxRange.pack_start(boxSetRange, Gtk::PACK_SHRINK, 1);
  labelDaRange.set_xalign(0);
  labelDaRange.set_margin_end(1);
  entryDaRange.set_margin_end(10);
  labelARange.set_xalign(0);
  labelARange.set_margin_end(1);
  boxSetRange.pack_start(labelDaRange, Gtk::PACK_SHRINK, 1);
  boxSetRange.pack_start(entryDaRange, Gtk::PACK_SHRINK, 1);
  boxSetRange.pack_start(labelARange, Gtk::PACK_SHRINK, 1);
  boxSetRange.pack_start(entryARange, Gtk::PACK_SHRINK, 1);

  boxFunzione.set_homogeneous(false);
  boxFunzione.set_margin_bottom(40);
  boxFirst1.pack_start(boxFunzione, Gtk::PACK_SHRINK, 1);

  labelFunzione.set_xalign(0);
  labelFunzione.set_margin_bottom(4);
  boxFunzione.pack_start(labelFunzione, Gtk::PACK_SHRINK, 1);
  boxFunzione.pack_start(entryFunzione, Gtk::PACK_SHRINK, 1);

  labelIntiParam.set_xalign(0);
  boxFirst1.pack_start(labelIntiParam, Gtk::PACK_SHRINK, 1);
  parametriInitScroll.set_size_request(400,150);
  parametriInitScroll.set_margin_bottom(20);
  boxFirst1.pack_start(parametriInitScroll, Gtk::PACK_SHRINK, 1);
  modelloTreeView = Gtk::ListStore::create(modelloColonne);
  treeViewInitPars.set_model(modelloTreeView);
  treeViewInitPars.append_column_editable("Parametro", modelloColonne.columnParName);
  treeViewInitPars.append_column_editable("Valore", modelloColonne.columnParValue);
  parametriInitScroll.add(treeViewInitPars);

  checkLogaritmico.set_margin_bottom(10);
  boxFirst1.pack_start(checkLogaritmico, Gtk::PACK_SHRINK, 1);

  bottoneFit.signal_clicked().connect(sigc::mem_fun(*this, &MyWindow::on_bottoneFit_clicked));
  bottoneFit.set_hexpand(false);
  boxFirst1.pack_start(bottoneFit, Gtk::PACK_SHRINK, 1);

  // Parte Destra
  boxFirst2.set_homogeneous(false);
  mainBox.pack_start(boxFirst2, Gtk::PACK_SHRINK, 1);

  labelFunzioneRes.set_xalign(0);
  labelFunzioneRes.set_margin_bottom(5);
  boxFirst2.pack_start(labelFunzioneRes, Gtk::PACK_SHRINK, 1);

  labelParametriInfo.set_xalign(0.5);
  boxFirst2.pack_start(labelParametriInfo, Gtk::PACK_SHRINK, 1);

  parametriScroll.set_size_request(400,150);
  parametriScroll.set_margin_bottom(5);
  boxFirst2.pack_start(parametriScroll, Gtk::PACK_SHRINK, 1);
  //modelloTreeView = Gtk::ListStore::create(modelloColonne);
  treeViewParametri.set_model(modelloTreeView);
  treeViewParametri.append_column("Parametro", modelloColonne.columnParName);
  treeViewParametri.append_column("Valore", modelloColonne.columnParValue);
  parametriScroll.add(treeViewParametri);

  boxFirst2.pack_start(labelGrafico, Gtk::PACK_SHRINK, 1);
  //boxFirst2.pack_start(grafico, Gtk::PACK_SHRINK, 1);

  bottoneSalva.set_margin_bottom(5);
  boxFirst2.pack_start(bottoneSalva, Gtk::PACK_SHRINK, 1);

  labelChiSq.set_xalign(0);
  labelChiSq.set_margin_bottom(5);
  boxFirst2.pack_start(labelChiSq,Gtk::PACK_SHRINK, 1);

  labelPValue.set_xalign(0);
  labelPValue.set_margin_bottom(5);
  boxFirst2.pack_start(labelPValue,Gtk::PACK_SHRINK, 1);

  labelDoF.set_xalign(0);
  boxFirst2.pack_start(labelDoF,Gtk::PACK_SHRINK, 1);

  show_all_children();
}

// DONE
void MyWindow::on_radioScelta_toggled(Glib::ustring tipo)
{
  if (tipo=="manuale" && radioManuale.get_active())
  {
    bottoneFile.set_sensitive(false);
    entryNumero.set_sensitive(true);
    bottoneManuale.set_sensitive(true);
  } else if (tipo=="file" && radioFile.get_active())
  {
    entryNumero.set_sensitive(false);
    bottoneFile.set_sensitive(true);
    bottoneManuale.set_sensitive(false);
  }
}

// DONE
void MyWindow::on_entryNumero_changed()
{
  if ((int) entryNumero.get_value() == manuali)
  {
    icona.set_from_icon_name("emblem-default-symbolic",Gtk::IconSize(2.));
  } else 
  {
    icona.set_from_icon_name("dialog-error-symbolic",Gtk::IconSize(2.));
  }
}

// DONE
void MyWindow::on_bottoneFile_clicked()
{
  bool funzionato = scegliFile();
  if (!funzionato) return;

  std::string nomeFile = pathFileCSV.filename().string();
  if (nomeFile.size()>19)
  {
    labelFile.set_text("File: "+nomeFile.substr(0,16)+"...");
  } else
  {
    labelFile.set_text("File: "+nomeFile);
  }
}

void MyWindow::on_bottoneManuale_clicked()
{
  int numeroDati = (int) entryNumero.get_value();
  DatiDialog dialogo(this, numeroDati);
  int risposta = dialogo.run();
  // if (risposta == Gtk.ResponseType.Ok)
  // {
  //   estraiDati(dialogo.listaX, dialogo.listaY, dialogo.listaErrori)
  // }
}

void MyWindow::on_bottoneFit_clicked()
{
  try {
    
  } catch(LunghezzaError erroreLunghezza) {
    lunghezzaErrorDialog();
  } catch(AnnullaError erroreAnnulla) {
  } catch(PochiDatiError errorePochiDati) {
    pochiDatiErrorDialog();
  } catch(ImportazioneError erroreImportazione) {
    importazioneErrorDialog();
  }
}

bool MyWindow::scegliFile()
{
  bool stato;
  FileDialog dialogo(this);
  int risposta = dialogo.run();
  switch (risposta)
  {
    case(Gtk::RESPONSE_OK):
      pathFileCSV.assign(dialogo.get_filename());
      stato = true;
      break;
    case(Gtk::RESPONSE_CANCEL):
      stato = false;
      break;
    default:
      stato = false;
      break;
  }
  return stato;
}


// Dialoghi errori
void MyWindow::pochiDatiErrorDialog()
{
  Gtk::MessageDialog dialog(*this, "Troppi pochi dati", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK);
  dialog.set_secondary_text("Si è cercato di effettuare un fit di un grado maggiore del numero di dati.\nQuesto non è possibile in quanto il numero minimo di vincoli richiesti corrisponde al grado del polinomio da fittare.");
  dialog.run();
}

void MyWindow::importazioneErrorDialog()
{
  Gtk::MessageDialog dialog(*this, "Errore nell'importare il file", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK);
  dialog.set_secondary_text("È stato riscontrato un errore nell'importazione del file.\nProbabilmente è corrotto oppure non è adatto ad un fit (ad esempio per un numero diverso di colonne).\nControllare il file e ritentare.");
  dialog.run();
}

void MyWindow::lunghezzaErrorDialog()
{
  Gtk::MessageDialog dialog(*this, "Numeri di dati diverse", false, Gtk::MESSAGE_WARNING, Gtk::BUTTONS_OK);
  dialog.set_secondary_text("È stato rilevato un numero diverso di dati lungo la x e lungo la y.\nControllare i dati e ritentare.");
  dialog.run();
}

void MyWindow::generateBlankPdf()
{
  TCanvas *canvasBlank = new TCanvas("BlankGraph", "BlankGraph", 800, 10, 600, 400);
  canvasBlank->SetFillColor(0);
  canvasBlank->SetGrid();
  canvasBlank->SaveAs(pathBlankPdf.c_str());
}