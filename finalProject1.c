#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ================= STRUCTS =================
typedef struct
{
    int day, month, year;
} Date;

typedef struct
{
    int memberId;
    char name[50];
    char nationalId[20];
    Date registrationDate;
    Date membershipExpiry;
    bool isActive;
} Member;

typedef struct
{
    int bookId;
    char title[100];
    int totalQuantity;
    int availableQuantity;
} Book;

typedef struct
{
    int borrowingId;
    int memberId;
    int bookId;

    Date borrowDate;
    Date dueDate;
    Date returnDate;

    bool isReturned;
    int fineAmount;
} Borrowing;

// ================= DATA =================
Member *members = NULL;
Book *books = NULL;
Borrowing *borrowings = NULL;

int memberCount = 0;
int bookCount = 0;
int borrowingCount = 0;

// ================= LOGIN =================
#define STAFF_ID 1234
GtkWidget *login_entry;
GtkWidget *main_window;

// ================= GUI =================
GtkWidget *entry_name, *entry_nid;
GtkWidget *entry_book, *entry_search;
GtkWidget *output_label;

// ================= LOGIN FUNCTION =================
void check_login(GtkWidget *widget, gpointer window)
{
    const char *input = gtk_entry_get_text(GTK_ENTRY(login_entry));

    if (atoi(input) == STAFF_ID)
    {
        gtk_widget_destroy(GTK_WIDGET(window)); // close login
        gtk_widget_show_all(main_window);       // show main window
    }
    else
    {
        gtk_entry_set_text(GTK_ENTRY(login_entry), "");
        gtk_entry_set_placeholder_text(GTK_ENTRY(login_entry), "❌ Wrong ID");
    }
}

// ================= REGISTER =================
void register_member(GtkWidget *widget, gpointer data)
{
    members = realloc(members, (memberCount + 1) * sizeof(Member));

    Member *m = &members[memberCount];
    m->memberId = memberCount + 1;

    strcpy(m->name, gtk_entry_get_text(GTK_ENTRY(entry_name)));
    strcpy(m->nationalId, gtk_entry_get_text(GTK_ENTRY(entry_nid)));

    m->isActive = true;
    m->registrationDate = (Date){1, 1, 2026};
    m->membershipExpiry = (Date){1, 1, 2027};

    memberCount++;

    gtk_label_set_text(GTK_LABEL(output_label), "✅ Member Registered");
}

// ================= ADD BOOK =================
void add_book(GtkWidget *widget, gpointer data)
{
    books = realloc(books, (bookCount + 1) * sizeof(Book));

    Book *b = &books[bookCount];
    b->bookId = bookCount + 1;

    strcpy(b->title, gtk_entry_get_text(GTK_ENTRY(entry_book)));

    b->totalQuantity = 1;
    b->availableQuantity = 1;

    bookCount++;

    gtk_label_set_text(GTK_LABEL(output_label), "✅ Book Added");
}

// ================= BORROW =================
void borrow_book(GtkWidget *widget, gpointer data)
{
    if (bookCount == 0 || memberCount == 0)
    {
        gtk_label_set_text(GTK_LABEL(output_label), "❌ Need member & book");
        return;
    }

    if (books[0].availableQuantity <= 0)
    {
        gtk_label_set_text(GTK_LABEL(output_label), "❌ No stock");
        return;
    }

    borrowings = realloc(borrowings, (borrowingCount + 1) * sizeof(Borrowing));

    Borrowing *br = &borrowings[borrowingCount];

    br->borrowingId = borrowingCount + 1;
    br->memberId = 1;
    br->bookId = 1;

    br->borrowDate = (Date){1, 6, 2026};
    br->dueDate = (Date){7, 6, 2026};
    br->isReturned = false;
    br->fineAmount = 0;

    books[0].availableQuantity--;

    borrowingCount++;

    gtk_label_set_text(GTK_LABEL(output_label), "📘 Book Borrowed");
}

// ================= RETURN =================
void return_book(GtkWidget *widget, gpointer data)
{
    if (borrowingCount == 0)
    {
        gtk_label_set_text(GTK_LABEL(output_label), "❌ No records");
        return;
    }

    Borrowing *br = &borrowings[0];

    if (br->isReturned)
    {
        gtk_label_set_text(GTK_LABEL(output_label), "❌ Already returned");
        return;
    }

    br->returnDate = (Date){10, 6, 2026};
    br->isReturned = true;

    if (br->returnDate.day > br->dueDate.day)
    {
        int late = br->returnDate.day - br->dueDate.day;
        br->fineAmount = late * 2;
    }

    books[0].availableQuantity++;

    gtk_label_set_text(GTK_LABEL(output_label), "📗 Book Returned");
}

// ================= SEARCH =================
void search_book(GtkWidget *widget, gpointer data)
{
    const char *key = gtk_entry_get_text(GTK_ENTRY(entry_search));

    for (int i = 0; i < bookCount; i++)
    {
        if (strstr(books[i].title, key))
        {
            char buffer[200];
            sprintf(buffer, "Found: %s | Available: %d",
                    books[i].title, books[i].availableQuantity);

            gtk_label_set_text(GTK_LABEL(output_label), buffer);
            return;
        }
    }

    gtk_label_set_text(GTK_LABEL(output_label), "Not Found");
}

// ================= REPORT =================
void report(GtkWidget *widget, gpointer data)
{
    int returned = 0, fineTotal = 0;

    for (int i = 0; i < borrowingCount; i++)
    {
        if (borrowings[i].isReturned)
        {
            returned++;
            fineTotal += borrowings[i].fineAmount;
        }
    }

    char buffer[300];
    sprintf(buffer,
            "📊 REPORT\nMembers: %d\nBooks: %d\nBorrowings: %d\nReturned: %d\nFine: %d",
            memberCount, bookCount, borrowingCount, returned, fineTotal);

    gtk_label_set_text(GTK_LABEL(output_label), buffer);
}

// ================= MAIN =================
int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // ===== MAIN WINDOW (HIDDEN FIRST) =====
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "Library System");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 700, 500);
    gtk_widget_hide(main_window); // ✅ IMPORTANT

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);

    gtk_widget_set_margin_start(grid, 20);
    gtk_widget_set_margin_end(grid, 20);
    gtk_widget_set_margin_top(grid, 20);
    gtk_widget_set_margin_bottom(grid, 20);

    gtk_container_add(GTK_CONTAINER(main_window), grid);

    entry_name = gtk_entry_new();
    entry_nid = gtk_entry_new();
    entry_book = gtk_entry_new();
    entry_search = gtk_entry_new();

    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_name), "Name");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_nid), "National ID");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_book), "Book Title");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_search), "Search");

    GtkWidget *btn_register = gtk_button_new_with_label("Register");
    GtkWidget *btn_add = gtk_button_new_with_label("Add Book");
    GtkWidget *btn_borrow = gtk_button_new_with_label("Borrow");
    GtkWidget *btn_return = gtk_button_new_with_label("Return");
    GtkWidget *btn_search = gtk_button_new_with_label("Search");
    GtkWidget *btn_report = gtk_button_new_with_label("Report");

    output_label = gtk_label_new("System Ready");

    gtk_grid_attach(GTK_GRID(grid), entry_name, 0, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_nid, 0, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_register, 0, 2, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), entry_book, 0, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_add, 0, 4, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_borrow, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_return, 1, 5, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), entry_search, 0, 6, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), btn_search, 0, 7, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), btn_report, 0, 8, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), output_label, 0, 9, 2, 1);

    g_signal_connect(btn_register, "clicked", G_CALLBACK(register_member), NULL);
    g_signal_connect(btn_add, "clicked", G_CALLBACK(add_book), NULL);
    g_signal_connect(btn_borrow, "clicked", G_CALLBACK(borrow_book), NULL);
    g_signal_connect(btn_return, "clicked", G_CALLBACK(return_book), NULL);
    g_signal_connect(btn_search, "clicked", G_CALLBACK(search_book), NULL);
    g_signal_connect(btn_report, "clicked", G_CALLBACK(report), NULL);

    // ===== LOGIN WINDOW =====
    GtkWidget *login_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(login_window), "Staff Login");
    gtk_window_set_default_size(GTK_WINDOW(login_window), 300, 150);

    GtkWidget *login_grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(login_window), login_grid);

    login_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(login_entry), "Enter Staff ID");

    GtkWidget *login_btn = gtk_button_new_with_label("Login");

    gtk_grid_attach(GTK_GRID(login_grid), login_entry, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(login_grid), login_btn, 0, 1, 1, 1);

    g_signal_connect(login_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(login_window); // ✅ ONLY login first

    gtk_main();

    return 0;
}
