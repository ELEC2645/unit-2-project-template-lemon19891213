#define _GNU_SOURCE  /* Enable M_PI definition */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "funcs.h"

/* Validate if string is an integer (optional +/- followed by digits) */
int is_integer(const char *s)
{
    if (!s || !*s) return 0;

    /* Optional sign */
    if (*s == '+' || *s == '-') s++;

    /* Must have at least one digit */
    if (!isdigit((unsigned char)*s)) return 0;

    /* Subsequent characters must be digits */
    while (*s) {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

/* Validate if string is a float (optional +/- , one decimal point, digits) */
int is_float(const char *s)
{
    if (!s || !*s) return 0;

    /* Optional sign */
    if (*s == '+' || *s == '-') s++;

    int has_digit = 0;
    int has_dot = 0;

    while (*s) {
        if (isdigit((unsigned char)*s)) {
            has_digit = 1;
        } else if (*s == '.' && !has_dot) {
            has_dot = 1;
        } else {
            return 0; /* Invalid character */
        }
        s++;
    }

    return has_digit; /* Must contain at least one digit */
}

/* Get valid float input with prompt */
double get_float(const char *prompt)
{
    char buf[128];
    double value = 0.0;
    int valid = 0;

    do {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("\nInput error. Exiting.");
            exit(1);
        }
        buf[strcspn(buf, "\r\n")] = '\0'; /* Remove newline */

        if (is_float(buf)) {
            value = strtod(buf, NULL);
            valid = 1;
        } else {
            printf("Invalid input! Please enter a valid float number.\n");
        }
    } while (!valid);

    return value;
}

/* Basic arithmetic operations: +, -, *, /, %, ^ */
void arithmetic_calc(void)
{
    double a, b, result;
    char op;
    char buf[16];

    printf("\n===== Basic Arithmetic =====\n");
    printf("Supported operators: + (Add), - (Subtract), * (Multiply), / (Divide), %% (Modulus, integer only), ^ (Power)\n");

    /* Get operands */
    a = get_float("Enter first number: ");
    b = get_float("Enter second number: ");

    /* Get operator */
    do {
        printf("Enter operator (+, -, *, /, %, ^): ");
        if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
        buf[strcspn(buf, "\r\n")] = '\0';

        if (strlen(buf) != 1) {
            printf("Invalid input! Please enter a single operator.\n");
            op = 0;
        } else {
            op = buf[0];
            if (op != '+' && op != '-' && op != '*' && op != '/' && op != '%' && op != '^') {
                printf("Unsupported operator! Please try again.\n");
                op = 0;
            }
        }
    } while (op == 0);

    /* Calculation and error handling */
    int error = 0;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/':
            if (fabs(b) < 1e-9) { /* Avoid division by zero */
                printf("Error: Division by zero is not allowed!\n");
                error = 1;
            } else {
                result = a / b;
            }
            break;
        case '%':
            if (fabs(b) < 1e-9) {
                printf("Error: Division by zero is not allowed!\n");
                error = 1;
            } else {
                result = (int)a % (int)b; /* Modulus only for integers */
            }
            break;
        case '^':
            result = pow(a, b);
            if (isnan(result) || isinf(result)) { /* Invalid power operation (e.g., negative^non-integer) */
                printf("Error: Invalid power operation!\n");
                error = 1;
            }
            break;
        default: error = 1;
    }

    if (!error) {
        printf("Result: %.6f %c %.6f = %.6f\n", a, op, b, result);
    }
}

/* Trigonometric functions: sin, cos, tan (supports Degree/Radian) */
void trig_calc(void)
{
    double value, result;
    int func_choice, unit_choice;
    char buf[128];

    printf("\n===== Trigonometric Functions =====\n");
    printf("Supported functions: 1.Sine(sin)  2.Cosine(cos)  3.Tangent(tan)\n");

    /* Select function */
    do {
        printf("Select function (1-3): ");
        if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
        buf[strcspn(buf, "\r\n")] = '\0';
        func_choice = is_integer(buf) ? atoi(buf) : 0;
        if (func_choice < 1 || func_choice > 3) {
            printf("Invalid selection! Please enter 1-3.\n");
            func_choice = 0;
        }
    } while (func_choice == 0);

    /* Select unit */
    do {
        printf("Select input unit: 1.Degree(°)  2.Radian(rad)\n");
        printf("Selection (1-2): ");
        if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
        buf[strcspn(buf, "\r\n")] = '\0';
        unit_choice = is_integer(buf) ? atoi(buf) : 0;
        if (unit_choice < 1 || unit_choice > 2) {
            printf("Invalid selection! Please enter 1-2.\n");
            unit_choice = 0;
        }
    } while (unit_choice == 0);

    /* Get input value and convert to radians (required by math.h) */
    value = get_float("Enter value: ");
    if (unit_choice == 1) {
        value = value * M_PI / 180.0; /* Degree to Radian conversion */
    }

    /* Calculation and error handling */
    int error = 0;
    const char *func_name[] = {"sin", "cos", "tan"};
    switch (func_choice) {
        case 1: result = sin(value); break;
        case 2: result = cos(value); break;
        case 3:
            /* Avoid tan singularity (π/2 + kπ) */
            if (fabs(fmod(value, M_PI) - M_PI/2) < 1e-6) {
                printf("Error: Tangent is undefined at this point!\n");
                error = 1;
            } else {
                result = tan(value);
            }
            break;
        default: error = 1;
    }

    if (!error) {
        printf("Result: %s(%.6f %s) = %.6f\n",
               func_name[func_choice-1],
               unit_choice == 1 ? (value*180/M_PI) : value,
               unit_choice == 1 ? "°" : "rad",
               result);
    }
}

/* Logarithmic functions: ln, log10, custom base */
void log_calc(void)
{
    double x, base, result;
    int func_choice;
    char buf[128];

    printf("\n===== Logarithmic Functions =====\n");
    printf("Supported functions: 1.Natural Log(ln)  2.Common Log(log10)  3.Custom Base Log\n");

    /* Select function */
    do {
        printf("Select function (1-3): ");
        if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
        buf[strcspn(buf, "\r\n")] = '\0';
        func_choice = is_integer(buf) ? atoi(buf) : 0;
        if (func_choice < 1 || func_choice > 3) {
            printf("Invalid selection! Please enter 1-3.\n");
            func_choice = 0;
        }
    } while (func_choice == 0);

    /* Get argument (must be > 0) */
    do {
        x = get_float("Enter argument x (x > 0): ");
        if (x <= 0) {
            printf("Error: Argument must be greater than 0! Please try again.\n");
        }
    } while (x <= 0);

    /* Calculation */
    int error = 0;
    switch (func_choice) {
        case 1: result = log(x); break; /* Natural log (ln) */
        case 2: result = log10(x); break; /* Common log (log10) */
        case 3:
            /* Get base (must be > 0 and != 1) */
            do {
                base = get_float("Enter base (base > 0 and base != 1): ");
                if (base <= 0) {
                    printf("Error: Base must be greater than 0! Please try again.\n");
                } else if (fabs(base - 1.0) < 1e-6) {
                    printf("Error: Base cannot be 1! Please try again.\n");
                }
            } while (base <= 0 || fabs(base - 1.0) < 1e-6);
            result = log(x) / log(base); /* Change of base formula */
            break;
        default: error = 1;
    }

    if (!error) {
        if (func_choice == 1) {
            printf("Result: ln(%.6f) = %.6f\n", x, result);
        } else if (func_choice == 2) {
            printf("Result: log10(%.6f) = %.6f\n", x, result);
        } else {
            printf("Result: log_%.6f(%.6f) = %.6f\n", base, x, result);
        }
    }
}

/* Unit conversion: Length, Temperature, Weight */
void unit_convert(void)
{
    int type_choice, from_unit, to_unit;
    double value, result;
    char buf[128];

    printf("\n===== Unit Conversion =====\n");
    printf("Supported types: 1.Length  2.Temperature  3.Weight\n");

    /* Select conversion type */
    do {
        printf("Select conversion type (1-3): ");
        if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
        buf[strcspn(buf, "\r\n")] = '\0';
        type_choice = is_integer(buf) ? atoi(buf) : 0;
        if (type_choice < 1 || type_choice > 3) {
            printf("Invalid selection! Please enter 1-3.\n");
            type_choice = 0;
        }
    } while (type_choice == 0);

    value = get_float("Enter value to convert: ");

    /* Length conversion (Meter, Centimeter, Inch, Foot) */
    if (type_choice == 1) {
        const char *units[] = {"Meter(m)", "Centimeter(cm)", "Inch(in)", "Foot(ft)"};
        printf("\nLength units: 1.Meter  2.Centimeter  3.Inch  4.Foot\n");
        
        /* Select source unit */
        do {
            printf("Select source unit (1-4): ");
            if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
            buf[strcspn(buf, "\r\n")] = '\0';
            from_unit = is_integer(buf) ? atoi(buf) : 0;
            if (from_unit < 1 || from_unit > 4) {
                printf("Invalid selection! Please enter 1-4.\n");
            }
        } while (from_unit < 1 || from_unit > 4);
        
        /* Select target unit */
        do {
            printf("Select target unit (1-4): ");
            if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
            buf[strcspn(buf, "\r\n")] = '\0';
            to_unit = is_integer(buf) ? atoi(buf) : 0;
            if (to_unit < 1 || to_unit > 4) {
                printf("Invalid selection! Please enter 1-4.\n");
            }
        } while (to_unit < 1 || to_unit > 4);

        /* Convert to Centimeter as intermediate unit */
        double cm;
        switch (from_unit) {
            case 1: cm = value * 100; break;
            case 2: cm = value; break;
            case 3: cm = value * 2.54; break;
            case 4: cm = value * 30.48; break;
        }

        /* Convert to target unit */
        switch (to_unit) {
            case 1: result = cm / 100; break;
            case 2: result = cm; break;
            case 3: result = cm / 2.54; break;
            case 4: result = cm / 30.48; break;
        }

        printf("Result: %.6f %s = %.6f %s\n", value, units[from_unit-1], result, units[to_unit-1]);
    }

    /* Temperature conversion (Celsius, Fahrenheit, Kelvin) */
    else if (type_choice == 2) {
        const char *units[] = {"Celsius(°C)", "Fahrenheit(°F)", "Kelvin(K)"};
        printf("\nTemperature units: 1.Celsius  2.Fahrenheit  3.Kelvin\n");
        
        /* Select source unit */
        do {
            printf("Select source unit (1-3): ");
            if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
            buf[strcspn(buf, "\r\n")] = '\0';
            from_unit = is_integer(buf) ? atoi(buf) : 0;
            if (from_unit < 1 || from_unit > 3) {
                printf("Invalid selection! Please enter 1-3.\n");
            }
        } while (from_unit < 1 || from_unit > 3);
        
        /* Select target unit */
        do {
            printf("Select target unit (1-3): ");
            if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
            buf[strcspn(buf, "\r\n")] = '\0';
            to_unit = is_integer(buf) ? atoi(buf) : 0;
            if (to_unit < 1 || to_unit > 3) {
                printf("Invalid selection! Please enter 1-3.\n");
            }
        } while (to_unit < 1 || to_unit > 3);

        /* Convert to Celsius as intermediate unit */
        double c;
        switch (from_unit) {
            case 1: c = value; break;
            case 2: c = (value - 32) * 5 / 9; break;
            case 3: c = value - 273.15; break;
        }

        /* Convert to target unit */
        switch (to_unit) {
            case 1: result = c; break;
            case 2: result = c * 9 / 5 + 32; break;
            case 3: result = c + 273.15; break;
        }

        printf("Result: %.6f %s = %.6f %s\n", value, units[from_unit-1], result, units[to_unit-1]);
    }

    /* Weight conversion (Kilogram, Gram, Pound) */
    else if (type_choice == 3) {
        const char *units[] = {"Kilogram(kg)", "Gram(g)", "Pound(lb)"};
        printf("\nWeight units: 1.Kilogram  2.Gram  3.Pound\n");
        
        /* Select source unit */
        do {
            printf("Select source unit (1-3): ");
            if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
            buf[strcspn(buf, "\r\n")] = '\0';
            from_unit = is_integer(buf) ? atoi(buf) : 0;
            if (from_unit < 1 || from_unit > 3) {
                printf("Invalid selection! Please enter 1-3.\n");
            }
        } while (from_unit < 1 || from_unit > 3);
        
        /* Select target unit */
        do {
            printf("Select target unit (1-3): ");
            if (!fgets(buf, sizeof(buf), stdin)) { exit(1); }
            buf[strcspn(buf, "\r\n")] = '\0';
            to_unit = is_integer(buf) ? atoi(buf) : 0;
            if (to_unit < 1 || to_unit > 3) {
                printf("Invalid selection! Please enter 1-3.\n");
            }
        } while (to_unit < 1 || to_unit > 3);

        /* Convert to Gram as intermediate unit */
        double g;
        switch (from_unit) {
            case 1: g = value * 1000; break;
            case 2: g = value; break;
            case 3: g = value * 453.592; break; /* 1 Pound ≈ 453.592 Grams */
        }

        /* Convert to target unit */
        switch (to_unit) {
            case 1: result = g / 1000; break;
            case 2: result = g; break;
            case 3: result = g / 453.592; break;
        }

        printf("Result: %.6f %s = %.6f %s\n", value, units[from_unit-1], result, units[to_unit-1]);
    }
}

/* Menu item functions (call corresponding implementations) */
void menu_item_1(void) { arithmetic_calc(); }
void menu_item_2(void) { trig_calc(); }
void menu_item_3(void) { log_calc(); }
void menu_item_4(void) { unit_convert(); }