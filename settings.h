GtkWidget *label_default, *label_prompt, *label_timeout, *label_ui, *label_title, *label_syslinuxpath,
*combobox_default,*combobox_ui,
*checkbutton_prompt,
*entry_timeout, *entry_syslinuxpath, *entry_title,
*box,*filechooser_syslinuxpath, *btn_savesettings,

*label_color_screen, *label_color_border, *label_color_title, *label_color_sel, *label_color_unsel,
*label_color_help, *label_color_timeout_msg, *label_color_timeout, *label_color_msg07,

*label_element, *label_oldmenu, *label_vesamenu, *label_about,
*entry_color_screen0, *entry_color_screen1, *entry_color_screen2,
*entry_color_border0, *entry_color_border1, *entry_color_border2,
*entry_color_title0, *entry_color_title1, *entry_color_title2,
*entry_color_sel0, *entry_color_sel1, *entry_color_sel2,
*entry_color_unsel0, *entry_color_unsel1, *entry_color_unsel2,
*entry_color_help0, *entry_color_help1, *entry_color_help2,
*entry_color_tabmsg0, *entry_color_tabmsg1, *entry_color_tabmsg2,
*entry_color_screen0, *entry_color_screen1, *entry_color_tabmsg2,
*entry_color_timeout_msg0, *entry_color_timeout_msg1, *entry_color_timeout_msg2,
*entry_color_timeout0, *entry_color_timeout1, *entry_color_timeout2,


*notebook, *colorsgrid;

void on_btn_savesettings_clicked()
{
	const gchar *ndefault = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_default));
	gboolean getprompt = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_prompt));
		const gchar *nprompt = getprompt ? "1" : "0";
	const gchar *ntimeout = gtk_entry_get_text(GTK_ENTRY(entry_timeout));
	const gchar *nui =gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_ui));
	const gchar *nsyslinux = gtk_entry_get_text(GTK_ENTRY(entry_syslinuxpath));
	const gchar *ntitle = gtk_entry_get_text(GTK_ENTRY(entry_title));

	strcpy(SYSLINUX_CONF_DEFAULT, ndefault );
	strcpy(SYSLINUX_CONF_PROMPT, nprompt);
	strcpy(SYSLINUX_CONF_TIMEOUT, ntimeout );
	strcpy(SYSLINUX_CONF_UI, nui);
	strcpy(SYSLINUX_CONF_TITLE, ntitle);

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

		notebook = gtk_notebook_new();
			grid = gtk_grid_new();
			colorsgrid = gtk_grid_new();

			gtk_notebook_append_page(GTK_NOTEBOOK(notebook), GTK_WIDGET(grid), gtk_label_new("Settings"));
			gtk_notebook_append_page(GTK_NOTEBOOK(notebook), GTK_WIDGET(colorsgrid), gtk_label_new("Colors"));
		gtk_container_add(GTK_CONTAINER(dialog), notebook);
		gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
		gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
		gtk_grid_set_column_homogeneous(GTK_GRID(colorsgrid), TRUE);
		//gtk_grid_set_row_homogeneous(GTK_GRID(colorsgrid), TRUE);

		// MAIN_GRID
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
		label_title = gtk_label_new("Title");
			entry_title = gtk_entry_new();
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
		gtk_grid_attach(GTK_GRID(grid), label_title, 0, 4, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), entry_title, 1, 4, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), label_syslinuxpath, 0, 5, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), box, 1, 5, 1, 1);
		gtk_grid_attach(GTK_GRID(grid), btn_cancel, 0, 6, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), btn_savesettings, 1, 6, 1, 1);

		// COLORS GRID
		entry_color_screen0 = gtk_entry_new();
		entry_color_border0 = gtk_entry_new();
		entry_color_title0 = gtk_entry_new();
		entry_color_sel0 = gtk_entry_new();
		entry_color_unsel0 = gtk_entry_new();
		entry_color_help0 = gtk_entry_new();
		entry_color_timeout_msg0 = gtk_entry_new();
		entry_color_timeout0 = gtk_entry_new();
		entry_color_tabmsg0 = gtk_entry_new();

		entry_color_screen1 = gtk_entry_new();
		entry_color_border1 = gtk_entry_new();
		entry_color_title1 = gtk_entry_new();
		entry_color_sel1 = gtk_entry_new();
		entry_color_unsel1 = gtk_entry_new();
		entry_color_help1 = gtk_entry_new();
		entry_color_timeout_msg1 = gtk_entry_new();
		entry_color_timeout1 = gtk_entry_new();
		entry_color_tabmsg1 = gtk_entry_new();

		entry_color_screen2 = gtk_entry_new();
		entry_color_border2 = gtk_entry_new();
		entry_color_title2 = gtk_entry_new();
		entry_color_sel2 = gtk_entry_new();
		entry_color_unsel2 = gtk_entry_new();
		entry_color_help2 = gtk_entry_new();
		entry_color_timeout_msg2 = gtk_entry_new();
		entry_color_timeout2 = gtk_entry_new();
		entry_color_tabmsg2 = gtk_entry_new();

		label_about = gtk_label_new(NULL);
			gtk_label_set_markup(GTK_LABEL(label_about), "For more information check <a href=\"https://wiki.syslinux.org/wiki/index.php?title=Menu#MENU_COLOR\">Syslinux documentation</a>");

		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Element"), 0, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Color (Old menu)"), 1, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Color (Vesamenu)"), 2, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Type"), 3, 0, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Screen"), 0, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_screen0, 1, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_screen1, 2, 1, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_screen2, 3, 1, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Border"), 0, 2, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_border0, 1, 2, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_border1, 2, 2, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_border2, 3, 2, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Title"), 0, 3, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_title0, 1, 3, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_title1, 2, 3, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_title2, 3, 3, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Selected"), 0, 4, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_sel0, 1, 4, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_sel1, 2, 4, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_sel2, 3, 4, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Not selected"), 0, 5, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_unsel0, 1, 5, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_unsel1, 2, 5, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_unsel2, 3, 5, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Help"), 0, 6, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_help0, 1, 6, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_help1, 2, 6, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_help2, 3, 6, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Timeout msg"), 0, 7, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_timeout_msg0, 1, 7, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_timeout_msg1, 2, 7, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_timeout_msg2, 3, 7, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Timeout"), 0, 8, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_timeout0, 1, 8, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_timeout1, 2, 8, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_timeout2, 3, 8, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Tab msg"), 0, 9, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_tabmsg0, 1, 9, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_tabmsg1, 2, 9, 1, 1);
			gtk_grid_attach(GTK_GRID(colorsgrid), entry_color_tabmsg2, 3, 9, 1, 1);
		gtk_grid_attach(GTK_GRID(colorsgrid), label_about, 0, 10, 4, 1);


		for (int i = 0; i < entry_count; i++)
		{
			gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combobox_default), entries[i].label);
		}
		defcombo (GTK_COMBO_BOX(combobox_default), SYSLINUX_CONF_DEFAULT);

		gtk_entry_set_text(GTK_ENTRY(entry_timeout), SYSLINUX_CONF_TIMEOUT);
		gtk_entry_set_text(GTK_ENTRY(entry_title), SYSLINUX_CONF_TITLE);

		gtk_entry_set_text(GTK_ENTRY(entry_color_border0), SYSLINUX_COLOR_BORDER_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_border1), SYSLINUX_COLOR_BORDER_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_border2), SYSLINUX_COLOR_BORDER_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_screen0), SYSLINUX_COLOR_SCREEN);
		gtk_entry_set_text(GTK_ENTRY(entry_color_title0), SYSLINUX_COLOR_TITLE);
		gtk_entry_set_text(GTK_ENTRY(entry_color_sel0), SYSLINUX_COLOR_SEL);
		gtk_entry_set_text(GTK_ENTRY(entry_color_unsel0), SYSLINUX_COLOR_UNSEL);
		gtk_entry_set_text(GTK_ENTRY(entry_color_help0), SYSLINUX_COLOR_HELP);
		gtk_entry_set_text(GTK_ENTRY(entry_color_timeout_msg0), SYSLINUX_COLOR_TIMEOUT_MSG);
		gtk_entry_set_text(GTK_ENTRY(entry_color_timeout0), SYSLINUX_COLOR_TIMEOUT);
		gtk_entry_set_text(GTK_ENTRY(entry_color_tabmsg0), SYSLINUX_COLOR_TABMSG);

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