#ifndef AI_PREDICTION_H
#define AI_PREDICTION_H

/*
   Initialize and train the AI model
   using data/travel_data.csv.
*/
int initializeAI(void);

/*
   Predict travel time using:

   distance = route distance in km
   traffic = traffic level
   1 = Low
   2 = Medium
   3 = High
*/
double predictTravelTime(double distance, int traffic);

/*
   Display prediction result.
*/
void displayAIPrediction(double distance, int traffic);

#endif