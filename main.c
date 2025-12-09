#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

static void main_menu(void);            /* Main loop menu */
static void print_main_menu(void);      /* Print main menu */
static int  get_user_input(void);       /* Get valid menu selection (integer) */
static void select_menu_item(int input);/* Execute function based on selection */
static void go_back_to_main(void);      /* Wait to return to main menu */

int main(void)
{
    /* Infinite loop until exit is selected */
    for(;;) {
        main_menu();
    }
    return 0;
}

static void main_menu(void)
{
    print_main_menu();
    int input = get_user_input();
    select_menu_item(input);
}

static int get_user_input(void)
{
    enum { MENU_ITEMS = 5 };   /* 1-4: functions, 5: Exit */
    char buf[128];
    int valid_input = 0;
    int value = 0;

    do {
        printf("\nSelect function (enter number 1-5): ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }

        buf[strcspn(buf, "\r\n")] = '\0'; /* Remove newline */

        if (!is_integer(buf)) {
            printf("Invalid input! Please enter an integer.\n");
            valid_input = 0;
        } else {
            value = (int)strtol(buf, NULL, 10);
            if (value >= 1 && value <= MENU_ITEMS) {
                valid_input = 1;
            } else {
                printf("Invalid input! Please select a number between 1-5.\n");
                valid_input = 0;
            }
        }
    } while (!valid_input);

    return value;
}

static void select_menu_item(int input)
{
    switch (input) {
        case 1:
            menu_item_1();
            go_back_to_main();
            break;
        case 2:
            menu_item_2();
            go_back_to_main();
            break;
        case 3:
            menu_item_3();
            go_back_to_main();
            break;
        case 4:
            menu_item_4();
            go_back_to_main();
            break;
        default:
            printf("\nThank you for using. Goodbye!\n");
            exit(0);
    }
}

static void print_main_menu(void)
{
    printf("\n=========== Scientific Calculator ===========");
    printf("\n\t1. Basic Arithmetic (Add, Subtract, Multiply, Divide, Modulus, Power)");
    printf("\n\t2. Trigonometric Functions (sin, cos, tan; Degree/Radian support)");
    printf("\n\t3. Logarithmic Functions (Natural log, Common log, Custom base)");
    printf("\n\t4. Unit Conversion (Length, Temperature, Weight)");
    printf("\n\t5. Exit");
    printf("\n=============================================");
}

static void go_back_to_main(void)
{
    char buf[64];
    do {
        printf("\nEnter 'b' or 'B' to return to main menu: ");
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0'; /* Remove newline */
    } while (!(buf[0] == 'b' || buf[0] == 'B') || buf[1] != '\0');
}