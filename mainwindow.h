void on_about(GtkMenuItem *menuitem, gpointer userdata)
{
	dialog = gtk_about_dialog_new();
	window_set_icon(GTK_WINDOW(dialog), program_icon);
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "Syslinux-customizer");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2024 ItsZariep");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Syslinux config editor");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), pver);
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://itzselenux.github.io/syslinux-customizer");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog), program_icon);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void create_window(void)
{
	program_icon_names = g_ptr_array_new();
	g_ptr_array_add(program_icon_names, "syslinux-customizer");
	g_ptr_array_add(program_icon_names, "grub-customizer");
	g_ptr_array_add(program_icon_names, "menu-editor");
	g_ptr_array_add(program_icon_names, "kmenuedit");
	g_ptr_array_add(program_icon_names, "gnome-menu");
	g_ptr_array_add(program_icon_names, "system-run");
	program_icon = probe_icons_from_theme(program_icon_names);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_title(GTK_WINDOW(window), "Syslinux customizer");
		window_set_icon(GTK_WINDOW(window), program_icon);
		gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
		gtk_widget_set_size_request(window, 333, 333);
		gtk_container_set_border_width(GTK_CONTAINER(window), 1);
		g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
		menubar = gtk_menu_bar_new();
			menu_file = gtk_menu_new();
			menuitem_file = gtk_menu_item_new_with_label("File");
				submenuitem_reloadconfig = gtk_menu_item_new_with_label("Reload Config");
				submenuitem_importconfig = gtk_menu_item_new_with_label("Import config");
				submenuitem_exportconfig = gtk_menu_item_new_with_label("Export config");
				submenuitem_quit = gtk_menu_item_new_with_label("Quit");
			menu_edit = gtk_menu_new();
			menuitem_edit = gtk_menu_item_new_with_label("Edit");
				submenuitem_new = gtk_menu_item_new_with_label("New Entry");
					gtk_widget_set_name(submenuitem_new, "|-1|!|!|!|!|!|!|");
				submenuitem_importentry = gtk_menu_item_new_with_label("Import Entry");
				submenuitem_exportentry = gtk_menu_item_new_with_label("Export Selected Entry");
				submenuitem_preferences = gtk_menu_item_new_with_label("Preferences");
			menu_help = gtk_menu_new();
			menuitem_help = gtk_menu_item_new_with_label("Help");
				submenuitem_help = gtk_menu_item_new_with_label("Help");
				submenuitem_about = gtk_menu_item_new_with_label("About");

		gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem_file);
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem_file), menu_file);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), submenuitem_reloadconfig);
					g_signal_connect(submenuitem_reloadconfig, "activate", G_CALLBACK(reset), window);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), submenuitem_importconfig);
					g_signal_connect_data(submenuitem_importconfig, "activate", G_CALLBACK(on_importexport), GINT_TO_POINTER(0), NULL, 0);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), submenuitem_exportconfig);
					g_signal_connect_data(submenuitem_exportconfig, "activate", G_CALLBACK(on_importexport), GINT_TO_POINTER(1), NULL, 0);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), submenuitem_quit);
					g_signal_connect(submenuitem_quit, "activate", G_CALLBACK(gtk_main_quit), NULL);

		gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem_edit);
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem_edit), menu_edit);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_edit), submenuitem_new);
					g_signal_connect(submenuitem_new, "activate", G_CALLBACK(on_newentry), window);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_edit), submenuitem_importentry);
					g_signal_connect(submenuitem_importentry, "activate", G_CALLBACK(on_importentry), NULL);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_edit), submenuitem_exportentry);
					g_signal_connect(submenuitem_exportentry, "activate", G_CALLBACK(on_exportentry), NULL);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_edit), submenuitem_preferences);
					g_signal_connect(submenuitem_preferences, "activate", G_CALLBACK(on_preferences), NULL);

		gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuitem_help);
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuitem_help), menu_help);
				gtk_menu_shell_append(GTK_MENU_SHELL(menu_help), submenuitem_about);
					g_signal_connect(submenuitem_about, "activate", G_CALLBACK(on_about), window);

			vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

			gtk_container_add(GTK_CONTAINER(window), vbox);
			gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
			listbox = gtk_list_box_new();
			scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
			gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledwindow),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
			gtk_widget_set_vexpand(scrolledwindow, TRUE);
			gtk_container_add(GTK_CONTAINER(scrolledwindow), listbox);
			gtk_container_add(GTK_CONTAINER(vbox), scrolledwindow);
}

void load_entries(void)
{
	for (i = 0; i < entry_count; ++i)
	{
		GtkWidget *row = gtk_list_box_row_new();
		gtk_container_add(GTK_CONTAINER(listbox), row);

		GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		gtk_container_add(GTK_CONTAINER(row), box);

		GtkWidget *label = gtk_label_new(entries[i].menulabel);

		GtkWidget *edit_button = gtk_button_new_with_label("edit");
		char buffer[ML * 7];
		sprintf(buffer, "|%d|!%s|!%s|!%s|!%s|!%s|!%s|", i,
			entries[i].label, entries[i].menulabel, entries[i].command,
			entries[i].append, entries[i].com32, entries[i].initrd);
		char entrypos[6];
		sprintf(entrypos, "%d", i);
		gtk_widget_set_name(edit_button, buffer);
		gtk_widget_set_name(row, entrypos);

		gtk_widget_set_hexpand(label, TRUE);
		gtk_widget_set_halign(label, GTK_ALIGN_START);

		gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 16);

		gtk_box_pack_end(GTK_BOX(box), edit_button, FALSE, FALSE, 0);
		g_signal_connect(edit_button, "clicked", G_CALLBACK(on_edit_button_clicked), NULL);
	}
}

void windowinit(void)
{
	create_window();
	load_entries();
}

void reset(GtkWidget *widget, gint init)
{
	GList *children = gtk_container_get_children(GTK_CONTAINER(listbox));
	for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
	{
		gtk_widget_destroy(GTK_WIDGET(iter->data));
	}
	g_list_free(children);

	entry_count = 0;
	gtk_container_remove(GTK_CONTAINER(scrolledwindow), listbox);
	//gtk_widget_destroy(listbox);
	listbox = gtk_list_box_new();
	gtk_container_add(GTK_CONTAINER(scrolledwindow), listbox);
	getconf();
	for (int i = 0; i < MAX_ENTRIES; i++)
	{
		strcpy(entries[i].label, "");
		strcpy(entries[i].menulabel, "");
		strcpy(entries[i].command, "");
		strcpy(entries[i].append, "");
		strcpy(entries[i].com32, "");
		strcpy(entries[i].initrd, "");
	}
	parse_syslinux_config(syslinuxcfg, entries);
	load_entries();
	gtk_widget_show_all(window);
}
