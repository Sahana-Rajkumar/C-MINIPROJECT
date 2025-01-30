#include <stdio.h>
#include <string.h>

#define RESET "\x1B[0m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"

char notes[12][31][100] = {{{0}}}; // Array to store notes for each date

// Function that returns the index of the day for date DD/MM/YYYY
int dayNumber(int day, int month, int year) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    year -= month < 3;
    return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}

// Function to return the name of the month for the given month number
char *getMonthName(int monthNumber) {
    static char *months[] = {"January", "February", "March", "April", "May",
                             "June", "July", "August", "September", "October",
                             "November", "December"};
    return months[monthNumber];
}

// Function to return the number of days in a month
int numberOfDays(int monthNumber, int year) {
    if (monthNumber == 1) { // February
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
            return 29;
        else
            return 28;
    }
    if (monthNumber == 3 || monthNumber == 5 || monthNumber == 8 || monthNumber == 10)
        return 30;
    return 31;
}

// Function to print the calendar for a specific month
void printMonthCalendar(int year, int month) {
    printf("\n ------------%s-------------\n", getMonthName(month - 1));
    printf(" Sun    Mon    Tue    Wed    Thu    Fri    Sat\n");

    int days = numberOfDays(month - 1, year);
    int current = dayNumber(1, month, year);

    int k;
    for (k = 0; k < current; k++)
        printf("     ");

    for (int j = 1; j <= days; j++) {
        if (k % 7 == 0) { // Sunday in GREEN
            if (notes[month - 1][j - 1][0] != '\0') // Notes in YELLOW
                printf(YELLOW "%5d" RESET, j);
            else
                printf(GREEN "%5d" RESET, j);
        } else if (notes[month - 1][j - 1][0] != '\0') { // Notes in YELLOW
            printf(YELLOW "%5d" RESET, j);
        } else {
            printf("%5d", j);
        }

        if (++k > 6) {
            k = 0;
            printf("\n");
        }
    }
    if (k)
        printf("\n");

    // Print notes for the month
    printf("\nNotes for %s:\n", getMonthName(month - 1));
    for (int day = 0; day < 31; day++) {
        if (notes[month - 1][day][0] != '\0') {
            printf("  %2d: %s\n", day + 1, notes[month - 1][day]);
        }
    }
}

// Function to save the calendar for selected months to a file
void saveSelectedMonthsToFile(int year, int *months, int count) {
    // Create a filename for the selected months
    char filename[100] = "calendar_";
    for (int i = 0; i < count; i++) {
        char monthName[20];
        sprintf(monthName, "%s_", getMonthName(months[i] - 1));
        strcat(filename, monthName);
    }
    sprintf(filename + strlen(filename) - 1, "_%d.txt", year); // Add year to the filename

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to save file.\n");
        return;
    }

    fprintf(file, "     Calendar - Selected Months of %d\n\n", year);

    for (int i = 0; i < count; i++) {
        int month = months[i];
        fprintf(file, "\n ------------%s-------------\n", getMonthName(month - 1));
        fprintf(file, " Sun Mon Tue Wed Thu Fri Sat\n");

        int days = numberOfDays(month - 1, year);
        int current = dayNumber(1, month, year);

        int k;
        for (k = 0; k < current; k++)
            fprintf(file, "     ");

        for (int j = 1; j <= days; j++) {
            if (k % 7 == 0) { // Sunday
                if (notes[month - 1][j - 1][0] != '\0') // Notes
                    fprintf(file, "%5d*", j);
                else
                    fprintf(file, "%5d", j);
            } else if (notes[month - 1][j - 1][0] != '\0') { // Notes
                fprintf(file, "%5d*", j);
            } else {
                fprintf(file, "%5d", j);
            }

            if (++k > 6) {
                k = 0;
                fprintf(file, "\n");
            }
        }
        if (k)
            fprintf(file, "\n");

        // Append notes for the current month
        fprintf(file, "\nNotes for %s:\n", getMonthName(month - 1));
        for (int day = 0; day < 31; day++) {
            if (notes[month - 1][day][0] != '\0') {
                fprintf(file, "  %2d: %s\n", day + 1, notes[month - 1][day]);
            }
        }
    }

    fclose(file);
    printf("Calendar for selected months saved to %s!\n", filename);
}

// Function to add notes
void addNote() {
    int month, day;
    char note[100];

    printf("Enter month (1-12): ");
    scanf("%d", &month);
    printf("Enter day (1-31): ");
    scanf("%d", &day);
    printf("Enter note: ");
    scanf(" %[^\n]s", note);

    strcpy(notes[month - 1][day - 1], note);
    printf("Note added successfully!\n");
}

// Function to save full calendar
void saveFullYearToFile(int year) {
    char filename[50];
    sprintf(filename, "calendar_%d.txt", year);

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Unable to save file.\n");
        return;
    }

    fprintf(file, "     Full Year Calendar - %d\n\n", year);

    for (int month = 1; month <= 12; month++) {
        fprintf(file, "\n ------------%s-------------\n", getMonthName(month - 1));
        fprintf(file, " Sun Mon Tue Wed Thu Fri Sat\n");

        int days = numberOfDays(month - 1, year);
        int current = dayNumber(1, month, year);

        int k;
        for (k = 0; k < current; k++)
            fprintf(file, "     ");

        for (int j = 1; j <= days; j++) {
            if (k % 7 == 0) { // Sunday
                if (notes[month - 1][j - 1][0] != '\0') // Notes
                    fprintf(file, "%5d*", j);
                else
                    fprintf(file, "%5d", j);
            } else if (notes[month - 1][j - 1][0] != '\0') { // Notes
                fprintf(file, "%5d*", j);
            } else {
                fprintf(file, "%5d", j);
            }

            if (++k > 6) {
                k = 0;
                fprintf(file, "\n");
            }
        }
        if (k)
            fprintf(file, "\n");

        // Append notes for the current month
        fprintf(file, "\nNotes for %s:\n", getMonthName(month - 1));
        for (int day = 0; day < 31; day++) {
            if (notes[month - 1][day][0] != '\0') {
                fprintf(file, "  %2d: %s\n", day + 1, notes[month - 1][day]);
            }
        }
    }

    fclose(file);
    printf("Full year calendar saved to %s!\n", filename);
}
// Main function
int main() {
    int year, choice, month;

    printf("Calendar Application\n");
    printf("Enter the year: ");
    scanf("%d", &year);

    do {
        printf("\nMenu:\n");
        printf("1. Display full year calendar\n");
        printf("2. Display specific month calendar\n");
        printf("3. Add a note\n");
        printf("4. Show notes for a month\n");
        printf("5. Save calendar for specific months\n");
        printf("6. Save full calendar\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            for (int i = 1; i <= 12; i++)
                printMonthCalendar(year, i);
            break;
        case 2:
            printf("Enter month (1-12): ");
            scanf("%d", &month);
            printMonthCalendar(year, month);
            break;
        case 3:
            addNote();
            break;
        case 4:
            printf("Enter month (1-12): ");
            scanf("%d", &month);
            printMonthCalendar(year, month); // Show notes included in month view
            break;
        case 5: {
            int count;
            printf("How many months do you want to save? ");
            scanf("%d", &count);

            int months[count];
            printf("Enter the months (1-12): ");
            for (int i = 0; i < count; i++) {
                scanf("%d", &months[i]);
            }

            saveSelectedMonthsToFile(year, months, count);
            break;
        }
        case 6:
        	 saveFullYearToFile(year);
                break;
        case 7:
           	 printf("Exiting...\n");
                break;
        default:
            printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}


