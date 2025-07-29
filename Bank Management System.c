#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

// Structure to store user details
struct User
{
    int accountNumber;
    char username[50];
    char password[50];
    char phone[15];
    float balance;
    char dateOfBirth[11];
    char address[100];
    char email[50];
};

// Function prototypes
void header();
void boot(); // Boot Graphics
void exitAnimation();

// User-Info Management
void viewinfo(struct User *user);
void changePassword(struct User *user);
void changeEmail(struct User *user);
void changePhoneNumber(struct User *user);
void createAccount();
int login(struct User *user);

// Banking Functions
void depositMoney(struct User *user);
void withdrawMoney(struct User *user);
void transferMoney(struct User *user);
void accountStatement(struct User *user);
void logTransaction(int accountNumber, const char *type, float amount, float balance);
void updateUserBalance(struct User *user);

// Input Validation
int isValidUsername(char *username);
int isValidPassword(char *password);
int isValidDateFormat(char *date);
int isValidPhoneNumber(char *phoneNumber);
int isValidEmail(char *email);
void lowercase(char str[]);
void continueKey();
void Bufferflush();
void encryptPassword(char *password, int key); // password mask using(*)

// Main function
int main()
{
    struct User currentUser;
    int choice;
    boot();

System_dash:
    header();
    // Ask for account creation or login
    printf("\n\t\t\t - - *** NAMASTE *** - - \n\n");
    Sleep(1000);
    printf("\t1. Create a Bank Account\n");
    Sleep(400);
    printf("\t2. Already Have An Accont? -> Login\n");
    Sleep(400);
    printf("\t3. Exit Bank\n\n");
    Sleep(700);
    printf("\tEnter your choice: ");
    scanf("%d", &choice);
    Bufferflush();
    if (choice == 1)
    {
        createAccount();
        if (login(&currentUser) != 1) // 1 for success and 0 for fail
        {
            printf("\tLogin failed.Try  again with different details.\n\t");
            continueKey();
            goto System_dash;
        }
    }
    else if (choice == 2)
    {
        if (login(&currentUser) != 1) // 1 for success and 0 for fail
        {
            printf("\tLogin failed.Try  again with different details.\n\t");
            continueKey();
            goto System_dash;
        }
    }
    else if (choice == 3)
    {
        // printf("\tThank you! Visit us again.\n");
        exitAnimation();
    }

    else
    {
        printf("\tInvalid choice\n\n");
        printf("Press any key to continue.");
        getch();
        goto System_dash;
    }

    // Banking menu for logged-in users
Customer_dash:
    while (1)
    {
        header();
        printf("\nWelcome, %s!\t\t\tCurrent Balance::NPR %.2f \n\n", currentUser.username, currentUser.balance);
        printf("\t1 --> Deposit\n");
        printf("\t2 --> Withdraw\n");
        printf("\t3 --> Transfer \n");
        printf("\t4 --> Transactions\n");
        printf("\t5 --> Setting\n");
        printf("\t6 --> Log Out\n");
        printf("\t7 --> Exit\n\n");
        printf("\tEnter your choice: ");
        scanf("%d", &choice);
        Bufferflush();

        switch (choice)
        {
        case 1:
            depositMoney(&currentUser);
            break;
        case 2:
            withdrawMoney(&currentUser);
            break;
        case 3:
            transferMoney(&currentUser);
            break;
        case 4:
            accountStatement(&currentUser);
            break;
        case 5:
            while (1)
            {
                header();

                printf(" \n\t\t\t- - - SETTINGS - - - \n\n");
                printf("\t1 --> Personal Information \n");
                printf("\t2 --> Change Password\n");
                printf("\t3 --> Change Email\n");
                printf("\t4 --> Change Phone Number\n");
                printf("\t5 --> Back\n\n");
                Sleep(600);
                printf("\tEnter your choice: ");
                scanf("%d", &choice);
                Bufferflush();
                switch (choice)
                {
                case 1:
                    viewinfo(&currentUser);
                    break;
                case 2:
                    changePassword(&currentUser);
                    break;
                case 3:
                    changeEmail(&currentUser);
                    break;
                case 4:
                    changePhoneNumber(&currentUser);
                    break;

                case 5:
                    goto Customer_dash;
                default:

                    printf("\tInvalid choice. Please try again.\n");
                    printf("Press any key to continue.");
                    getch();
                }
            }
            break;
        case 6:
            goto System_dash;
            break;
        case 7:
            printf("\tThank you! Visit us again.\n");
            exitAnimation();
        default:
            printf("\tInvalid choice. Please try again.\n");
            printf("Press any key to continue.");
            getch();
        }
    }

    return 0;
}

// Function to create a new account
void createAccount()
{
    struct User newUser, temp;
    FILE *file;
    FILE *f;
    int lastAccNo = 12345678;
    header();

    // Open the user details file to find the last account number
    f = fopen("userdetail.txt", "r");
    if (f != NULL)
    {
        while (fscanf(f, "%d %s %s %s %f %s %s %s", &newUser.accountNumber, newUser.username, newUser.phone, newUser.password, &newUser.balance, newUser.dateOfBirth, newUser.address, newUser.email) != EOF)
        {
            if (newUser.accountNumber > lastAccNo)
            {
                lastAccNo = newUser.accountNumber;
            }
        }

        fclose(f);
    }

    // Getting user inputs
    // Getting user Full name
    while (1)
    {
        printf("\tEnter your username : ");
        scanf("%s", newUser.username); //%[^\n]
        lowercase(newUser.username);   // converting username to lowercase

        if (isValidUsername(newUser.username) != 1)
        {
            printf("\tInvalid username! Please try again. \nNote: Username must be 5-30 characters long and contain only letters.\n");
        }

        else
            break;
    }
    Bufferflush();

    // Get user date of birth
    while (1)
    {
        printf("\tEnter your date of birth in BS (YYYY-MM-DD): ");
        scanf("%s", newUser.dateOfBirth);
        Bufferflush();
        if (isValidDateFormat(newUser.dateOfBirth) != 1)
        {
            printf("\tError! Ensure the date format is YYYY-MM-DD.\n");
        }
        else
        {
            break;
        }
    }

    // Get user address
    while (1)
    {
        printf("\tEnter your address: ");
        scanf("%s", newUser.address);
        Bufferflush();
        lowercase(newUser.address);

        // Check if the address contains only alphanumeric characters
        int isValid = 1; // Flag to check validity
        for (int i = 0; i < strlen(newUser.address); i++)
        {
            if (!isalnum(newUser.address[i]))
            {
                isValid = 0;
                break;
            }
        }
        if (isValid)
        {
            break; // Address is valid
        }
        else
        {
            printf("\tError! Address must contain only alphanumeric characters. Please try again.\n");
        }
    }

    // Get valid email and check for duplicates

    while (1)
    {
        while (1)
        {
            printf("\tEnter your email: ");
            scanf("%s", newUser.email);
            Bufferflush();
            if (isValidEmail(newUser.email) != 1)
            {
                printf("\tError! Invalid email format.\n");
            }
            else
            {
                break;
            }
        }

        // Check if email already exists
        f = fopen("userdetail.txt", "r");
        int emailExists = 0;
        if (f != NULL)
        {
            while (fscanf(f, "%d %s %s %s %f %s %s %s\n", &temp.accountNumber, temp.username, temp.phone, temp.password, &temp.balance, temp.dateOfBirth, temp.address, temp.email) != EOF)
            {
                if (strcmp(temp.email, newUser.email) == 0)
                {
                    emailExists = 1;
                    break;
                }
            }
            fclose(f);
        }

        if (emailExists)
        {
            printf("\tError! An account with this email already exists. Please try a different email.\n");
        }
        else
        {
            break;
        }
    }

    // Get valid phone number and check for duplicates
    while (1)
    {
        while (1)
        {
            printf("\tEnter your phone number: ");
            scanf("%s", newUser.phone);
            Bufferflush();
            if (isValidPhoneNumber(newUser.phone) != 1)
            {
                printf("\tError! Invalid phone number\n");
            }
            else
            {
                break;
            }
        }

        // Check if phone number already exists
        f = fopen("userdetail.txt", "r");
        int phoneExists = 0;
        if (f != NULL)
        {
            while (fscanf(f, "%d %s %s %s %f %s %s %s\n", &temp.accountNumber, temp.username, temp.phone, temp.password, &temp.balance, temp.dateOfBirth, temp.address, temp.email) != EOF)
            {
                if (strcmp(temp.phone, newUser.phone) == 0)
                {
                    phoneExists = 1;
                    break;
                }
            }
            fclose(f);
        }

        if (phoneExists)
        {
            printf("\tAn account with this phone number already exists. Try entering with a different phone number.\n");
        }
        else
        {
            break;
        }
    }

    // Getting Password...
    while (1)
    {
        encryptPassword(newUser.password, 1); // key==1
        if (isValidPassword(newUser.password) != 1)
        {
            printf("\tError! Too weak password. Try again.\n");
            printf("\tNote: Password should contain at least 8 characters, ");
            printf("1 uppercase letter, 1 lowercase letter, 1 digit, and 1 special character.\n");
        }

        else
            break;
    }

    Bufferflush();

    newUser.accountNumber = lastAccNo + 1;
    newUser.balance = 0.0;

    
    // Save user details to file
    file = fopen("userdetail.txt", "a"); // opening file in append mode
    if (file == NULL)
    {
        printf("\tError opening user details file.\n");
        return;
    }
    fprintf(file, "%d %s %s %s %.2f %s %s %s\n", newUser.accountNumber, newUser.username, newUser.phone, newUser.password, newUser.balance, newUser.dateOfBirth, newUser.address, newUser.email);
    fclose(file);

    printf("\n\t\t Please wait! Your data is being processed....\n");
    Sleep(600);
    printf("\t\t Almost there....\n\n");
    Sleep(400);

    printf("\tAccount created successfully!\n");
    printf("\tYour account number is: %d\n", newUser.accountNumber);
    printf("\nPress any key to procced to Login Page.");
    getch();
}

// Function to authenticate user
int login(struct User *user)
{

    int accountNumber;
    char username[50];
    char password[50];

    FILE *file = fopen("userdetail.txt", "r");
    header();

    if (file == NULL)
    {
        printf("\tNo user accounts found. Please create an account first.\n");
        continueKey();
        return 0;
    }

    printf("\tEnter your account number: ");
    scanf("%d", &accountNumber);
    Bufferflush();

    printf("\tEnter your username: ");
    scanf("%s", username);
    lowercase(username);
    Bufferflush();
    
//Getting password from user
    encryptPassword(password,4); //key 4 // encryption while entering password
    printf("\n\t\tPlease wait....\n");
    Sleep(700);

    // Search for the user in the file
    while (fscanf(file, "%d %s %s %s %f %s %s %s", &user->accountNumber, user->username, user->phone, user->password, &user->balance, user->dateOfBirth, user->address, user->email) != EOF)
    {
        if (user->accountNumber == accountNumber && strcmp(user->username, username) == 0 && strcmp(user->password, password) == 0)
        {
            fclose(file);
            return 1; // Login successful
        }
    }

    fclose(file);
    return 0; // Login failed
}

// Function to deposit money
void depositMoney(struct User *user)
{
    float amount;
    header();

    printf("\tEnter the amount to deposit: ");
    scanf("%f", &amount);
    Bufferflush();

    if (amount <= 0)
    {
        printf("\tInvalid amount. Please enter a positive value.\n");
        printf("Press any key to continue.");
        getch();
        return;
    }

    user->balance += amount;
    printf("\tDeposit successful. New balance: %.2f\n", user->balance);
    continueKey();
    // Log the transaction
    logTransaction(user->accountNumber, "DEPOSIT", amount, user->balance);

    // Update user balance in the file
    updateUserBalance(user);
}

// Function to withdraw money
void withdrawMoney(struct User *user)
{
    float amount;
    header();

    printf("\tEnter the amount to withdraw: ");
    scanf("%f", &amount);
    Bufferflush();

    if (amount <= 0)
    {
        printf("\tInvalid amount. Please enter a positive value.\n");
        continueKey();
        return;
    }

    if (amount > user->balance)
    {
        printf("\tInsufficient balance.\n");
        continueKey();
        return;
    }

    user->balance -= amount;
    printf("\tWithdrawal successful. New balance: %.2f\n", user->balance);
    continueKey();
    // Log the transaction
    logTransaction(user->accountNumber, "WITHDRAW", amount, user->balance);

    // Update user balance in the file
    updateUserBalance(user);
}

void transferMoney(struct User *user)
{
    float amount;
    int receiverAccNo;
    FILE *file = fopen("userdetail.txt", "r");
    header();
    while (1)
    {
        printf("\tEnter the account number to transfer to: ");
        scanf("%d", &receiverAccNo);
        Bufferflush();
        printf("\tEnter the amount to transfer: ");
        scanf("%f", &amount);
        Bufferflush();
        if (amount <= 0)
        {
            printf("\tInvalid amount. Please enter a positive value.\n");
            continueKey();
            return;
        }

        if (amount > user->balance)
        {
            printf("\tInsufficient balance.\n");
            continueKey();
            return;
        }
        if (receiverAccNo == user->accountNumber)
        {
            printf("Transaction failed! You cannot transfer money to your own account.");
            continueKey();
            return;
        }
        // Checking whether the receiver account exists
        struct User receiver;
        int userFound = 0;
        while (fscanf(file, "%d %s %s %s %f %s %s %s", &receiver.accountNumber, receiver.username, receiver.phone, receiver.password, &receiver.balance, receiver.dateOfBirth, receiver.address, receiver.email) != EOF)
        {
            if (receiver.accountNumber == receiverAccNo)
            {
                userFound = 1;
                break;
            }
        }
        fclose(file);

        if (!userFound)
        {
            printf("\tReceiver account does not exist.\n");
            continueKey();
            return;
        }

        // Update balances
        user->balance -= amount;
        receiver.balance += amount;

        // Log the transaction
        logTransaction(user->accountNumber, "TRANSFER", amount, user->balance);
        logTransaction(receiver.accountNumber, "RECEIVE", amount, receiver.balance);

        // Update user balances in the file
        updateUserBalance(user);
        updateUserBalance(&receiver);

        printf("\tTransfer successful! New balance: %.2f\n", user->balance);
        continueKey();
        return;
    }
}

// Function to display account statement (only for the logged-in user)
void accountStatement(struct User *user)
{
    FILE *file = fopen("transaction_log.txt", "r");
    header();

    if (file == NULL)
    {
        printf("\tNo transactions found.\n");
        continueKey();
        return;
    }

    printf("\n\tAccount Statement for Account Number: %d\n", user->accountNumber);
    printf("\t Date       Time     Amount      Activity   Balance\n\n");

    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        int loggedAccountNumber;
        char date[20], time[20], activity[20];
        float amount, balance;

        // Parse the transaction log line
        sscanf(line, "%s %s %d %f %s %f", date, time, &loggedAccountNumber, &amount, activity, &balance);

        // Display only transactions for the logged-in user
        if (loggedAccountNumber == user->accountNumber)
        {
            printf("\t%s %s %.2f \t %s  %.2f\n", date, time, amount, activity, balance);
        }
    }
    continueKey();
}

// Function to log transactions
void logTransaction(int accountNumber, const char *type, float amount, float balance)
{

    FILE *file = fopen("transaction_log.txt", "a");
    header();

    if (file == NULL)
    {
        printf("\tError opening transaction log file.\n");
        continueKey();
        return;
    }

    // Get current time
    time_t now;
    time(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

    // Log the transaction
    fprintf(file, "%s %d %.2f %s %.2f\n", timestamp, accountNumber, amount, type, balance);

    fclose(file);
}

// Function to update user balance in the file
void updateUserBalance(struct User *user)
{
    FILE *file = fopen("userdetail.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");
    header();

    if (file == NULL || tempFile == NULL)
    {
        printf("\tError updating user balance.\n");
        return;
    }

    struct User tempUser;
    int userFound = 0;
    while (fscanf(file, "%d %s %s %s %f %s %s %s\n", &tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, &tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email) != EOF)
    {
        if (tempUser.accountNumber == user->accountNumber)
        {
            tempUser.balance = user->balance;
            userFound = 1; // User found
        }
        fprintf(tempFile, "%d %s %s %s %.2f %s %s %s\n", tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email);
    }

    fclose(file);
    fclose(tempFile);

    // Checking if user is found?
    if (!userFound)
    {
        printf("\tUser not found. Balance update failed.\n");
        remove("temp.txt"); // Remove temp file if user not found
        return;
    }

    remove("userdetail.txt");
    rename("temp.txt", "userdetail.txt");
}

// Press any key to continue
void continueKey()
{
    printf("\n\tPress any key to continue.");
    getch();
}

// Buffer Flush
void Bufferflush()
{
    // Buffer Flush
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

// Organization's Header                     --Ready
void header()
{
    system("cls");
    printf("\n");
    printf("\t\t################################################\n");
    printf("\t\t#                                              #\n");
    printf("\t\t#        ***  WELCOME TO NKB BANK  ***         #\n");
    printf("\t\t#                                    Pvt. Ltd. #\n");
    printf("\t\t################################################\n");
    printf("\n\t-------------------------------------------------------\n");
}
// Exit animation
void exitAnimation()
{
    system("cls");

    printf("\n\n\n\n\n\n\n\n");

    printf("\t\t         THANK YOU FOR USING NKB BANK           \n");
    printf("\t\t                                                \n");
    printf("\t\t        Visit Us Again For More Services        \n\n");
    printf("\t\t        For more information, contact us at     \n");
    printf("\t\t         +234 813 123 4569                      \n");
    printf("\t\t         Email: nkb.bank@gmail.com              \n");
    printf("\t\t         Website: nkbbank.com                   \n\n");
    printf("\t#######################################################################\n");

    Sleep(3);
    printf("\n\n(Press any key to exit.)");
    getch();
    system("cls"); // Wait before closing
    exit(0);
}

// boot graphics
void boot()
{
    printf("\n\n\n\n");
    printf("\t\tNNNN    N  K   K  BBBBB      BBBBB    AAAAA  NNNN    N  K   K\n");
    printf("\t\tN   N   N  K  K   B    B     B    B   A   A  N   N   N  K  K\n");
    printf("\t\tN    N  N  KK     BBBBB      BBBBB    AAAAA  N    N  N  KK\n");
    printf("\t\tN     N N  K  K   B    B     B    B   A   A  N     N N  K  K\n");
    printf("\t\tN      NN  K   K  BBBBB      BBBBB    A   A  N      NN  K   K\n");
    printf("\n\n\t\t\t\t\tWELCOMES YOU!\n\n");
    Sleep(100);
    printf("\t\t");

    for (int i = 0; i < 62; i++)
    {
        printf("_");
        Sleep(7);
    }
    Sleep(100);
}
// for login and create account
void encryptPassword(char *password, int key)
{
    char ch;
    int i = 0;
    if (key == 1)
    {
        printf("\tEnter valid and strong password: ");
    }
    else if (key == 2)
    {
        printf("\tEnter current password: ");
    }
    else if (key == 3)
    {
        printf("\n\tEnter new password: ");
    }
    else if (key == 4)
    {
        printf("\tEnter your password: ");
    }
    else
    {
        printf("\n\tEnter cofirm password: ");
    }
    while (1)
    {
        ch = getch(); // Read character without displaying it
        if (ch == 13) // Enter key (ASCII 13)
            break;
        if (ch == 8 && i > 0)
        {                    // Handle backspace
            printf("\b \b"); // Move cursor back, print space, move back again
            i--;
            continue;
        }
        password[i++] = ch;
        printf("*"); // Show '*' instead of actual character
    }
    password[i] = '\0'; // Null-terminate string
}

int isValidPassword(char *password)
{
    int length = 0, hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;
    char specialChars[] = "@$!#*?&";

    length = strlen(password);
    if (length < 8)
        return 0; // Password must be at least 8 characters long

    for (int i = 0; i < length; i++)
    {
        if (isupper(password[i]))
            hasUpper = 1;
        if (islower(password[i]))
            hasLower = 1;
        if (isdigit(password[i]))
            hasDigit = 1;
        if (strchr(specialChars, password[i])) // check the character in string and return pointer if found
            hasSpecial = 1;
    }

    return (hasUpper && hasLower && hasDigit && hasSpecial); // logic AND (&&), Return 1 if every values has 1
}
int isValidUsername(char *username)
{
    int length = strlen(username);

    // Check length (5-15 characters)
    if (length < 5 || length > 30)
    {
        return 0;
    }

    // Check if all characters are alphabetic (A-Z, a-z)
    for (int i = 0; i < length; i++)
    {
        if (!isalpha(username[i]))
        {
            return 0; // Invalid username
        }

        return 1; // Valid username
    }
}
int isValidDateFormat(char *date)
{
    if (strlen(date) != 10) // Check length
        return 0;
    for (int i = 0; i < 10; i++) // Check if characters are digits or '-'
    {
        if ((i == 4 || i == 7))
        {
            if (date[i] != '-')
                return 0; // Ensure dashes at the right places
        }
        else
        {
            if (!isdigit(date[i]))
                return 0; // Ensure numbers only
        }
    }
    return 1; // Valid format
}
int isValidPhoneNumber(char *phoneNumber)
{
    // Check if the phone number has exactly 10 digits
    if (strlen(phoneNumber) != 10)
    {
        return 0; // Invalid if length is not 10
    }

    // Check if the first two digits are 97 or 98
    if (phoneNumber[0] != '9' || (phoneNumber[1] != '7' && phoneNumber[1] != '8'))
    {
        return 0; // Invalid if it does not start with 97 or 98
    }

    // Check if all characters are digits
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(phoneNumber[i]))
        {
            return 0; // Invalid if any character is not a digit
        }
    }

    return 1; // Valid phone number
}
int isValidEmail(char *email)
{
    int count_Atrate = 0, count_Dot = 0;
    int len = strlen(email); // Get the length of the email
    for (int i = 0; i < len; i++)
    {
        if (email[i] == '@')
            count_Atrate++;
        if (email[i] == '.')
            count_Dot++;
    }
    return (count_Atrate && count_Dot); //&&-> logical AND //It will return 1 if both counts has 1 value
}
void lowercase(char str[])
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = tolower(str[i]);
    }
}
void viewinfo(struct User *user)
{
    system("cls");
    header();
    printf("\t\t\t** Personal details ***\n");
    printf("\tAccount Number: %d\n", user->accountNumber);
    printf("\tName: %s\n", user->username);
    printf("\tPhone no.: %s\n", user->phone);
    printf("\tPassword: %s\n", user->password);
    printf("\tEmail: %s\n", user->email);
    printf("\tBalance: NPR %.2f\n", user->balance);
    printf("\tDOB: %s BS\n", user->dateOfBirth);
    printf("\tAddress: %s\n", user->address);
    printf("\tEmail: %s\n\n", user->email);
    // Sleep(3);
    continueKey();
}
void changePassword(struct User *user)
{
    system("cls");
    char currentPassword[50], newPassword[50], confirmPassword[50];
    FILE *file, *tempFile;
    struct User tempUser;
    int found = 0;

    header();
    // Getting current password from user
    encryptPassword(currentPassword, 2); // key==2
    if (strcmp(user->password, currentPassword) != 0)
    {
        printf("\tIncorrect password!\n");
        continueKey();
        return;
    }
    while (1)
    {
        // Getting new password from user
        encryptPassword(newPassword, 3); // key==3
        if (!isValidPassword(newPassword))
        {
            printf("\tError! Too weak password. Try again.\n\tNote: Password should be at least 8 characters long and contain at least 1 uppercase, 1 lowercase, 1 digit, and 1 special character.\n");
            continue;
        }

        // Getting confirm password from user
        encryptPassword(confirmPassword, 5); // key==4
        if (strcmp(newPassword, confirmPassword) != 0)
        {
            printf("\tPasswords do not match! Try again.\n");
        }
        else
        {
            break;
        }
    }
    file = fopen("userdetail.txt", "r");
    tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("\tError updating password.\n");
        return;
    }

    while (fscanf(file, "%d %s %s %s %f %s %s %s\n", &tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, &tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email) == 8)
    {
        if (tempUser.accountNumber == user->accountNumber)
        {
            strcpy(tempUser.password, newPassword);
            strcpy(user->password, newPassword);
            found = 1;
        }
        fprintf(tempFile, "%d %s %s %s %.2f %s %s %s\n", tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email);
    }

    fclose(file);
    fclose(tempFile);

    if (!found)
    {
        printf("\tUser not found. Password update failed.\n");
        remove("temp.txt");
    }
    else
    {
        remove("userdetail.txt");
        rename("temp.txt", "userdetail.txt");
        printf("\n\tPassword changed successfully!\n");
    }

    continueKey();
}
void changeEmail(struct User *user)
{
    system("cls");
    char currentEmail[50], newEmail[50], confirmEmail[50];
    FILE *file, *f, *tempFile;
    struct User tempUser, temp;
    int found = 0;
    header();

    printf("\tEnter your current Email: ");
    scanf("%s", currentEmail);
    Bufferflush();
    if (strcmp(user->email, currentEmail) != 0)
    {
        printf("\tIncorrect Email!\n");
        continueKey();
        return;
    }
    while (1)
    {
        while (1)
        {

            printf("\tEnter new Email: ");
            scanf("%s", newEmail);
            Bufferflush();

            if (!isValidEmail(newEmail))
            {
                printf("Error ! due to invalid email format\n");
                continue;
            }

            // Check if email already exists for other account number
            f = fopen("userdetail.txt", "r");
            int emailExists = 0;
            if (f != NULL)
            {
                while (fscanf(f, "%d %s %s %s %f %s %s %s\n", &temp.accountNumber, temp.username, temp.phone, temp.password, &temp.balance, temp.dateOfBirth, temp.address, temp.email) != EOF)
                {
                    if (strcmp(temp.email, newEmail) == 0)
                    {
                        emailExists = 1;
                        break;
                    }
                }
                fclose(f);
            }

            if (emailExists)
            {
                printf("Error ! An account with this email already exists.\n");
            }
            else
            {
                break;
            }
        }
        printf("\tConfirm new Email: ");
        scanf("%s", confirmEmail);
        Bufferflush();

        if (strcmp(newEmail, confirmEmail) != 0)
        {
            printf("Error! Email do not match! Try again.\n");
            continue;
        }
        else
        {

            break;
        }
    }
    // Updating the new email in userdetail.txt
    file = fopen("userdetail.txt", "r");
    tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("\tError updating Email.\n");
        return;
    }

    while (fscanf(file, "%d %s %s %s %f %s %s %s\n", &tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, &tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email) == 8)
    {
        if (tempUser.accountNumber == user->accountNumber)
        {
            strcpy(tempUser.email, newEmail);
            strcpy(user->email, newEmail);
            found = 1;
        }
        fprintf(tempFile, "%d %s %s %s %.2f %s %s %s\n", tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email);
    }

    fclose(file);
    fclose(tempFile);

    if (!found)
    {
        printf("\tUser not found. Email update failed.\n");
        remove("temp.txt");
    }
    else
    {
        remove("userdetail.txt");
        rename("temp.txt", "userdetail.txt");
        printf("\tEmail changed successfully!\n");
    }

    continueKey();
}
void changePhoneNumber(struct User *user)
{
    system("cls");
    char currentPhone[50], newPhone[50], confirmPhone[50];
    FILE *file, *f, *tempFile;
    struct User tempUser, temp;
    int found = 0;
    header();

    printf("\tEnter your current PhoneNumber: ");
    scanf("%s", currentPhone);
    Bufferflush();
    if (strcmp(user->phone, currentPhone) != 0)
    {
        printf("\tIncorrect PhoneNumber!\n");
        continueKey();
        return;
    }
    while (1)
    {
        while (1)
        {

            printf("\tEnter new PhoneNumber: ");
            scanf("%s", newPhone);
            Bufferflush();

            if (!isValidPhoneNumber(newPhone))
            {
                printf("Error ! due to invalid Phone number format\n");
                continue;
            }

            // Check if phone number already exists
            f = fopen("userdetail.txt", "r");
            int phoneExists = 0;
            if (f != NULL)
            {
                while (fscanf(f, "%d %s %s %s %f %s %s %s\n", &temp.accountNumber, temp.username, temp.phone, temp.password, &temp.balance, temp.dateOfBirth, temp.address, temp.email) != EOF)
                {
                    if (strcmp(temp.phone, newPhone) == 0)
                    {
                        phoneExists = 1;
                        break;
                    }
                }
                fclose(f);
            }

            if (phoneExists)
            {
                printf("Error! An account with this phone number already exists.\n");
            }
            else
            {
                break;
            }
        }
        printf("\tConfirm new Phone: ");
        scanf("%s", confirmPhone);
        Bufferflush();

        if (strcmp(newPhone, confirmPhone) != 0)
        {

            printf("\tPhoneNumber do not match! Try again.\n");
            continue;
        }
        else
        {
            break;
        }
    }
    // updating new phone number in userdetail.txt
    file = fopen("userdetail.txt", "r");
    tempFile = fopen("temp.txt", "w");
    if (file == NULL || tempFile == NULL)
    {
        printf("\tError updating Phone Number.\n");
        return;
    }

    while (fscanf(file, "%d %s %s %s %f %s %s %s\n", &tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, &tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email) == 8)
    {
        if (tempUser.accountNumber == user->accountNumber)
        {
            strcpy(tempUser.phone, newPhone);
            strcpy(user->phone, newPhone);
            found = 1;
        }
        fprintf(tempFile, "%d %s %s %s %.2f %s %s %s\n", tempUser.accountNumber, tempUser.username, tempUser.phone, tempUser.password, tempUser.balance, tempUser.dateOfBirth, tempUser.address, tempUser.email);
    }

    fclose(file);
    fclose(tempFile);

    if (!found)
    {
        printf("\tUser not found. Phone Number update failed.\n");
        remove("temp.txt");
    }
    else
    {
        remove("userdetail.txt");
        rename("temp.txt", "userdetail.txt");
        printf("\tPhone Number changed successfully!\n");
    }

    continueKey();
}
