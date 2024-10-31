gchar* probe_icons_from_theme(GPtrArray *icon_names)
{
	theme = gtk_icon_theme_get_default();
	gint i;

	for (i = 0; i < icon_names->len; i++)
	{
		gchar *icon_name = g_ptr_array_index(icon_names, i);
		info = gtk_icon_theme_lookup_icon(theme, icon_name, 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			if (icon != NULL)
			{
				g_object_unref(icon);
				g_object_unref(info);
				return g_strdup(icon_name);
			}
			g_object_unref(info);
		}
	}
	return g_strdup("\0");
}

void window_set_icon(GtkWindow *target, gchar *iconname)
{
	info = gtk_icon_theme_lookup_icon(theme, iconname, 16, 16);
		icon = gtk_icon_info_load_icon(info, NULL);
		gtk_window_set_icon(target, icon);
		g_object_unref(icon);
		g_object_unref(info);
}

void on_newentry(void)
{
	strcpy(entries[entry_count].label, "newentry");
	strcpy(entries[entry_count].menulabel, "New Syslinux entry");
	strcpy(entries[entry_count].command, "");
	strcpy(entries[entry_count].append, "");
	strcpy(entries[entry_count].com32, "");
	strcpy(entries[entry_count].initrd, "");
	entry_count++;
	saveconfig(0);
	reset(window, 1);
}

void show_error_dialog(const gchar *message)
{
	dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", message);
	gtk_window_set_title(GTK_WINDOW(dialog), "ERROR");
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}
