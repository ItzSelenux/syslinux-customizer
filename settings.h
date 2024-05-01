GtkWidget *label_default, *label_prompt, *label_timeout, *label_ui, *combobox_default,
*checkbutton_prompt, *entry_timeout, *combobox_ui, *label_syslinuxpath, *entry_syslinuxpath,
*box,*filechooser_syslinuxpath, *btn_savesettings;

void on_btn_savesettings_clicked()
{
	const gchar *ndefault = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_default));
	gboolean getprompt = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_prompt));
		const gchar *nprompt = getprompt ? "1" : "0";
	const gchar *ntimeout = gtk_entry_get_text(GTK_ENTRY(entry_timeout));
	const gchar *nui =gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_ui));
	const gchar *nsyslinux = gtk_entry_get_text(GTK_ENTRY(entry_syslinuxpath));

	strcpy(SYSLINUX_CONF_DEFAULT, ndefault );
	strcpy(SYSLINUX_CONF_PROMPT, nprompt);
	strcpy(SYSLINUX_CONF_TIMEOUT, ntimeout );
	strcpy(SYSLINUX_CONF_UI, nui);

	saveconfig(0);
	reset(window);
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void defcombo (GtkComboBox *combo, const gchar *value)
{
	int default_index = -1;
	GtkTreeModel *model = gtk_combo_box_get_model(combo);
	int num_elements = gtk_tree_model_iter_n_children(model, NULL);
	
	for (int i = 0; i < num_elements; i++)
	{
		GtkTreeIter iter;
		gtk_tree_model_iter_nth_child(model, &iter, NULL, i);
		
		gchar *text;
		gtk_tree_model_get(model, &iter, 0, &text, -1);
		
		if (strcmp(value, text) == 0)
		{
			default_index = i;
			g_free(text);
			break;
		}
		g_free(text);
	}
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), default_index);
}

void on_preferences(GtkWidget *button, gpointer data)
{
	gtk_init(NULL, NULL);
		dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(dialog), "Syslinux preferences");
		g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);

		grid = gtk_grid_new();
		gtk_container_add(GTK_CONTAINER(dialog), grid);
		gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
		gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
		
		label_default = gtk_label_new("Default entry:");
			combobox_default = gtk_combo_box_text_new();
		label_prompt = gtk_label_new("Show Prompt:");
			checkbutton_prompt = gtk_check_button_new();
		label_timeout = gtk_label_new("Timeout (seconds):");
			entry_timeout = gtk_entry_new();
		label_ui = gtk_label_new("UI Type:");
			combobox_ui = gtk_combo_box_text_new();
		label_syslinuxpath = gtk_label_new("Syslinux.cfg path:"); 
			box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
			gtk_box_set_homogeneous(GTK_BOX(box), TRUE);
			entry_syslinuxpath = gtk_entry_new();
			filechooser_syslinuxpath = gtk_file_chooser_button_new("select file", GTK_FILE_CHOOSER_ACTION_OPEN);
			gtk_container_add(GTK_CONTAINER(box), entry_syslinuxpath);
			//gtk_container_add(GTK_CONTAINER(box), filechooser_syslinuxpath);
		btn_cancel = gtk_button_new_with_label("cancel");
		btn_savesettings = gtk_button_new_with_label("save");

		gtk_grid_attach(GTK_GRID(grid), label_default, 0, 0, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), combobox_default, 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_timeout, 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_timeout, 1, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_prompt, 0, 2, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), checkbutton_prompt, 1, 2, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_ui, 0, 3, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), combobox_ui, 1, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_syslinuxpath, 0, 4, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), box, 1, 4, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), btn_cancel, 0, 6, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), btn_savesettings, 1, 6, 1, 1);

		for (int i = 0; i < entry_count; i++)
		{
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_default), entries[i].label);
		}
		defcombo (GTK_COMBO_BOX(combobox_default), SYSLINUX_CONF_DEFAULT);

		gtk_entry_set_text(GTK_ENTRY(entry_timeout), SYSLINUX_CONF_TIMEOUT);

		char menupath[256], vesamenupath[256];
			sprintf(menupath, "%s/menu.c32", syslinuxpath);
			sprintf(vesamenupath, "%s/vesamenu.c32", syslinuxpath);
		if (access(menupath, F_OK) != -1)
		{
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_ui), "menu.c32");
		}
		if (access(vesamenupath, F_OK) != -1)
		{
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_ui), "vesamenu.c32");
		}
		defcombo (GTK_COMBO_BOX(combobox_ui), SYSLINUX_CONF_UI);

		int intconf_prompt = atoi(SYSLINUX_CONF_PROMPT);
		if (!intconf_prompt)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_prompt), FALSE);
		}
		else 
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(checkbutton_prompt), TRUE);
		}

		gtk_entry_set_text(GTK_ENTRY(entry_syslinuxpath), syslinuxpath);

		g_signal_connect(btn_cancel, "clicked", G_CALLBACK(on_btn_cancel_clicked), dialog);
		g_signal_connect(btn_savesettings, "clicked", G_CALLBACK(on_btn_savesettings_clicked), dialog);
		gtk_widget_show_all(dialog);
		gtk_main();
}