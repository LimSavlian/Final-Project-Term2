
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================= STRUCTS =================
typedef struct
{
    int id;
    char name[50];
    char nationalId[20];
} Member;

typedef struct
{
    int id;
    char title[100];
    int total;
    int available;
} Book;

// ================= GLOBAL =================
Member *members = NULL;
Book *books = NULL;

int memberCount = 0;
int bookCount = 0;

// GUI widgets
GtkWidget *name_entry, *nid_entry;
GtkWidget *book_entry;
GtkWidget *search_entry;
GtkWidget *label_output;

// ================= MEMBER =================
void register_member(GtkWidget *widget, gpointer data)
{
    members = realloc(members, (memberCount + 1) * sizeof(Member));

    Member *m = &members[memberCount];
    m->id = memberCount + 1;

    strcpy(m->name, gtk_entry_get_text(GTK_ENTRY(name_entry)));
    strcpy(m->nationalId, gtk_entry_get_text(GTK_ENTRY(nid_entry)));

    memberCount++;

    gtk_label_set_text(GTK_LABEL(label_output), "✅ Member registered!");
}

// ================= BOOK =================
void add_book(GtkWidget *widget, gpointer data)
{
    books = realloc(books, (bookCount + 1) * sizeof(Book));

    Book *b = &books[bookCount];
    b->id = bookCount + 1;

    strcpy(b->title, gtk_entry_get_text(GTK_ENTRY(book_entry)));

    b->total = 1;
    b->available = 1;

    bookCount++;

    gtk_label_set_text(GTK_LABEL(label_output), "✅ Book added!");
}

// ================= BORROW =================
void borrow_book(GtkWidget *widget, gpointer data)
{
    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].available > 0)
        {
            books[i].available--;
            gtk_label_set_text(GTK_LABEL(label_output), "📘 Book borrowed!");
            return;
        }
    }
    gtk_label_set_text(GTK_LABEL(label_output), "❌ No available books!");
}

// ================= RETURN =================
void return_book(GtkWidget *widget, gpointer data)
{
    if (bookCount > 0)
    {
        books[0].available++;
        gtk_label_set_text(GTK_LABEL(label_output), "📗 Book returned!");
    }
}

// ================= SEARCH =================
void search(GtkWidget *widget, gpointer data)
{
    const char *key = gtk_entry_get_text(GTK_ENTRY(search_entry));

    for (int i = 0; i < bookCount; i++)
    {
        if (strstr(books[i].title, key))
        {
            gtk_label_set_text(GTK_LABEL(label_output), books[i].title);
            return;
        }
    }

    gtk_label_set_text(GTK_LABEL(label_output), "Not found!");
}

// ================= REPORT =================
void report(GtkWidget *widget, gpointer data)
{
    char buffer[200];
    int borrowed = 0;

    for (int i = 0; i < bookCount; i++)
    {
        borrowed += (books[i].total - books[i].available);
    }

    sprintf(buffer,
            "Members: %d\nBooks: %d\nBorrowed: %d",
            memberCount, bookCount, borrowed);

    gtk_label_set_text(GTK_LABEL(label_output), buffer);
}

// ================= GUI =================
int main(int argc, char *argv[])
{
    GtkWidget *window, *grid;

    GtkWidget *btn_register, *btn_addbook;
    GtkWidget *btn_borrow, *btn_return;
    GtkWidget *btn_search, *btn_report;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Library System");

    // ✅ Bigger window
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 500);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();

    // ✅ Add spacing
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    // ✅ Add margins
    gtk_widget_set_margin_start(grid, 20);
    gtk_widget_set_margin_end(grid, 20);
    gtk_widget_set_margin_top(grid, 20);
    gtk_widget_set_margin_bottom(grid, 20);

    gtk_container_add(GTK_CONTAINER(window), grid);

    // ===== REGISTER =====
    name_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Name");

    nid_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(nid_entry), "National ID");

    btn_register = gtk_button_new_with_label("Register Member");
    g_signal_connect(btn_register, "clicked", G_CALLBACK(register_member), NULL);

    // ===== BOOK =====
    book_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(book_entry), "Book Title");

    btn_addbook = gtk_button_new_with_label("Add Book");
    g_signal_connect(btn_addbook, "clicked", G_CALLBACK(add_book), NULL);

    btn_borrow = gtk_button_new_with_label("Borrow");
    g_signal_connect(btn_borrow, "clicked", G_CALLBACK(borrow_book), NULL);

    btn_return = gtk_button_new_with_label("Return");
    g_signal_connect(btn_return, "clicked", G_CALLBACK(return_book), NULL);

    // ===== SEARCH =====
    search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search book");

    btn_search = gtk_button_new_with_label("Search");
    g_signal_connect(btn_search, "clicked", G_CALLBACK(search), NULL);

    // ===== REPORT =====
    btn_report = gtk_button_new_with_label("View Report");
    g_signal_connect(btn_report, "clicked", G_CALLBACK(report), NULL);

    // ===== OUTPUT =====
    label_output = gtk_label_new("Library Ready");

    // ✅ Make inputs stretch (IMPORTANT)
    gtk_widget_set_hexpand(name_entry, TRUE);
    gtk_widget_set_hexpand(nid_entry, TRUE);
    gtk_widget_set_hexpand(book_entry, TRUE);
    gtk_widget_set_hexpand(search_entry, TRUE);

    // Layout
    gtk_grid_attach(GTK_GRID(grid), name_entry, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), nid_entry, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_register, 0, 2, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), book_entry, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_addbook, 0, 4, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_borrow, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_return, 1, 5, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), search_entry, 0, 6, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_search, 0, 7, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_report, 0, 8, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), label_output, 0, 9, 2, 1);

    gtk_widget_show_all(window);
    gtk_main();

    free(members);
    free(books);
    return 0;
}
