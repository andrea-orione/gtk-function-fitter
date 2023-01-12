#ifndef GTKMM_FILEDIALOG_H
#define GTKMM_FILEDIALOG_H

#include <gtkmm.h>

class FileDialog : public Gtk::FileChooserDialog
{
public:
  FileDialog(Gtk::Window *);
};

#endif //GTKMM_FILEDIALOG_H