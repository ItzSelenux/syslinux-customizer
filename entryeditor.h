void on_btn_save_clicked(void)
{
	const gchar *nlabel = gtk_entry_get_text(GTK_ENTRY(entry_clabel));
	const gchar *nmenulabel = gtk_entry_get_text(GTK_ENTRY(entry_cmenulabel));
	const gchar *ncommand = gtk_entry_get_text(GTK_ENTRY(entry_ccommand));
	const gchar *nappend = gtk_entry_get_text(GTK_ENTRY(entry_cappend));
	const gchar *ninitrd = gtk_entry_get_text(GTK_ENTRY(entry_cinitrd));
	const gchar *ncom32 = gtk_entry_get_text(GTK_ENTRY(entry_ccom32));

	strcpy(entries[i].label, nlabel);
	strcpy(entries[i].menulabel, nmenulabel);
	strcpy(entries[i].command, ncommand);
	strcpy(entries[i].append, nappend);
	strcpy(entries[i].initrd, ninitrd);
	strcpy(entries[i].com32, ncom32);

	saveconfig(0);
	reset(window, 1);
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void CleanEntry(char *a, char b)
{
	char *ptr = a;
	while ((ptr = strchr(ptr, b)) != NULL)
	{
		memmove(ptr, ptr + 1, strlen(ptr));
	}
}

void on_edit_button_clicked(GtkWidget *button, gpointer data)
{
	char clabel[ML],cmenulabel[ML],ccommand[ML],
		cappend[ML],ccom32[ML],cinitrd[ML];
	const gchar *widget_name = gtk_widget_get_name(button);

	sscanf(widget_name, "|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|",
		&i, clabel, cmenulabel, ccommand, cappend, ccom32, cinitrd);

	CleanEntry(clabel, '!');CleanEntry(cmenulabel, '!');CleanEntry(ccommand, '!');
	CleanEntry(cappend, '!');CleanEntry(ccom32, '!');CleanEntry(cinitrd, '!');

	g_print("current entry: |%d|%s|%s|%s|%s|%s|%s|\n",
		i, clabel, cmenulabel, ccommand, cappend, ccom32, cinitrd);
		
	// EDIT WINDOW
	gtk_init(NULL, NULL);
		dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		char title[ML*2];
		sprintf(title, "%s - Syslinux Customizer", cmenulabel);
		gtk_window_set_title(GTK_WINDOW(dialog), title);
		g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);

		grid = gtk_grid_new();
		gtk_container_add(GTK_CONTAINER(dialog), grid);
		gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

		label_notice = gtk_label_new("if you don't need a value, you can left it empty");
		label_clabel = gtk_label_new("Label:");
			entry_clabel = gtk_entry_new();
		label_cmenulabel = gtk_label_new("Menu Label:");
			entry_cmenulabel = gtk_entry_new();
		label_ccommand = gtk_label_new("Command:");
			entry_ccommand = gtk_entry_new();
		label_cappend = gtk_label_new("Append:");
			entry_cappend = gtk_entry_new();
		label_cinitrd = gtk_label_new("Initrd:");
			entry_cinitrd = gtk_entry_new();
		label_ccom32 = gtk_label_new("COM32:");
			entry_ccom32 = gtk_entry_new();
		btn_cancel = gtk_button_new_with_label("cancel");
		btn_save = gtk_button_new_with_label("save");
		gtk_widget_set_halign(label_notice, GTK_ALIGN_CENTER);

		gtk_grid_attach(GTK_GRID(grid), label_clabel, 0, 0, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_clabel, 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_cmenulabel, 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_cmenulabel, 1, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_ccommand, 0, 2, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_ccommand, 1, 2, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_cappend, 0, 3, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_cappend, 1, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_cinitrd, 0, 4, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_cinitrd, 1, 4, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_ccom32, 0, 5, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_ccom32, 1, 5, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), btn_cancel, 0, 6, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), btn_save, 1, 6, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_notice, 0, 7, 2, 1);

		gtk_entry_set_text(GTK_ENTRY(entry_clabel), clabel);
		gtk_entry_set_text(GTK_ENTRY(entry_cmenulabel), cmenulabel);
		gtk_entry_set_text(GTK_ENTRY(entry_ccommand), ccommand);
		gtk_entry_set_text(GTK_ENTRY(entry_cappend), cappend);
		gtk_entry_set_text(GTK_ENTRY(entry_cinitrd), cinitrd);
		gtk_entry_set_text(GTK_ENTRY(entry_ccom32), ccom32);

		g_signal_connect(btn_cancel, "clicked", G_CALLBACK(on_btn_cancel_clicked), dialog);
		g_signal_connect(btn_save, "clicked", G_CALLBACK(on_btn_save_clicked), dialog);
		gtk_widget_show_all(dialog);
		gtk_main();
}
