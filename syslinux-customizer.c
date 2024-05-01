#include "syslinux-customizer.h"
#include "importexport.h"
#include "entryeditor.h"
#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
	getconf();
	parse_syslinux_config(syslinuxcfg, entries);
	gtk_init(&argc, &argv);
	windowinit();
	gtk_widget_show_all(window);
	gtk_main();
}