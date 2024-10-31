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

void on_preferences(GtkWidget *button, gpointer data);
void reset(GtkWidget *widget, gint init);
void show_error_dialog(const gchar *message);
void window_set_icon(GtkWindow *target, gchar *iconname);

void on_btn_cancel_clicked(GtkWidget *widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(data));
}

void getconf(void)
{
	gchar *user_config_file = NULL;
	gchar *default_path = "/boot/syslinux/";
	gchar *default_config = "/boot/syslinux/syslinux.cfg";
	FILE *config_file = NULL;
	long fsize;
	gchar *temp_syslinuxpath;

	if (syslinuxcfg != NULL)
	{
		return;
	}

	if (home_dir == NULL)
	{
		g_warning("No $HOME available");
	}
	else
	{
		snprintf(config_file_path, sizeof(config_file_path), "%s/.config/syslinux-customizer.cfg", home_dir);
		config_file = fopen(config_file_path, "r");
	}

	if (config_file == NULL)
	{
		config_file = fopen("/etc/syslinux-customizer.cfg", "r");
	}

	if (config_file == NULL)
	{
		syslinuxpath = strdup(default_path);
		syslinuxcfg = strdup(default_config);
		g_print("Using default syslinux.cfg: %s\n", syslinuxcfg);
		return;
	}

	fseek(config_file, 0, SEEK_END);
	fsize = ftell(config_file);
	fseek(config_file, 0, SEEK_SET);

	temp_syslinuxpath = malloc(fsize + 1);
	if (temp_syslinuxpath == NULL)
	{
		fclose(config_file);
		g_warning("Error: Malloc failed");
		return;
	}

	size_t bytesRead = fread(temp_syslinuxpath, 1, fsize, config_file);
	if (bytesRead != fsize)
	{
		fclose(config_file);
		free(temp_syslinuxpath);
		g_warning("Error: File read error");
		return;
	}
	fclose(config_file);

	for (long i = fsize - 1; i >= 0; i--)
	{
		if (temp_syslinuxpath[i] == '\n' || temp_syslinuxpath[i] == '\r')
		{
			temp_syslinuxpath[i] = '\0';
			break;
		}
	}

	FILE *test_file = fopen(temp_syslinuxpath, "r");
	if (test_file == NULL)
	{
		free(temp_syslinuxpath);
		syslinuxpath = strdup(default_path);
		syslinuxcfg = strdup(default_config);
		g_warning("invalid PATH in configuration, using default value");
		g_print("Using default syslinux.cfg: %s\n", syslinuxcfg);
		return;
	}
	fclose(test_file);

	syslinuxpath = strdup(temp_syslinuxpath);
	free(temp_syslinuxpath);

	syslinuxcfg = malloc(strlen(syslinuxpath) + 14);
	if (syslinuxcfg == NULL)
	{
		free(syslinuxpath);
		g_warning("Error: Malloc failed");
		return;
	}
	snprintf(syslinuxcfg, strlen(syslinuxpath) + 14, "%s/syslinux.cfg", syslinuxpath);

	g_print("Custom syslinux.cfg: %s\n", syslinuxcfg);
}

void saveconfig(int exporting)
{
	gchar *temp_syslinuxpath;

	if (!nohome)
	{ 
		if (nsyslinuxpath != NULL && strlen(nsyslinuxpath) > 0)
		{
			FILE *appdata = fopen(config_file_path, "w");
			if (appdata != NULL)
			{
				fprintf(appdata, "%s\n", nsyslinuxpath);
				fclose(appdata);
			}
			else
			{
				g_warning("Error: Could not open config file for writing: %s\n", config_file_path);
			}
		}
	}

	FILE *output = fopen(syslinuxcfg, "w");
	if (output == NULL)
	{
		gchar error_message[256];
		snprintf(error_message, sizeof(error_message), "Can't write to %s", syslinuxcfg);
		show_error_dialog(error_message);
		return;
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
				fprintf(output, "# CFG File generated with Syslinux Customizer\n");
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
				if (strlen(SYSLINUX_CONF_TITLE) > 0)
				{
					fprintf(output, "MENU TITLE %s\n\n", SYSLINUX_CONF_TITLE);
				}
				else
				{
					fprintf(output, "MENU TITLE Choose a system\n\n");
				}
				// Colors
				if (strlen(SYSLINUX_COLOR_SCREEN) > 0)
				{
					fprintf(output, "MENU COLOR screen %s %s %s \n",
						SYSLINUX_COLOR_SCREEN_OLDMENU, SYSLINUX_COLOR_SCREEN_VESAMENU, SYSLINUX_COLOR_SCREEN_SHADOW);
				}
				else
				{
					fprintf(output, "MENU COLOR screen 30;30 #a0000000 #a0000000 all\n");
				}
				if (strlen(SYSLINUX_COLOR_BORDER) > 0)
				{
					fprintf(output, "MENU COLOR border %s %s %s\n", SYSLINUX_COLOR_BORDER_OLDMENU, SYSLINUX_COLOR_BORDER_VESAMENU, SYSLINUX_COLOR_BORDER_SHADOW);
				}
				else
				{
					fprintf(output, "MENU COLOR border 30;44 #40000000 #00000000 std\n");
				}

				if (strlen(SYSLINUX_COLOR_TITLE) > 0)
				{
					fprintf(output, "MENU COLOR title %s %s %s\n", SYSLINUX_COLOR_TITLE_OLDMENU, SYSLINUX_COLOR_TITLE_VESAMENU, SYSLINUX_COLOR_TITLE_SHADOW);
				}
				else
				{
					fprintf(output, "MENU COLOR title 1;36;44 #c00090f0 #00000000 std\n");
				}

				if (strlen(SYSLINUX_COLOR_SEL) > 0)
				{
					fprintf(output, "MENU COLOR sel %s %s %s\n", SYSLINUX_COLOR_SEL_OLDMENU, SYSLINUX_COLOR_SEL_VESAMENU, SYSLINUX_COLOR_SEL_SHADOW);
				}
				else
				{
					fprintf(output, "MENU COLOR sel 7;37;40 #e0000000 #20ff8000 all\n");
				}

				if (strlen(SYSLINUX_COLOR_UNSEL) > 0) {
					fprintf(output, "MENU COLOR unsel %s %s %s\n", SYSLINUX_COLOR_UNSEL_OLDMENU, SYSLINUX_COLOR_UNSEL_VESAMENU, SYSLINUX_COLOR_UNSEL_SHADOW);
				} else {
					fprintf(output, "MENU COLOR unsel 37;44 #90ffffff #00000000 std\n");
				}

				if (strlen(SYSLINUX_COLOR_HELP) > 0)
				{
					fprintf(output, "MENU COLOR help %s %s %s\n", SYSLINUX_COLOR_HELP_OLDMENU, SYSLINUX_COLOR_HELP_VESAMENU, SYSLINUX_COLOR_HELP_SHADOW);
				}
				else 
				{
					fprintf(output, "MENU COLOR help 37;40 #c0ffffff #00000000 std\n");
				}

				if (strlen(SYSLINUX_COLOR_TIMEOUT_MSG) > 0)
				{
					fprintf(output, "MENU COLOR timeout_msg %s %s %s\n", SYSLINUX_COLOR_TIMEOUT_MSG_OLDMENU, SYSLINUX_COLOR_TIMEOUT_MSG_VESAMENU, SYSLINUX_COLOR_TIMEOUT_MSG_SHADOW);
				}
				else
				{
					fprintf(output, "MENU COLOR timeout_msg 37;40 #80ffffff #00000000 std\n");
				}

				if (strlen(SYSLINUX_COLOR_TIMEOUT) > 0)
				{
					fprintf(output, "MENU COLOR timeout %s %s %s\n", SYSLINUX_COLOR_TIMEOUT_OLDMENU, SYSLINUX_COLOR_TIMEOUT_VESAMENU, SYSLINUX_COLOR_TIMEOUT_SHADOW);
				}
				else
				{
					fprintf(output, "MENU COLOR timeout 1;37;40 #c0ffffff #00000000 std\n");
				}

				if (strlen(SYSLINUX_COLOR_TABMSG) > 0)
				{
					fprintf(output, "MENU COLOR tabmsg %s %s %s\n", SYSLINUX_COLOR_TABMSG_OLDMENU, SYSLINUX_COLOR_TABMSG_VESAMENU, SYSLINUX_COLOR_TABMSG_SHADOW);
				}
				else
				{
					fprintf(output, "MENU COLOR tabmsg 31;40 #90ffff00 #00000000 std\n");
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
	gtk_widget_destroy(GTK_WIDGET(dialog));
}

void on_btn_savesettings_clicked(void)
{
	nsyslinuxpath =  g_strdup(gtk_entry_get_text(GTK_ENTRY(entry_syslinuxpath)));

	FILE *test_file = fopen(nsyslinuxpath, "r");
	if (test_file == NULL)
	{
		free(nsyslinuxpath);
		show_error_dialog("Invalid/non existent Syslinux PATH");
		return;
	}
	else
	{
		syslinuxpath = nsyslinuxpath;
		syslinuxcfg = malloc(strlen(syslinuxpath) + 14);
		if (syslinuxcfg == NULL)
		{
			free(syslinuxpath);
			g_warning("Error: Malloc failed");
			return;
		}
		snprintf(syslinuxcfg, strlen(syslinuxpath) + 14, "%s/syslinux.cfg", syslinuxpath);
	}
		fclose(test_file);

		g_warning("%s", syslinuxcfg);
	FILE *output = fopen(syslinuxcfg, "a");
	if (output == NULL)
	{
		gchar error_message[256];
		snprintf(error_message, sizeof(error_message), "Can't write to %s", syslinuxcfg);
		show_error_dialog(error_message);
		return;
	}
	fclose(output);

	reset(window, 1);
	const gchar *ndefault = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_default));
	gboolean getprompt = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(checkbutton_prompt));
		const gchar *nprompt = getprompt ? "1" : "0";
	const gchar *ntimeout = gtk_entry_get_text(GTK_ENTRY(entry_timeout));
	const gchar *nui = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combobox_ui));
	const gchar *nsyslinux = gtk_entry_get_text(GTK_ENTRY(entry_syslinuxpath));

	const gchar *ntitle = gtk_entry_get_text(GTK_ENTRY(entry_title));

	strncpy(SYSLINUX_CONF_DEFAULT, ndefault, sizeof(SYSLINUX_CONF_DEFAULT) - 1);
	SYSLINUX_CONF_DEFAULT[sizeof(SYSLINUX_CONF_DEFAULT) - 1] = '\0';

	strncpy(SYSLINUX_CONF_PROMPT, nprompt, sizeof(SYSLINUX_CONF_PROMPT) - 1);
	SYSLINUX_CONF_PROMPT[sizeof(SYSLINUX_CONF_PROMPT) - 1] = '\0';

	strncpy(SYSLINUX_CONF_TIMEOUT, ntimeout, sizeof(SYSLINUX_CONF_TIMEOUT) - 1);
	SYSLINUX_CONF_TIMEOUT[sizeof(SYSLINUX_CONF_TIMEOUT) - 1] = '\0';

	strncpy(SYSLINUX_CONF_UI, nui, sizeof(SYSLINUX_CONF_UI) - 1);
	SYSLINUX_CONF_UI[sizeof(SYSLINUX_CONF_UI) - 1] = '\0';

	strncpy(SYSLINUX_CONF_TITLE, ntitle, sizeof(SYSLINUX_CONF_TITLE) - 1);
	SYSLINUX_CONF_TITLE[sizeof(SYSLINUX_CONF_TITLE) - 1] = '\0';
	saveconfig(0);
}

void defcombo(GtkComboBox *combo, const gchar *value)
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
		window_set_icon(GTK_WINDOW(dialog), program_icon);
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
		g_signal_connect(dialog, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);

		GtkWidget *confbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

		notebook = gtk_notebook_new();
			grid = gtk_grid_new();
			colorsgrid = gtk_grid_new();
				GtkWidget *scrolled_colorsgrid = gtk_scrolled_window_new(NULL, NULL);
				gtk_container_add(GTK_CONTAINER(scrolled_colorsgrid), colorsgrid);
				gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_colorsgrid),
					GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

			gtk_notebook_append_page(GTK_NOTEBOOK(notebook), GTK_WIDGET(grid), gtk_label_new("Settings"));
			gtk_notebook_append_page(GTK_NOTEBOOK(notebook), GTK_WIDGET(scrolled_colorsgrid), gtk_label_new("Colors"));
		//gtk_container_add(GTK_CONTAINER(dialog), notebook);
		gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
		gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	//	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
		gtk_grid_set_column_homogeneous(GTK_GRID(colorsgrid), TRUE);
		//gtk_grid_set_row_homogeneous(GTK_GRID(colorsgrid), TRUE);

		// MAIN_GRID
		label_default = gtk_label_new("Default entry:");
			combobox_default = gtk_combo_box_text_new();
		label_prompt = gtk_label_new("Show Prompt:");
			checkbutton_prompt = gtk_check_button_new();
		label_timeout = gtk_label_new("Timeout (seconds):");
			//entry_timeout = gtk_entry_new();
			GtkAdjustment *timeout_adjustment = gtk_adjustment_new(1, 0, 999999, 1, 1, 0); 
				entry_timeout = gtk_spin_button_new(timeout_adjustment, 1, 0);
		label_ui = gtk_label_new("UI Type:");
			combobox_ui = gtk_combo_box_text_new();
		label_syslinuxpath = gtk_label_new("Syslinux path:"); 
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
		//gtk_grid_attach(GTK_GRID(grid), btn_cancel, 0, 6, 1, 1);
			//gtk_grid_attach(GTK_GRID(grid), btn_savesettings, 1, 6, 1, 1);

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
		gtk_grid_attach(GTK_GRID(colorsgrid), gtk_label_new("Shadow"), 3, 0, 1, 1);
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

	gtk_box_pack_start(GTK_BOX(confbox), notebook, TRUE, TRUE, 0);

	GtkWidget *applybox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
		gtk_box_pack_end(GTK_BOX(applybox), btn_savesettings, FALSE, FALSE, 2);
		gtk_box_pack_end(GTK_BOX(applybox), btn_cancel, FALSE, FALSE, 2);

	gtk_box_pack_start(GTK_BOX(confbox), applybox, FALSE, FALSE, 0);
		gtk_container_add(GTK_CONTAINER(dialog), confbox);

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
		gtk_entry_set_text(GTK_ENTRY(entry_color_screen0), SYSLINUX_COLOR_SCREEN_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_screen1), SYSLINUX_COLOR_SCREEN_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_screen2), SYSLINUX_COLOR_SCREEN_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_title0), SYSLINUX_COLOR_TITLE_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_title1), SYSLINUX_COLOR_TITLE_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_title2), SYSLINUX_COLOR_TITLE_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_sel0), SYSLINUX_COLOR_SEL_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_sel1), SYSLINUX_COLOR_SEL_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_sel2), SYSLINUX_COLOR_SEL_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_unsel0), SYSLINUX_COLOR_UNSEL_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_unsel1), SYSLINUX_COLOR_UNSEL_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_unsel2), SYSLINUX_COLOR_UNSEL_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_help0), SYSLINUX_COLOR_HELP_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_help1), SYSLINUX_COLOR_HELP_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_help2), SYSLINUX_COLOR_HELP_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_timeout_msg0), SYSLINUX_COLOR_TIMEOUT_MSG_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_timeout_msg1), SYSLINUX_COLOR_TIMEOUT_MSG_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_timeout_msg2), SYSLINUX_COLOR_TIMEOUT_MSG_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_timeout0), SYSLINUX_COLOR_TIMEOUT_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_timeout1), SYSLINUX_COLOR_TIMEOUT_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_timeout2), SYSLINUX_COLOR_TIMEOUT_SHADOW);
		gtk_entry_set_text(GTK_ENTRY(entry_color_tabmsg0), SYSLINUX_COLOR_TABMSG_OLDMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_tabmsg1), SYSLINUX_COLOR_TABMSG_VESAMENU);
			gtk_entry_set_text(GTK_ENTRY(entry_color_tabmsg2), SYSLINUX_COLOR_TABMSG_SHADOW);

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
