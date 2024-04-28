char *on_select_file(const char *filetype, const char *title, int save_dialog)
{
	GtkWidget *dialog;
	GtkFileChooserAction action = save_dialog ? GTK_FILE_CHOOSER_ACTION_SAVE : GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new(title,
		NULL,
		action,
		"_Cancel",
		GTK_RESPONSE_CANCEL,
		save_dialog ? "_Save" : "_Open",
		GTK_RESPONSE_ACCEPT,
		NULL);

	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter, filetype);
	gtk_file_filter_set_name(filter, "SysLinux Customizer Entry (*.sce)");
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

	res = gtk_dialog_run(GTK_DIALOG(dialog));
	char *filename = NULL;

	if (res == GTK_RESPONSE_ACCEPT)
	{
		GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
		if (save_dialog)
		{
			filename = gtk_file_chooser_get_filename(chooser);
		}
		else
		{
			filename = g_strdup(gtk_file_chooser_get_filename(chooser));
		}
	}
	gtk_widget_destroy(dialog);

	return filename;
}

void on_importentry()
{
	char *filename = on_select_file("*.sce", "Select a SysLinux Customizer Entry (.sce)", 0);
	if (!filename)
	{
		g_printerr("\033[1;31mERROR\033[0m: Selected file is NULL\n");
		reset(window);
		return;
	}

	FILE *sourceFile = fopen(filename, "r");
	g_free(filename);
	if (!sourceFile)
	{
		g_printerr("\033[1;31mERROR\033[0m: Unable to open selected file for reading\n");
		return;
	}

	FILE *destinationFile = fopen(syslinuxcfg, "a");
	if (!destinationFile)
	{
		fclose(sourceFile);
		g_printerr("\033[1;31mERROR\033[0m: Unable to open destination file for appending\n");
		return;
	}

	char buffer[1024];
	size_t bytesRead;
	while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0)
	{
		fwrite(buffer, 1, bytesRead, destinationFile);
	}

	fclose(sourceFile);
	fclose(destinationFile);
}

void on_exportentry()
{
	char *filename = on_select_file("*.sce", "Select a SysLinux Customizer Entry (.sce)", 1);
	if (!filename)
	{
		g_printerr("\033[1;31mERROR\033[0m: Selected file is NULL\n");
		reset(window);
		return;
	}

	GtkWidget *focused_widget = gtk_window_get_focus(GTK_WINDOW(window));
	if (focused_widget != NULL)
	{
		syslinuxcfg = filename;
		const gchar *widget_name = gtk_widget_get_name(focused_widget);
		exportcontext = atoi(widget_name);
		saveconfig(1);
		reset(window);
	}
	else
	{
		g_print("No hay ningún widget con foco en la ventana.\n");
	}
}

void on_importexport(GtkMenuItem *menu_item, gpointer user_data)
{
	int exporting = GPOINTER_TO_INT(user_data);
	g_print("%d", exporting);
	char *filename = on_select_file("*.cfg", "Select a SysLinux Config file (.cfg)", exporting);
	if (!filename)
	{
		g_printerr("\033[1;31mERROR\033[0m: Selected file is NULL\n");
		reset(window);
		return;
	}

	if (exporting == 1)
	{
		FILE *sourceFile = fopen(syslinuxcfg, "r");
		FILE *destinationFile = fopen(filename, "w");

		if (sourceFile == NULL || destinationFile == NULL)
		{
			printf("Error opening files.\n");
			return;
		}

		char buffer[MAX_ENTRIES];
		size_t bytesRead;

		while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0)
		{
			fwrite(buffer, 1, bytesRead, destinationFile);
		}

		fclose(sourceFile);
		fclose(destinationFile);

		printf("Data copied from %s to %s.\n", syslinuxcfg, filename);
	}
	else if (exporting == 0)
	{
		GtkWidget *warning = gtk_message_dialog_new(GTK_WINDOW(window),
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_WARNING,
			GTK_BUTTONS_OK_CANCEL,
			"Are you sure you want to proceed with data copying? The current configuration will be overwritten.");

		gint response = gtk_dialog_run(GTK_DIALOG(warning));

		if (response == GTK_RESPONSE_OK)
		{
			FILE *sourceFile = fopen(filename, "r");
			FILE *destinationFile = fopen(syslinuxcfg, "w");
			
			if (sourceFile == NULL || destinationFile == NULL)
			{
				printf("Error opening files.\n");
				return;
			}

			char buffer[MAX_ENTRIES];
			size_t bytesRead;

			while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0)
			{
				fwrite(buffer, 1, bytesRead, destinationFile);
			}

			fclose(sourceFile);
			fclose(destinationFile);

			printf("Data copied from %s to %s.\n", filename, syslinuxcfg);
		}
		gtk_widget_destroy(warning);
	}
}