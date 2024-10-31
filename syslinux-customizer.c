#include "syslinux-customizer.h"

int main(int argc, char *argv[])
{
	home_dir = getenv("HOME");
	getconf();
	parse_syslinux_config(syslinuxcfg, entries);
	gtk_init(&argc, &argv);
	windowinit();
	gtk_widget_show_all(window);
	gtk_main();
}
