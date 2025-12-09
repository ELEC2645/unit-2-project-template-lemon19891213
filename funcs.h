#ifndef FUNCS_H
#define FUNCS_H

/* Menu item functions */
void menu_item_1(void);
void menu_item_2(void);
void menu_item_3(void);
void menu_item_4(void);

/* Helper functions */
double get_float(const char *prompt);  /* Get valid float input */
int is_float(const char *s);           /* Validate float input */
int is_integer(const char *s);         /* Validate integer input */

/* Function implementations */
void arithmetic_calc(void);            /* Basic arithmetic operations */
void trig_calc(void);                  /* Trigonometric functions */
void log_calc(void);                   /* Logarithmic functions */
void unit_convert(void);               /* Unit conversion */

#endif