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
    char name[50];
    char institution[100];
    Date dateOfBirth;
    char nationalId[10];

    Date registrationDate;
    Date membershipExpiry;

    bool isActive;
} Member;

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

typedef struct
{
    int bookId;
    char title[100];
    char author[50];

    int totalQuantity;
    int availableQuantity;
} Book;

typedef struct
{
    int paymentId;
    int memberId;

    int amount;
    Date paymentDate;

    int yearCovered;
} MembershipPayment;
