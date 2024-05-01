#include <gtk/gtk.h>
#include "syslinux-parser/syslinux-parser.h"
#define MAX_ENTRIES 65536

GtkWidget *window,*menubar,*vbox,*listbox,*menu_file,*menuitem_file,*submenuitem_reloadconfig,
*submenuitem_importconfig,*submenuitem_exportconfig,*submenuitem_quit,*menu_edit,*menuitem_edit,
*submenuitem_new,*submenuitem_importentry,*submenuitem_exportentry,*submenuitem_preferences,
*menu_help,*menuitem_help,*submenuitem_help,*submenuitem_about, *scrolledwindow, *dialog;

char *syslinuxcfg, *syslinuxpath, *selfcfg, *pver="0.91";
FILE *selffile;

int parse_result,exportcontext,i;
struct SYSLINUX_ENTRY entries[MAX_ENTRIES];

void reset(GtkWidget *widget);
void on_preferences(GtkWidget *button, gpointer data);

void saveconfig(int exporting)
{
	FILE *output;
	output = fopen(syslinuxcfg, "w");
	if (output == NULL)
	{
		g_printerr("\033[1;31mERROR\033[0m: Process running but config does not exist, did you moved the file?\n");
		gtk_main_quit();
	}
	else
	{
		unsigned int n=1;
		if (exporting)
		{
			i = exportcontext;n=0;
		}
		else if (!exporting)
		{
		exportcontext = 0;
		}
			if (!exporting)
			{
				fprintf(output, "# CFG File generated with Syslinux Customizer\n", SYSLINUX_CONF_DEFAULT);
				if (strlen(SYSLINUX_CONF_DEFAULT) > 0)
				{
					fprintf(output, "DEFAULT %s\n", SYSLINUX_CONF_DEFAULT);
				}
				else
				{
					fprintf(output, "DEFAULT system\n");
				}
				if (strlen(SYSLINUX_CONF_PROMPT) > 0)
				{
					fprintf(output, "PROMPT %s\n", SYSLINUX_CONF_PROMPT);
				}
				else
				{
					fprintf(output, "PROMPT 0\n");
				}
				if (strlen(SYSLINUX_CONF_TIMEOUT) > 0)
				{
					fprintf(output, "TIMEOUT %s\n", SYSLINUX_CONF_TIMEOUT);
				}
				else
				{
					fprintf(output, "TIMEOUT 50\n");
				}
				if (strlen(SYSLINUX_CONF_UI) > 0)
				{
					fprintf(output, "UI %s\n", SYSLINUX_CONF_UI);
				}
				else
				{
					fprintf(output, "UI menu.c32\n");
				}
			}
			fprintf(output, "\n");

		for (i = exportcontext; i < entry_count; i++)
		{
			if (exporting)
			{
				i = exportcontext;
			}

			if (strlen(entries[i].label) > 0)
			{
				fprintf(output, "LABEL %s\n", entries[i].label);
			}
			if (strlen(entries[i].menulabel) > 0)
			{
				fprintf(output, "MENU LABEL %s\n", entries[i].menulabel);
			}
			if (strlen(entries[i].command) > 0)
			{
				fprintf(output, "LINUX %s\n", entries[i].command);
			}
			if (strlen(entries[i].append) > 0)
			{
				fprintf(output, "APPEND %s\n", entries[i].append);
			}
			if (strlen(entries[i].com32) > 0)
			{
				fprintf(output, "COM32 %s\n", entries[i].com32);
			}
			if (strlen(entries[i].initrd) > 0)
			{
				fprintf(output, "INITRD %s\n", entries[i].initrd);
			}
			fprintf(output, "\n");

			if (exporting)
			{
				entry_count = n;
			}
		}
	}
	fclose(output);
}

void on_newentry()
{
	strcpy(entries[entry_count].label, "newentry");
	strcpy(entries[entry_count].menulabel, "New Syslinux entry");
	strcpy(entries[entry_count].command, "");
	strcpy(entries[entry_count].append, "");
	strcpy(entries[entry_count].com32, "");
	strcpy(entries[entry_count].initrd, "");
	entry_count++;
	saveconfig(0);
	reset(window);
}

void getconf() {
	FILE *selffile = fopen("/etc/syslinux-customizer.cfg", "r");
	if (selffile != NULL) {
		fseek(selffile, 0, SEEK_END);
		long fsize = ftell(selffile);
		fseek(selffile, 0, SEEK_SET);

		char *temp_syslinuxpath = malloc(fsize + 1);
		if (temp_syslinuxpath == NULL) {
			fclose(selffile);
			printf("Error: Malloc failed\n");
			return;
		}

		size_t bytesRead = fread(temp_syslinuxpath, 1, fsize, selffile);
		if (bytesRead != fsize) {
			fclose(selffile);
			free(temp_syslinuxpath);
			printf("Error: File read error\n");
			return;
		}
		fclose(selffile);

		for (long i = fsize - 1; i >= 0; i--) {
			if (temp_syslinuxpath[i] == '\n' || temp_syslinuxpath[i] == '\r') {
				temp_syslinuxpath[i] = '\0';
				break;
			}
		}

		syslinuxpath = malloc(strlen(temp_syslinuxpath) + 1); // Asignación global
		if (syslinuxpath == NULL) {
			free(temp_syslinuxpath);
			printf("Error: Malloc failed\n");
			return;
		}
		strcpy(syslinuxpath, temp_syslinuxpath);
		free(temp_syslinuxpath);

		syslinuxcfg = malloc(strlen(syslinuxpath) + 14);
		if (syslinuxcfg == NULL) {
			free(syslinuxpath);
			printf("Error: Malloc failed\n");
			return;
		}
		snprintf(syslinuxcfg, strlen(syslinuxpath) + 14, "%s/syslinux.cfg", syslinuxpath);
	} else {
		syslinuxpath = "/boot/syslinux/";
		syslinuxcfg = "/boot/syslinux/syslinux.cfg";
	}
	g_print("custom syslinux.cfg: %s", syslinuxcfg);
}


void CleanEntry(char *a, char b)
{
	char *ptr = a;
	while ((ptr = strchr(ptr, b)) != NULL)
	{
		memmove(ptr, ptr + 1, strlen(ptr));
	}
}

void on_about(GtkMenuItem *menuitem, gpointer userdata)
{
	dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Syslinux-customizer");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2024 ItzSelenux");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Syslinux config editor");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), pver);
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/syslinux-customizer");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"kmenuedit");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}