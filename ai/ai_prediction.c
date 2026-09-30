#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ai_prediction.h"

#define MAX_DATA 100
#define MIN_TRAFFIC 1
#define MAX_TRAFFIC 3
#define EPSILON 1e-12

typedef struct
{
    double distance;
    double traffic;
    double travelTime;
} Data;

static double b0 = 0.0;
static double b1 = 0.0;
static double b2 = 0.0;
static int modelReady = 0;

static double predict(double distance, double traffic)
{
    return round(b0 * 100.0) / 100.0 +
           (round(b1 * 100.0) / 100.0) * distance +
           (round(b2 * 100.0) / 100.0) * traffic;
}
static int trainModel(Data data[], int n)
{
    double meanX1 = 0.0, meanX2 = 0.0, meanY = 0.0;
    double s11 = 0.0, s22 = 0.0, s12 = 0.0;
    double s1y = 0.0, s2y = 0.0;
    double determinant;
    int i;

    if (n < 3)
        return 0;

    /* Calculate feature and target means. */
    for (i = 0; i < n; i++)
    {
        meanX1 += data[i].distance;
        meanX2 += data[i].traffic;
        meanY += data[i].travelTime;
    }

    meanX1 /= n;
    meanX2 /= n;
    meanY /= n;

    /*
       Center the variables before solving the normal equations.
       This avoids numerical instability caused by the different
       scales of distance and traffic.
    */
    for (i = 0; i < n; i++)
    {
        double x1 = data[i].distance - meanX1;
        double x2 = data[i].traffic - meanX2;
        double y = data[i].travelTime - meanY;

        s11 += x1 * x1;
        s22 += x2 * x2;
        s12 += x1 * x2;
        s1y += x1 * y;
        s2y += x2 * y;
    }

    /*
       Centered normal equations:

       [s11  s12] [b1] = [s1y]
       [s12  s22] [b2]   [s2y]
    */
    determinant = s11 * s22 - s12 * s12;

    if (fabs(determinant) < EPSILON)
        return 0;

    b1 = (s1y * s22 - s2y * s12) / determinant;
    b2 = (s2y * s11 - s1y * s12) / determinant;
    b0 = meanY - b1 * meanX1 - b2 * meanX2;

    modelReady = 1;
    return 1;
}

static double calculateMAE(Data data[], int n)
{
    double totalError = 0.0;
    int i;

    if (n <= 0)
        return -1.0;

    for (i = 0; i < n; i++)
    {
        double error = data[i].travelTime -
                       predict(data[i].distance, data[i].traffic);

        totalError += fabs(error);
    }

    return totalError / n;
}

int initializeAI(void)
{
    Data data[MAX_DATA];
    int n = 0;
    int i;
    FILE *file = fopen("data/travel_data.csv", "r");

    modelReady = 0;

    if (file == NULL)
    {
        printf("\nAI Error: Could not open dataset.\n");
        printf("Required file: data/travel_data.csv\n");
        return 0;
    }

    char header[200];
    if (fgets(header, sizeof(header), file) == NULL)
    {
        fclose(file);
        printf("\nAI Error: Dataset header could not be read.\n");
        return 0;
    }

    while (n < MAX_DATA)
    {
        double distance, traffic, travelTime;
        int result = fscanf(file, "%lf,%lf,%lf",
                            &distance, &traffic, &travelTime);

        if (result == EOF)
            break;

        if (result != 3)
        {
            fclose(file);
            printf("\nAI Error: Invalid training data.\n");
            return 0;
        }

        if (distance <= 0 || traffic < MIN_TRAFFIC ||
            traffic > MAX_TRAFFIC || travelTime <= 0)
        {
            fclose(file);
            printf("\nAI Error: Invalid values in training data.\n");
            return 0;
        }

        data[n].distance = distance;
        data[n].traffic = traffic;
        data[n].travelTime = travelTime;
        n++;
    }

    fclose(file);

    if (n <= 0)
    {
        printf("\nAI Error: Dataset is empty.\n");
        return 0;
    }

    if (!trainModel(data, n))
    {
        printf("\nAI Error: Model training failed.\n");
        return 0;
    }

    double mae = calculateMAE(data, n);

    printf("\n========================================\n");
    printf("       AI TRAVEL-TIME MODULE\n");
    printf("========================================\n");
    printf("Dataset Records : %d\n", n);
    printf("\nRegression Equation:\n");
    printf("Travel Time = %.2f + %.2f(Distance) + %.2f(Traffic)\n",
           b0, b1, b2);
    printf("\nMean Absolute Error: %.2f minutes\n", mae);
    printf("AI Model Status: Ready\n");
    printf("========================================\n");

    FILE *resultFile = fopen("results/prediction_results.txt", "w");

    if (resultFile != NULL)
    {
        fprintf(resultFile, "AI TRAVEL-TIME PREDICTION RESULTS\n");
        fprintf(resultFile, "=================================\n\n");
        fprintf(resultFile, "Dataset Records: %d\n\n", n);
        fprintf(resultFile, "Regression Equation:\n");
        fprintf(resultFile,
                "Travel Time = %.2f + %.2f(Distance) + %.2f(Traffic)\n\n",
                b0, b1, b2);
        fprintf(resultFile, "Mean Absolute Error: %.2f minutes\n\n", mae);
        fprintf(resultFile, "ACTUAL VS PREDICTED RESULTS\n");
        fprintf(resultFile, "------------------------------------------------------------\n");
        fprintf(resultFile, "Distance\tTraffic\tActual\tPredicted\tError\n");
        fprintf(resultFile, "------------------------------------------------------------\n");

        for (i = 0; i < n; i++)
        {
            double predictedValue = predict(data[i].distance,
                                            data[i].traffic);
            double error = fabs(data[i].travelTime - predictedValue);

            fprintf(resultFile, "%.2f\t\t%.0f\t\t%.2f\t%.2f\t\t%.2f\n",
                    data[i].distance, data[i].traffic,
                    data[i].travelTime, predictedValue, error);
        }

        fclose(resultFile);
    }

    return 1;
}

double predictTravelTime(double distance, int traffic)
{
    if (!modelReady)
    {
        printf("\nAI Error: Model is not initialized.\n");
        return -1.0;
    }

    if (distance <= 0)
    {
        printf("\nAI Error: Invalid route distance.\n");
        return -1.0;
    }

    if (traffic < MIN_TRAFFIC || traffic > MAX_TRAFFIC)
    {
        printf("\nAI Error: Traffic level must be between 1 and 3.\n");
        return -1.0;
    }

    return predict(distance, (double)traffic);
}

void displayAIPrediction(double distance, int traffic)
{
    double predicted = predictTravelTime(distance, traffic);

    if (predicted < 0)
        return;

    printf("\n========================================\n");
    printf("       AI TRAVEL-TIME PREDICTION\n");
    printf("========================================\n");
    printf("Route Distance        : %.2f km\n", distance);
    if (traffic == 1)
    printf("Traffic Level         : 1 (Low)\n");
    else if (traffic == 2)
    printf("Traffic Level         : 2 (Medium)\n");
    else
    printf("Traffic Level         : 3 (High)\n");
    printf("Predicted Travel Time : %.2f minutes\n", predicted);
    printf("========================================\n");
}
int main()
{
    double distance;
    int traffic;

    initializeAI();

    printf("\nEnter route distance (km): ");
    scanf("%lf", &distance);

    printf("Enter traffic level (1=Low, 2=Medium, 3=High): ");
    scanf("%d", &traffic);

    displayAIPrediction(distance, traffic);

    return 0;
}