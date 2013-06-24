#include <gtk/gtk.h>

#include "idoscalemenuitem.h"
#include "idocalendarmenuitem.h"
#include "idoentrymenuitem.h"
#include "idoswitchmenuitem.h"
#include "idousermenuitem.h"
#include "config.h"

static void
slider_grabbed (GtkWidget *widget, gpointer user_data)
{
  g_print ("grabbed\n");
}

static void
slider_released (GtkWidget *widget, gpointer user_data)
{
  g_print ("released\n");
}

static GtkWidget *
create_user_menu (const char * username,
                  const char * filename,
                  gboolean is_logged_in,
                  gboolean is_active)
{
  GtkWidget * ret;
  GFile * file = filename ? g_file_new_for_path (filename) : NULL;
  GIcon * icon = file ? g_file_icon_new (file) : NULL;

  ret = g_object_new (IDO_USER_MENU_ITEM_TYPE,
                      "label", username,
                      "icon", icon,
                      "is-logged-in", is_logged_in,
                      "is-current-user", is_active,
                      NULL);

  g_clear_object (&icon);
  g_clear_object (&file);
  return ret;
}

int
main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *menu;
  GtkWidget *menuitem;
  GtkWidget *root;
  GtkWidget *menubar;
  GtkWidget *image;
  GtkWidget *label;

  g_unsetenv ("UBUNTU_MENUPROXY");

  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "Menus");
  gtk_widget_set_size_request (window, 300, 200);
  g_signal_connect (window, "delete-event",
                    G_CALLBACK (gtk_main_quit), NULL);

  vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

  gtk_container_add (GTK_CONTAINER (window), vbox);

  menubar = gtk_menu_bar_new ();
  gtk_box_pack_start (GTK_BOX (vbox), menubar, FALSE, FALSE, 0);

  menu = gtk_menu_new ();

  root = gtk_menu_item_new_with_label ("File");
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (root), menu);

  menuitem = gtk_menu_item_new_with_label ("New");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);

  menuitem = gtk_menu_item_new_with_label ("Open");
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);

  menuitem = ido_scale_menu_item_new_with_range ("Volume",
                                                 IDO_RANGE_STYLE_DEFAULT,
                                                 65, 0, 100, 1);
  ido_scale_menu_item_set_style (IDO_SCALE_MENU_ITEM (menuitem), IDO_SCALE_MENU_ITEM_STYLE_IMAGE);
  image = ido_scale_menu_item_get_primary_image (IDO_SCALE_MENU_ITEM (menuitem));
  gtk_image_set_from_stock (GTK_IMAGE (image), GTK_STOCK_NEW, GTK_ICON_SIZE_MENU);
  image = ido_scale_menu_item_get_secondary_image (IDO_SCALE_MENU_ITEM (menuitem));
  gtk_image_set_from_stock (GTK_IMAGE (image), GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);

  g_signal_connect (menuitem, "slider-grabbed", G_CALLBACK (slider_grabbed), NULL);
  g_signal_connect (menuitem, "slider-released", G_CALLBACK (slider_released), NULL);

  menuitem = ido_entry_menu_item_new ();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);

  menuitem = ido_switch_menu_item_new ();
  label = gtk_label_new ("This is a switch");
  gtk_widget_show(label);
  gtk_container_add (ido_switch_menu_item_get_content_area(IDO_SWITCH_MENU_ITEM(menuitem)), label);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);

  menuitem = ido_calendar_menu_item_new ();
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);

  menuitem = ido_scale_menu_item_new_with_range ("Volume",
                                                 IDO_RANGE_STYLE_SMALL,
                                                 65, 0, 100, 1);
  ido_scale_menu_item_set_style (IDO_SCALE_MENU_ITEM (menuitem), IDO_SCALE_MENU_ITEM_STYLE_IMAGE);
  image = ido_scale_menu_item_get_primary_image (IDO_SCALE_MENU_ITEM (menuitem));
  gtk_image_set_from_stock (GTK_IMAGE (image), GTK_STOCK_NEW, GTK_ICON_SIZE_MENU);
  image = ido_scale_menu_item_get_secondary_image (IDO_SCALE_MENU_ITEM (menuitem));
  gtk_image_set_from_stock (GTK_IMAGE (image), GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), menuitem);
  g_signal_connect (menuitem, "slider-grabbed", G_CALLBACK (slider_grabbed), NULL);
  g_signal_connect (menuitem, "slider-released", G_CALLBACK (slider_released), NULL);

  /**
  ***  Users
  **/

  gtk_menu_shell_append (GTK_MENU_SHELL (menu),
                         create_user_menu ("Guest",
                                           NULL,
                                           FALSE, FALSE));

  gtk_menu_shell_append (GTK_MENU_SHELL (menu),
                         create_user_menu ("Bobby Fischer",
                                           "/usr/share/pixmaps/faces/chess.jpg",
                                           FALSE, FALSE));

  gtk_menu_shell_append (GTK_MENU_SHELL (menu),
                         create_user_menu ("Linus Torvalds",
                                           "/usr/share/pixmaps/faces/penguin.jpg",
                                           TRUE, FALSE));
 
  gtk_menu_shell_append (GTK_MENU_SHELL (menu),
                         create_user_menu ("Mark Shuttleworth",
                                           "/usr/share/pixmaps/faces/astronaut.jpg",
                                           TRUE, TRUE));

  gtk_menu_shell_append (GTK_MENU_SHELL (menu), ido_user_menu_item_new ());
  gtk_menu_shell_append (GTK_MENU_SHELL (menu), gtk_separator_menu_item_new ());

  /* Add the menubar */
  gtk_menu_shell_append (GTK_MENU_SHELL (menubar), root);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
