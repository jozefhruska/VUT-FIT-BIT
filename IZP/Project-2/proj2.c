/*
 Project created by: Jozef Hruška (xhrusk25)
*/

#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>

#define ERR_BASE "ERROR: "
#define ERR_BAD_FORMAT ERR_BASE "Use of badly formatted arguments. Use --help for user manual.\n"
#define ERR_SOMETHING ERR_BASE "Oops! Something went wrong. Use --help for user manual.\n"


/**
 * Returns parameter 'x' raised to the power of 'power'
 * @param x - A value used in the calculation where 'x' is raised to the power of 'power'.
 * @param power A value used in the calculation where 'x' is raised to the power of 'power'.
 * @return Double value 'x' raised to the power of 'power'
 */
double toPowerOf(double x, int power) {
    double result = x;
    for (int i = 2; i <= power; i++) result *= x;
    return result;
}

/**
 * Returns tangens of the angle 'x' calculated using of Taylor's polynom.
 * @param x - Given angle to be calculated
 * @param n - Number of iterations
 * @return Tangens of the angle 'x'
 */
double taylor_tan(double x, unsigned int n) {

    double numArray[] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162,
                         18888466084, 113927491862, 58870668456604};

    double denArray[] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875,
                         10854718875, 1856156927625, 194896477400625,
                         49308808782358125, 3698160658676859375};

    double result = 0;
    int exponent = 1;

    for (unsigned int i = 0; i < n; i++) {
        result += (numArray[i] * toPowerOf(x, i + exponent)) / denArray[i];
        exponent++;
    }
    return result;
}

/**
 * Returns tangens of the angle 'x' calculated using of sequenced fractions.
 * @param x - Given angle to be calculated
 * @param n - Number of iterations
 * @return Tangens of the angle 'x'
 */
double cfrac_tan(double x, unsigned int n) {
    double previous = 0;
    double result;

    for (unsigned int i = n; i > 0; i--) {
        result = 1/(((i*2-1)/x)-previous);
        previous = result;
    }

    return result;
}

/**
 * Checks if given value is not null and is number.
 * @param number - String value to be checked
 * @return True if is a number, false if not
 */
bool isValidNumber(char number[]) {
    if (number != NULL) {

        char *letter = '\0';
        strtod(number, &letter);
        if (letter != '\0') return false;

        if (atof(number) != 0) return true;
        else if (atoi(number) != 0) return true;
        else return false;
    } else return false;
}

/**
 * Prints table of tangens calculations, with given iterations.
 * @param angle - Angle given in radians
 * @param compStart - First iteration to be displayed
 * @param compEnd - Last iteration to be displayed
 * @return True if completed succesfully, false if not
 */
bool printTanTable(double angle, int compStart, int compEnd) {
    if (0 < compStart && compStart <= compEnd && compEnd < 14) {
        double results[5];

        for (unsigned int i = (unsigned int) compStart; i <= (unsigned int) compEnd; i++) {
            results[0] = tan(angle);
            results[1] = taylor_tan(angle, i);
            results[2] = fabs(results[0] - results[1]);
            results[3] = cfrac_tan(angle, i);
            results[4] = fabs(results[0] - results[3]);

            printf("%d %e %e %e %e %e\n", i, results[0], results[1], results[2], results[3], results[4]);
        }
        return true;
    } else return false;
}

int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printf("Usage: proj2 [options]\n\n");

            printf("Options:\n");

            printf("\t--help\tShows instructions to operate the program\n");

            printf("\t--tan angle compStart compEnd\n");
            printf("\t\tangle: Angle given in radians\n");
            printf("\t\tcompStart: First comparation to be displayed (Higher than 0, lower than 'compEnd')\n");
            printf("\t\tcompEnd: Last comparation to be displayed. (Higher than 'compStart', lower than 14.)\n");

            printf("\t--m angle_alpha [angle_beta] [-c height]\n");
            printf("\t\tangle_alpha: Angle given in radians\n");
            printf("\t\tangle_beta: Angle given in radians\n");
            printf("\t\t[-c height]: Optional height of sensor placed from the ground (Default 1.5m)\n");
            printf("\t\t\theight: Height given in meters\n");
            break;
        } else if (strcmp(argv[i], "--tan") == 0) {
            if (isValidNumber(argv[i + 1]) && isValidNumber(argv[i + 2]) && isValidNumber(argv[i + 3])) {
                if (printTanTable(atof(argv[i + 1]), atoi(argv[i + 2]), atoi(argv[i + 3])) == false)
                    printf(ERR_SOMETHING);
            } else printf(ERR_BAD_FORMAT);
            break;
        } else if (strcmp(argv[i], "-m") == 0) {
            double height = 1.5;
            for (int j = 1; j < argc; j++) {
                if (strcmp(argv[j], "-c") == 0) {
                    if (isValidNumber(argv[j + 1])) height = atof(argv[j + 1]);
                    break;
                }
            }

            if (isValidNumber(argv[i + 1]) && isValidNumber(argv[i + 2]) == false) {
                double angleAlpha = 0;
                angleAlpha = atof(argv[i + 1]);

                double towerDistance = height / cfrac_tan(angleAlpha, 10);

                printf("%.10e\n", towerDistance);
            } else if (isValidNumber(argv[i + 1]) && isValidNumber(argv[i + 2])) {
                double angleAlpha = 0;
                double angleBeta = 0;
                angleAlpha = atof(argv[i + 1]);
                angleBeta = atof(argv[i + 2]);

                double towerDistance = height / cfrac_tan(angleAlpha, 10);
                double towerHeight = towerDistance * cfrac_tan(angleBeta, 10) + 1.5;

                printf("%.10e\n", towerDistance);
                printf("%.10e\n", towerHeight);
            } else printf(ERR_BAD_FORMAT);

            break;
        } else printf(ERR_BAD_FORMAT);
    }

    return 0;
}
