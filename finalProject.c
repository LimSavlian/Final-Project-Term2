#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    int day;
    int month;
    int year;
} Date;

typedef struct
{
    int memberId;
    char name[20];
    char institution[50];
    Date registrationDate;
    bool paymentStatus;
} Member;

typedef struct
{
    int borrowingId;
    int memberId;
    int bookId;
    Date borrowDate;
    Date returnDate;
} Borrowing;

typedef struct
{
    int bookId;
    char title[70];
    char author[20];
    int quantity;
} Books;

typedef struct
{
    int memberId;
    int amount;
    Date paymentDate;
    int FineAmount;
} Date;
