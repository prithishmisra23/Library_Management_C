#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_BOOKS 1000
#define MAX_MEMBERS 500
#define MAX_TITLE 100
#define MAX_AUTHOR 50
#define MAX_NAME 50
#define MAX_ID 20
#define FILENAME_BOOKS "books.dat"
#define FILENAME_MEMBERS "members.dat"
#define FILENAME_TRANSACTIONS "transactions.dat"

// Structure definitions
typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char ISBN[14];
    int year;
    int quantity;
    int available;
    char category[30];
} Book;

typedef struct {
    int id;
    char name[MAX_NAME];
    char membership_id[MAX_ID];
    char email[50];
    char phone[15];
    int books_issued;
    char join_date[11];
} Member;

typedef struct {
    int transaction_id;
    int book_id;
    int member_id;
    char issue_date[11];
    char due_date[11];
    char return_date[11];
    int returned;
} Transaction;

// Global arrays
Book books[MAX_BOOKS];
Member members[MAX_MEMBERS];
Transaction transactions[MAX_BOOKS * 10]; // Assuming multiple transactions per book

int book_count = 0;
int member_count = 0;
int transaction_count = 0;

// Function prototypes
void loadData();
void saveData();
void displayMenu();
void addBook();
void viewBooks();
void searchBook();
void updateBook();
void deleteBook();
void addMember();
void viewMembers();
void searchMember();
void updateMember();
void deleteMember();
void issueBook();
void returnBook();
void viewTransactions();
void generateReports();
void getCurrentDate(char* date);
void addDays(char* source, char* dest, int days);
int dateDifference(char* date1, char* date2);
int findBookById(int id);
int findBookByISBN(char* isbn);
int findMemberById(int id);
int findMemberByMembershipId(char* membership_id);
int isISBNValid(char* isbn);
void clearInputBuffer();
void printHeader(char* title);

// Main function
int main() {
    loadData();
    
    int choice;
    
    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: searchBook(); break;
            case 4: updateBook(); break;
            case 5: deleteBook(); break;
            case 6: addMember(); break;
            case 7: viewMembers(); break;
            case 8: searchMember(); break;
            case 9: updateMember(); break;
            case 10: deleteMember(); break;
            case 11: issueBook(); break;
            case 12: returnBook(); break;
            case 13: viewTransactions(); break;
            case 14: generateReports(); break;
            case 15: 
                saveData();
                printf("Data saved successfully!\n");
                break;
            case 0:
                saveData();
                printf("Thank you for using Library Management System!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
        
        if(choice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
        }
        
    } while(choice != 0);
    
    return 0;
}

// Function to display main menu
void displayMenu() {
    system("clear || cls");
    printHeader("LIBRARY MANAGEMENT SYSTEM");
    printf("1.  Add New Book\n");
    printf("2.  View All Books\n");
    printf("3.  Search Book\n");
    printf("4.  Update Book\n");
    printf("5.  Delete Book\n");
    printf("6.  Add New Member\n");
    printf("7.  View All Members\n");
    printf("8.  Search Member\n");
    printf("9.  Update Member\n");
    printf("10. Delete Member\n");
    printf("11. Issue Book\n");
    printf("12. Return Book\n");
    printf("13. View Transactions\n");
    printf("14. Generate Reports\n");
    printf("15. Save Data\n");
    printf("0.  Exit\n");
    printf("====================================\n");
}

// Function to load data from files
void loadData() {
    FILE *file;
    
    // Load books
    file = fopen(FILENAME_BOOKS, "rb");
    if(file != NULL) {
        book_count = fread(books, sizeof(Book), MAX_BOOKS, file);
        fclose(file);
    }
    
    // Load members
    file = fopen(FILENAME_MEMBERS, "rb");
    if(file != NULL) {
        member_count = fread(members, sizeof(Member), MAX_MEMBERS, file);
        fclose(file);
    }
    
    // Load transactions
    file = fopen(FILENAME_TRANSACTIONS, "rb");
    if(file != NULL) {
        transaction_count = fread(transactions, sizeof(Transaction), MAX_BOOKS * 10, file);
        fclose(file);
    }
}

// Function to save data to files
void saveData() {
    FILE *file;
    
    // Save books
    file = fopen(FILENAME_BOOKS, "wb");
    if(file != NULL) {
        fwrite(books, sizeof(Book), book_count, file);
        fclose(file);
    }
    
    // Save members
    file = fopen(FILENAME_MEMBERS, "wb");
    if(file != NULL) {
        fwrite(members, sizeof(Member), member_count, file);
        fclose(file);
    }
    
    // Save transactions
    file = fopen(FILENAME_TRANSACTIONS, "wb");
    if(file != NULL) {
        fwrite(transactions, sizeof(Transaction), transaction_count, file);
        fclose(file);
    }
}

// Function to add a new book
void addBook() {
    system("clear || cls");
    printHeader("ADD NEW BOOK");
    
    if(book_count >= MAX_BOOKS) {
        printf("Book storage is full!\n");
        return;
    }
    
    Book newBook;
    
    printf("Enter Book Details:\n");
    printf("===================\n");
    
    newBook.id = book_count > 0 ? books[book_count-1].id + 1 : 1001;
    
    printf("Title: ");
    fgets(newBook.title, MAX_TITLE, stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;
    
    printf("Author: ");
    fgets(newBook.author, MAX_AUTHOR, stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;
    
    do {
        printf("ISBN (13 digits): ");
        fgets(newBook.ISBN, 14, stdin);
        newBook.ISBN[strcspn(newBook.ISBN, "\n")] = 0;
        
        if(!isISBNValid(newBook.ISBN)) {
            printf("Invalid ISBN format! Please enter 13 digits.\n");
        }
    } while(!isISBNValid(newBook.ISBN));
    
    printf("Publication Year: ");
    scanf("%d", &newBook.year);
    clearInputBuffer();
    
    printf("Category: ");
    fgets(newBook.category, 30, stdin);
    newBook.category[strcspn(newBook.category, "\n")] = 0;
    
    printf("Total Quantity: ");
    scanf("%d", &newBook.quantity);
    clearInputBuffer();
    
    newBook.available = newBook.quantity;
    
    books[book_count++] = newBook;
    
    printf("\nBook added successfully! Book ID: %d\n", newBook.id);
}

// Function to view all books
void viewBooks() {
    system("clear || cls");
    printHeader("VIEW ALL BOOKS");
    
    if(book_count == 0) {
        printf("No books found!\n");
        return;
    }
    
    printf("%-5s %-30s %-20s %-13s %-8s %-10s %-10s %-15s\n", 
           "ID", "Title", "Author", "ISBN", "Year", "Quantity", "Available", "Category");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < book_count; i++) {
        printf("%-5d %-30s %-20s %-13s %-8d %-10d %-10d %-15s\n",
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].ISBN,
               books[i].year,
               books[i].quantity,
               books[i].available,
               books[i].category);
    }
}

// Function to search for a book
void searchBook() {
    system("clear || cls");
    printHeader("SEARCH BOOK");
    
    int choice;
    printf("Search by:\n");
    printf("1. Book ID\n");
    printf("2. ISBN\n");
    printf("3. Title\n");
    printf("4. Author\n");
    printf("5. Category\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    char searchTerm[100];
    printf("Enter search term: ");
    fgets(searchTerm, 100, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;
    
    printf("\nSearch Results:\n");
    printf("%-5s %-30s %-20s %-13s %-8s %-10s %-10s %-15s\n", 
           "ID", "Title", "Author", "ISBN", "Year", "Quantity", "Available", "Category");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    int found = 0;
    for(int i = 0; i < book_count; i++) {
        int match = 0;
        
        switch(choice) {
            case 1: 
                if(books[i].id == atoi(searchTerm)) match = 1;
                break;
            case 2:
                if(strstr(books[i].ISBN, searchTerm) != NULL) match = 1;
                break;
            case 3:
                if(strstr(books[i].title, searchTerm) != NULL) match = 1;
                break;
            case 4:
                if(strstr(books[i].author, searchTerm) != NULL) match = 1;
                break;
            case 5:
                if(strstr(books[i].category, searchTerm) != NULL) match = 1;
                break;
        }
        
        if(match) {
            found = 1;
            printf("%-5d %-30s %-20s %-13s %-8d %-10d %-10d %-15s\n",
                   books[i].id,
                   books[i].title,
                   books[i].author,
                   books[i].ISBN,
                   books[i].year,
                   books[i].quantity,
                   books[i].available,
                   books[i].category);
        }
    }
    
    if(!found) {
        printf("No books found matching the search criteria.\n");
    }
}

// Function to update a book
void updateBook() {
    system("clear || cls");
    printHeader("UPDATE BOOK");
    
    int id;
    printf("Enter Book ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = findBookById(id);
    if(index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    printf("\nCurrent Details:\n");
    printf("Title: %s\n", books[index].title);
    printf("Author: %s\n", books[index].author);
    printf("ISBN: %s\n", books[index].ISBN);
    printf("Year: %d\n", books[index].year);
    printf("Category: %s\n", books[index].category);
    printf("Quantity: %d\n", books[index].quantity);
    printf("Available: %d\n", books[index].available);
    
    printf("\nEnter new details (press Enter to keep current value):\n");
    
    printf("Title [%s]: ", books[index].title);
    char temp[100];
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        strcpy(books[index].title, temp);
    }
    
    printf("Author [%s]: ", books[index].author);
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        strcpy(books[index].author, temp);
    }
    
    printf("Category [%s]: ", books[index].category);
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        strcpy(books[index].category, temp);
    }
    
    printf("Year [%d]: ", books[index].year);
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        books[index].year = atoi(temp);
    }
    
    printf("Total Quantity [%d]: ", books[index].quantity);
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        int newQty = atoi(temp);
        int diff = newQty - books[index].quantity;
        books[index].quantity = newQty;
        books[index].available += diff;
    }
    
    printf("\nBook updated successfully!\n");
}

// Function to delete a book
void deleteBook() {
    system("clear || cls");
    printHeader("DELETE BOOK");
    
    int id;
    printf("Enter Book ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = findBookById(id);
    if(index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    printf("\nBook Details:\n");
    printf("Title: %s\n", books[index].title);
    printf("Author: %s\n", books[index].author);
    printf("ISBN: %s\n", books[index].ISBN);
    
    if(books[index].available != books[index].quantity) {
        printf("Cannot delete book! Some copies are still issued.\n");
        return;
    }
    
    char confirm;
    printf("Are you sure you want to delete this book? (y/n): ");
    scanf("%c", &confirm);
    clearInputBuffer();
    
    if(confirm == 'y' || confirm == 'Y') {
        for(int i = index; i < book_count - 1; i++) {
            books[i] = books[i + 1];
        }
        book_count--;
        printf("Book deleted successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

// Function to add a new member
void addMember() {
    system("clear || cls");
    printHeader("ADD NEW MEMBER");
    
    if(member_count >= MAX_MEMBERS) {
        printf("Member storage is full!\n");
        return;
    }
    
    Member newMember;
    
    printf("Enter Member Details:\n");
    printf("=====================\n");
    
    newMember.id = member_count > 0 ? members[member_count-1].id + 1 : 2001;
    
    printf("Name: ");
    fgets(newMember.name, MAX_NAME, stdin);
    newMember.name[strcspn(newMember.name, "\n")] = 0;
    
    // Generate membership ID
    sprintf(newMember.membership_id, "MEM%04d", newMember.id);
    
    printf("Email: ");
    fgets(newMember.email, 50, stdin);
    newMember.email[strcspn(newMember.email, "\n")] = 0;
    
    printf("Phone: ");
    fgets(newMember.phone, 15, stdin);
    newMember.phone[strcspn(newMember.phone, "\n")] = 0;
    
    newMember.books_issued = 0;
    
    getCurrentDate(newMember.join_date);
    
    members[member_count++] = newMember;
    
    printf("\nMember added successfully!\n");
    printf("Member ID: %d\n", newMember.id);
    printf("Membership ID: %s\n", newMember.membership_id);
}

// Function to view all members
void viewMembers() {
    system("clear || cls");
    printHeader("VIEW ALL MEMBERS");
    
    if(member_count == 0) {
        printf("No members found!\n");
        return;
    }
    
    printf("%-5s %-20s %-15s %-25s %-15s %-10s %-15s\n", 
           "ID", "Name", "Membership ID", "Email", "Phone", "Issued", "Join Date");
    printf("--------------------------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < member_count; i++) {
        printf("%-5d %-20s %-15s %-25s %-15s %-10d %-15s\n",
               members[i].id,
               members[i].name,
               members[i].membership_id,
               members[i].email,
               members[i].phone,
               members[i].books_issued,
               members[i].join_date);
    }
}

// Function to search for a member
void searchMember() {
    system("clear || cls");
    printHeader("SEARCH MEMBER");
    
    int choice;
    printf("Search by:\n");
    printf("1. Member ID\n");
    printf("2. Membership ID\n");
    printf("3. Name\n");
    printf("4. Email\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    char searchTerm[100];
    printf("Enter search term: ");
    fgets(searchTerm, 100, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;
    
    printf("\nSearch Results:\n");
    printf("%-5s %-20s %-15s %-25s %-15s %-10s %-15s\n", 
           "ID", "Name", "Membership ID", "Email", "Phone", "Issued", "Join Date");
    printf("--------------------------------------------------------------------------------------------------\n");
    
    int found = 0;
    for(int i = 0; i < member_count; i++) {
        int match = 0;
        
        switch(choice) {
            case 1: 
                if(members[i].id == atoi(searchTerm)) match = 1;
                break;
            case 2:
                if(strstr(members[i].membership_id, searchTerm) != NULL) match = 1;
                break;
            case 3:
                if(strstr(members[i].name, searchTerm) != NULL) match = 1;
                break;
            case 4:
                if(strstr(members[i].email, searchTerm) != NULL) match = 1;
                break;
        }
        
        if(match) {
            found = 1;
            printf("%-5d %-20s %-15s %-25s %-15s %-10d %-15s\n",
                   members[i].id,
                   members[i].name,
                   members[i].membership_id,
                   members[i].email,
                   members[i].phone,
                   members[i].books_issued,
                   members[i].join_date);
        }
    }
    
    if(!found) {
        printf("No members found matching the search criteria.\n");
    }
}

// Function to update a member
void updateMember() {
    system("clear || cls");
    printHeader("UPDATE MEMBER");
    
    int id;
    printf("Enter Member ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = findMemberById(id);
    if(index == -1) {
        printf("Member not found!\n");
        return;
    }
    
    printf("\nCurrent Details:\n");
    printf("Name: %s\n", members[index].name);
    printf("Email: %s\n", members[index].email);
    printf("Phone: %s\n", members[index].phone);
    printf("Books Issued: %d\n", members[index].books_issued);
    
    printf("\nEnter new details (press Enter to keep current value):\n");
    
    printf("Name [%s]: ", members[index].name);
    char temp[100];
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        strcpy(members[index].name, temp);
    }
    
    printf("Email [%s]: ", members[index].email);
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        strcpy(members[index].email, temp);
    }
    
    printf("Phone [%s]: ", members[index].phone);
    fgets(temp, 100, stdin);
    temp[strcspn(temp, "\n")] = 0;
    if(strlen(temp) > 0) {
        strcpy(members[index].phone, temp);
    }
    
    printf("\nMember updated successfully!\n");
}

// Function to delete a member
void deleteMember() {
    system("clear || cls");
    printHeader("DELETE MEMBER");
    
    int id;
    printf("Enter Member ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = findMemberById(id);
    if(index == -1) {
        printf("Member not found!\n");
        return;
    }
    
    printf("\nMember Details:\n");
    printf("Name: %s\n", members[index].name);
    printf("Membership ID: %s\n", members[index].membership_id);
    printf("Books Issued: %d\n", members[index].books_issued);
    
    if(members[index].books_issued > 0) {
        printf("Cannot delete member! Some books are still issued.\n");
        return;
    }
    
    char confirm;
    printf("Are you sure you want to delete this member? (y/n): ");
    scanf("%c", &confirm);
    clearInputBuffer();
    
    if(confirm == 'y' || confirm == 'Y') {
        for(int i = index; i < member_count - 1; i++) {
            members[i] = members[i + 1];
        }
        member_count--;
        printf("Member deleted successfully!\n");
    } else {
        printf("Deletion cancelled.\n");
    }
}

// Function to issue a book
void issueBook() {
    system("clear || cls");
    printHeader("ISSUE BOOK");
    
    int book_id, member_id;
    
    printf("Enter Book ID: ");
    scanf("%d", &book_id);
    clearInputBuffer();
    
    int book_index = findBookById(book_id);
    if(book_index == -1) {
        printf("Book not found!\n");
        return;
    }
    
    if(books[book_index].available <= 0) {
        printf("Book not available! All copies are issued.\n");
        return;
    }
    
    printf("Enter Member ID: ");
    scanf("%d", &member_id);
    clearInputBuffer();
    
    int member_index = findMemberById(member_id);
    if(member_index == -1) {
        printf("Member not found!\n");
        return;
    }
    
    if(members[member_index].books_issued >= 5) {
        printf("Member has reached maximum issue limit (5 books)!\n");
        return;
    }
    
    // Create transaction
    Transaction newTransaction;
    newTransaction.transaction_id = transaction_count > 0 ? transactions[transaction_count-1].transaction_id + 1 : 3001;
    newTransaction.book_id = book_id;
    newTransaction.member_id = member_id;
    getCurrentDate(newTransaction.issue_date);
    addDays(newTransaction.issue_date, newTransaction.due_date, 14); // 14 days due
    newTransaction.return_date[0] = '\0';
    newTransaction.returned = 0;
    
    // Update book and member
    books[book_index].available--;
    members[member_index].books_issued++;
    
    // Add transaction
    transactions[transaction_count++] = newTransaction;
    
    printf("\nBook issued successfully!\n");
    printf("Transaction ID: %d\n", newTransaction.transaction_id);
    printf("Book: %s\n", books[book_index].title);
    printf("Member: %s\n", members[member_index].name);
    printf("Issue Date: %s\n", newTransaction.issue_date);
    printf("Due Date: %s\n", newTransaction.due_date);
}

// Function to return a book
void returnBook() {
    system("clear || cls");
    printHeader("RETURN BOOK");
    
    int transaction_id;
    printf("Enter Transaction ID: ");
    scanf("%d", &transaction_id);
    clearInputBuffer();
    
    int found = 0;
    for(int i = 0; i < transaction_count; i++) {
        if(transactions[i].transaction_id == transaction_id && transactions[i].returned == 0) {
            found = 1;
            
            // Update transaction
            getCurrentDate(transactions[i].return_date);
            transactions[i].returned = 1;
            
            // Find book and member
            int book_index = findBookById(transactions[i].book_id);
            int member_index = findMemberById(transactions[i].member_id);
            
            // Update book and member
            if(book_index != -1) {
                books[book_index].available++;
            }
            if(member_index != -1) {
                members[member_index].books_issued--;
            }
            
            // Calculate fine if overdue
            char current_date[11];
            getCurrentDate(current_date);
            int days_overdue = dateDifference(transactions[i].due_date, current_date);
            
            printf("\nBook returned successfully!\n");
            printf("Transaction ID: %d\n", transactions[i].transaction_id);
            printf("Return Date: %s\n", transactions[i].return_date);
            
            if(days_overdue > 0) {
                float fine = days_overdue * 5.0; // $5 per day late fee
                printf("Overdue by %d days\n", days_overdue);
                printf("Fine: $%.2f\n", fine);
            } else {
                printf("Returned on time. No fine.\n");
            }
            break;
        }
    }
    
    if(!found) {
        printf("Transaction not found or book already returned!\n");
    }
}

// Function to view transactions
void viewTransactions() {
    system("clear || cls");
    printHeader("VIEW TRANSACTIONS");
    
    if(transaction_count == 0) {
        printf("No transactions found!\n");
        return;
    }
    
    printf("%-10s %-8s %-8s %-12s %-12s %-12s %-8s\n", 
           "Trans ID", "Book ID", "Member ID", "Issue Date", "Due Date", "Return Date", "Status");
    printf("----------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < transaction_count; i++) {
        printf("%-10d %-8d %-8d %-12s %-12s %-12s %-8s\n",
               transactions[i].transaction_id,
               transactions[i].book_id,
               transactions[i].member_id,
               transactions[i].issue_date,
               transactions[i].due_date,
               transactions[i].return_date[0] ? transactions[i].return_date : "N/A",
               transactions[i].returned ? "Returned" : "Issued");
    }
}

// Function to generate reports
void generateReports() {
    system("clear || cls");
    printHeader("GENERATE REPORTS");
    
    int choice;
    printf("1. Books Available\n");
    printf("2. Books Issued\n");
    printf("3. Overdue Books\n");
    printf("4. Member Report\n");
    printf("5. Category-wise Report\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    switch(choice) {
        case 1: {
            system("clear || cls");
            printHeader("BOOKS AVAILABLE");
            printf("%-5s %-30s %-20s %-10s\n", "ID", "Title", "Author", "Available");
            printf("--------------------------------------------------------------\n");
            for(int i = 0; i < book_count; i++) {
                if(books[i].available > 0) {
                    printf("%-5d %-30s %-20s %-10d\n",
                           books[i].id,
                           books[i].title,
                           books[i].author,
                           books[i].available);
                }
            }
            break;
        }
        case 2: {
            system("clear || cls");
            printHeader("BOOKS CURRENTLY ISSUED");
            printf("%-5s %-30s %-8s %-12s %-12s\n", 
                   "Book ID", "Title", "Member ID", "Issue Date", "Due Date");
            printf("-----------------------------------------------------------------\n");
            for(int i = 0; i < transaction_count; i++) {
                if(transactions[i].returned == 0) {
                    int book_index = findBookById(transactions[i].book_id);
                    if(book_index != -1) {
                        printf("%-5d %-30s %-8d %-12s %-12s\n",
                               transactions[i].book_id,
                               books[book_index].title,
                               transactions[i].member_id,
                               transactions[i].issue_date,
                               transactions[i].due_date);
                    }
                }
            }
            break;
        }
        case 3: {
            system("clear || cls");
            printHeader("OVERDUE BOOKS");
            char current_date[11];
            getCurrentDate(current_date);
            
            printf("%-5s %-30s %-8s %-12s %-12s %-8s\n", 
                   "Book ID", "Title", "Member ID", "Due Date", "Today", "Days Late");
            printf("--------------------------------------------------------------------------\n");
            
            int overdue_count = 0;
            for(int i = 0; i < transaction_count; i++) {
                if(transactions[i].returned == 0) {
                    int days_overdue = dateDifference(transactions[i].due_date, current_date);
                    if(days_overdue > 0) {
                        int book_index = findBookById(transactions[i].book_id);
                        if(book_index != -1) {
                            printf("%-5d %-30s %-8d %-12s %-12s %-8d\n",
                                   transactions[i].book_id,
                                   books[book_index].title,
                                   transactions[i].member_id,
                                   transactions[i].due_date,
                                   current_date,
                                   days_overdue);
                            overdue_count++;
                        }
                    }
                }
            }
            if(overdue_count == 0) {
                printf("No overdue books!\n");
            }
            break;
        }
        case 4: {
            system("clear || cls");
            printHeader("MEMBER REPORT");
            printf("%-5s %-20s %-15s %-10s\n", "ID", "Name", "Books Issued", "Join Date");
            printf("----------------------------------------------------\n");
            for(int i = 0; i < member_count; i++) {
                printf("%-5d %-20s %-15d %-10s\n",
                       members[i].id,
                       members[i].name,
                       members[i].books_issued,
                       members[i].join_date);
            }
            break;
        }
        case 5: {
            system("clear || cls");
            printHeader("CATEGORY-WISE BOOKS");
            
            // Count books per category
            char categories[100][30];
            int category_count[100] = {0};
            int unique_categories = 0;
            
            for(int i = 0; i < book_count; i++) {
                int found = 0;
                for(int j = 0; j < unique_categories; j++) {
                    if(strcmp(categories[j], books[i].category) == 0) {
                        category_count[j]++;
                        found = 1;
                        break;
                    }
                }
                if(!found) {
                    strcpy(categories[unique_categories], books[i].category);
                    category_count[unique_categories] = 1;
                    unique_categories++;
                }
            }
            
            printf("%-20s %-10s\n", "Category", "Book Count");
            printf("------------------------------\n");
            for(int i = 0; i < unique_categories; i++) {
                printf("%-20s %-10d\n", categories[i], category_count[i]);
            }
            break;
        }
        default:
            printf("Invalid choice!\n");
    }
}

// Helper function to get current date
void getCurrentDate(char* date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// Helper function to add days to a date
void addDays(char* source, char* dest, int days) {
    struct tm tm = {0};
    sscanf(source, "%4d-%2d-%2d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday);
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    
    time_t t = mktime(&tm);
    t += days * 24 * 60 * 60;
    
    struct tm* new_tm = localtime(&t);
    sprintf(dest, "%04d-%02d-%02d", new_tm->tm_year + 1900, new_tm->tm_mon + 1, new_tm->tm_mday);
}

// Helper function to calculate difference between two dates
int dateDifference(char* date1, char* date2) {
    struct tm tm1 = {0};
    struct tm tm2 = {0};
    
    sscanf(date1, "%4d-%2d-%2d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday);
    sscanf(date2, "%4d-%2d-%2d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday);
    
    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm2.tm_year -= 1900;
    tm2.tm_mon -= 1;
    
    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);
    
    double diff = difftime(t2, t1) / (60 * 60 * 24);
    return (int)diff;
}

// Helper function to find book by ID
int findBookById(int id) {
    for(int i = 0; i < book_count; i++) {
        if(books[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Helper function to find book by ISBN
int findBookByISBN(char* isbn) {
    for(int i = 0; i < book_count; i++) {
        if(strcmp(books[i].ISBN, isbn) == 0) {
            return i;
        }
    }
    return -1;
}

// Helper function to find member by ID
int findMemberById(int id) {
    for(int i = 0; i < member_count; i++) {
        if(members[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Helper function to find member by Membership ID
int findMemberByMembershipId(char* membership_id) {
    for(int i = 0; i < member_count; i++) {
        if(strcmp(members[i].membership_id, membership_id) == 0) {
            return i;
        }
    }
    return -1;
}

// Helper function to validate ISBN
int isISBNValid(char* isbn) {
    if(strlen(isbn) != 13) {
        return 0;
    }
    
    for(int i = 0; i < 13; i++) {
        if(!isdigit(isbn[i])) {
            return 0;
        }
    }
    return 1;
}

// Helper function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Helper function to print header
void printHeader(char* title) {
    printf("====================================\n");
    printf("    %s\n", title);
    printf("====================================\n");
}
