#include <gtk/gtk.h>
#include "hangman_core.c"
#include "hangman_title.c"

int main(int argc, char* argv[]) {
    GtkWidget* window;
    GtkWidget* label_box;
    GtkWidget* title, * version, * author;
    PangoAttrList* attr_lst;
    
    gtk_init(&argc, &argv);
    
    attr_lst = pango_attr_list_new();
    pango_attr_list_insert(attr_lst, pango_attr_family_new("monospace"));
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hangman");
    gtk_widget_set_size_request(window, 800, 600);
    gtk_window_set_resizable(GTK_WINDOW(window),false);
    
    label_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_resizable(GTK_WINDOW(window),false);
    gtk_container_add(GTK_CONTAINER(window), label_box);
    
    title = gtk_label_new(HANGMAN);
    gtk_label_set_attributes(GTK_LABEL(title), attr_lst);
    gtk_box_pack_start(GTK_BOX(label_box), title, false, false, 20);
    
    version = gtk_label_new(VERSION);
    gtk_label_set_attributes(GTK_LABEL(version), attr_lst);
    gtk_box_pack_start(GTK_BOX(label_box), version, false, false, 20);
    
    author = gtk_label_new(AUTHOR);
    gtk_label_set_attributes(GTK_LABEL(author), attr_lst);
    gtk_box_pack_start(GTK_BOX(label_box), author, false, false, 20);
    
    gtk_widget_show_all(window);
    
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
        
    gtk_main();
    
    return 0;
}
