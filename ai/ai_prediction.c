#include <stdio.h>
#include <stdlib.h>

#define MAX_DATA 100

typedef struct
{
    double distance;
    double traffic;
    double travelTime;
} Data;


/* --------------------------------------------------
   PREDICTION FUNCTION
   Travel Time = b0 + b1(Distance) + b2(Traffic)
   -------------------------------------------------- */
double predict(double distance,
               double traffic,
               double b0,
               double b1,
               double b2)
{
    return b0 + b1 * distance + b2 * traffic;
}


/* --------------------------------------------------
   TRAIN MULTIPLE LINEAR REGRESSION MODEL
   Uses Gaussian Elimination
   -------------------------------------------------- */
int trainModel(Data data[],
               int n,
               double *b0,
               double *b1,
               double *b2)
{
    double sx1 = 0;
    double sx2 = 0;
    double sy = 0;

    double sx1x1 = 0;
    double sx2x2 = 0;
    double sx1x2 = 0;

    double sx1y = 0;
    double sx2y = 0;

    int i, j, k;

    /* Calculate required sums */
    for (i = 0; i < n; i++)
    {
        double x1 = data[i].distance;
        double x2 = data[i].traffic;
        double y = data[i].travelTime;

        sx1 += x1;
        sx2 += x2;
        sy += y;

        sx1x1 += x1 * x1;
        sx2x2 += x2 * x2;
        sx1x2 += x1 * x2;

        sx1y += x1 * y;
        sx2y += x2 * y;
    }


    /*
       Normal equations:

       [ n      sx1      sx2   ] [b0]   [sy]
       [ sx1    sx1x1    sx1x2 ] [b1] = [sx1y]
       [ sx2    sx1x2    sx2x2 ] [b2]   [sx2y]
    */

    double A[3][4] =
    {
        {n,   sx1,   sx2,   sy},
        {sx1, sx1x1, sx1x2, sx1y},
        {sx2, sx1x2, sx2x2, sx2y}
    };


    /* Gaussian Elimination */

    for (i = 0; i < 3; i++)
    {
        double pivot = A[i][i];

        if (pivot == 0)
        {
            return 0;
        }

        /* Normalize pivot row */
        for (j = i; j < 4; j++)
        {
            A[i][j] = A[i][j] / pivot;
        }

        /* Eliminate other rows */
        for (k = 0; k < 3; k++)
        {
            if (k != i)
            {
                double factor = A[k][i];

                for (j = i; j < 4; j++)
                {
                    A[k][j] =
                        A[k][j] - factor * A[i][j];
                }
            }
        }
    }


    /* Regression coefficients */

    *b0 = A[0][3];
    *b1 = A[1][3];
    *b2 = A[2][3];

    return 1;
}


/* --------------------------------------------------
   CALCULATE MEAN ABSOLUTE ERROR
   -------------------------------------------------- */
double calculateMAE(Data data[],
                    int n,
                    double b0,
                    double b1,
                    double b2)
{
    double totalError = 0;

    for (int i = 0; i < n; i++)
    {
        double predicted =
            predict(data[i].distance,
                    data[i].traffic,
                    b0,
                    b1,
                    b2);

        double error =
            data[i].travelTime - predicted;

        if (error < 0)
        {
            error = -error;
        }

        totalError += error;
    }

    return totalError / n;
}


/* --------------------------------------------------
   MAIN FUNCTION
   -------------------------------------------------- */
int main()
{
    Data data[MAX_DATA];

    int n = 0;

    double b0;
    double b1;
    double b2;

    double mae;

    double distance;
    int traffic;

    double predicted;


    /* --------------------------------------------------
       OPEN DATASET
       -------------------------------------------------- */

    FILE *file =
        fopen("data/travel_data.csv", "r");

    if (file == NULL)
    {
        printf("\nError: Could not open dataset.\n");
        printf("Check that this file exists:\n");
        printf("data/travel_data.csv\n");

        return 1;
    }


    /* Skip CSV header */

    char header[200];

    fgets(header, sizeof(header), file);


    /* Read dataset */

    while (n < MAX_DATA &&
           fscanf(file,
                  "%lf,%lf,%lf",
                  &data[n].distance,
                  &data[n].traffic,
                  &data[n].travelTime) == 3)
    {
        n++;
    }

    fclose(file);


    /* Check dataset */

    if (n == 0)
    {
        printf("\nError: Dataset is empty.\n");

        return 1;
    }


    /* --------------------------------------------------
       TRAIN MODEL
       -------------------------------------------------- */

    if (!trainModel(data,
                    n,
                    &b0,
                    &b1,
                    &b2))
    {
        printf("\nError: Model training failed.\n");

        return 1;
    }


    /* --------------------------------------------------
       CALCULATE MAE
       -------------------------------------------------- */

    mae =
        calculateMAE(data,
                     n,
                     b0,
                     b1,
                     b2);


    /* --------------------------------------------------
       DISPLAY MODEL INFORMATION
       -------------------------------------------------- */

    printf("\n");
    printf("========================================\n");
    printf("     AI TRAVEL-TIME PREDICTION\n");
    printf("========================================\n");

    printf("\nDataset Records : %d\n", n);

    printf("\nRegression Equation:\n");

    printf("Travel Time = %.2f + %.2f(Distance) + %.2f(Traffic)\n",
           b0,
           b1,
           b2);

    printf("\nMean Absolute Error: %.2f minutes\n",
           mae);


    /* --------------------------------------------------
       CREATE RESULTS FILE
       -------------------------------------------------- */

    FILE *resultFile =
        fopen("results/prediction_results.txt", "w");

    if (resultFile == NULL)
    {
        printf("\nError: Could not create results file.\n");

        return 1;
    }


    /* Write model information */

    fprintf(resultFile,
            "AI TRAVEL-TIME PREDICTION RESULTS\n");

    fprintf(resultFile,
            "=================================\n\n");

    fprintf(resultFile,
            "Dataset Records: %d\n\n",
            n);

    fprintf(resultFile,
            "Regression Equation:\n");

    fprintf(resultFile,
            "Travel Time = %.2f + %.2f(Distance) + %.2f(Traffic)\n\n",
            b0,
            b1,
            b2);

    fprintf(resultFile,
            "Mean Absolute Error: %.2f minutes\n\n",
            mae);


    /* --------------------------------------------------
       WRITE ACTUAL VS PREDICTED RESULTS
       -------------------------------------------------- */

    fprintf(resultFile,
            "ACTUAL VS PREDICTED RESULTS\n");

    fprintf(resultFile,
            "------------------------------------------------------------\n");

    fprintf(resultFile,
            "Distance\tTraffic\tActual\tPredicted\tError\n");

    fprintf(resultFile,
            "------------------------------------------------------------\n");


    for (int i = 0; i < n; i++)
    {
        double predictedValue =
            predict(data[i].distance,
                    data[i].traffic,
                    b0,
                    b1,
                    b2);

        double error =
            data[i].travelTime - predictedValue;

        if (error < 0)
        {
            error = -error;
        }

        fprintf(resultFile,
                "%.2f\t\t%.0f\t\t%.2f\t%.2f\t\t%.2f\n",
                data[i].distance,
                data[i].traffic,
                data[i].travelTime,
                predictedValue,
                error);
    }


    fclose(resultFile);


    /* --------------------------------------------------
       USER INPUT FOR NEW PREDICTION
       -------------------------------------------------- */

    printf("\n----------------------------------------\n");

    printf("Enter route distance (km): ");
    scanf("%lf", &distance);

    printf("Enter traffic level (1-5): ");
    scanf("%d", &traffic);


    /* Validate distance */

    if (distance <= 0)
    {
        printf("\nInvalid distance.\n");

        return 1;
    }


    /* Validate traffic */

    if (traffic < 1 || traffic > 5)
    {
        printf("\nInvalid traffic level.\n");
        printf("Traffic level must be between 1 and 5.\n");

        return 1;
    }


    /* Predict travel time */

    predicted =
        predict(distance,
                traffic,
                b0,
                b1,
                b2);


    /* --------------------------------------------------
       DISPLAY FINAL PREDICTION
       -------------------------------------------------- */

    printf("\n========================================\n");
    printf("          PREDICTION RESULT\n");
    printf("========================================\n");

    printf("Route Distance        : %.2f km\n",
           distance);

    printf("Traffic Level         : %d\n",
           traffic);

    printf("Predicted Travel Time : %.2f minutes\n",
           predicted);

    printf("========================================\n");

    printf("\nResults saved to:\n");
    printf("results/prediction_results.txt\n");


    return 0;
}