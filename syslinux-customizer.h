#include <gtk/gtk.h>
#include "syslinux-parser/syslinux-parser.h"
#define MAX_ENTRIES 65536

GtkWidget *window,*menubar,*vbox,*listbox,*menu_file,*menuitem_file,*submenuitem_reloadconfig,
*submenuitem_importconfig,*submenuitem_exportconfig,*submenuitem_quit,*menu_edit,*menuitem_edit,
*submenuitem_new,*submenuitem_importentry,*submenuitem_exportentry,*submenuitem_preferences,
*menu_help,*menuitem_help,*submenuitem_help,*submenuitem_about, *scrolledwindow, *dialog,

 *label_clabel, *label_cmenulabel, *label_ccommand, *label_cappend, *label_ccom32, *label_cinitrd,
	*entry_clabel, *entry_cmenulabel, *entry_ccommand, *entry_cappend, *entry_ccom32, *entry_cinitrd, *grid,
	*btn_cancel, *btn_save, *label_notice;

GPtrArray *program_icon_names;
GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;
gchar *home_dir, *syslinuxcfg, *syslinuxpath, *nsyslinuxpath, *selfcfg, *pver="0.93", *program_icon;
gchar config_file_path[256];
FILE *selffile;

gint parse_result,exportcontext,i, nohome;
struct SYSLINUX_ENTRY entries[MAX_ENTRIES];

#include "settings.h"
#include "entryeditor.h"
#include "events.h"
#include "importexport.h"
#include "mainwindow.h"
