# Credit Risk Analysis using Regression

This project applies the concepts of linear and non-linear regression to estimate the credit default risk of 10 potential clients. To do so, we use historical data on 1,000 of the bank's clients, including various attributes associated with each client as well as the probability that a given client defaults on a payment.

Thus, we estimate **client default risk** using three regression models: **Linear Probability Model (LPM)**, **Logit**, and **Probit**.

## Project structure

## What the Project do ? 

1. Import and transform the credit data and create explonatory variable corresponding to the designed model. 
2. Inspect correlations between variables.
3. Estimate the three regression model Linear Probability Model, Logit, and Probit models.
4. Predict default probabilities for 10 loan applicants.
5. Calculate expected profit for one-year loans at a 10% interest rate for each of the applicants and decide which loan should be accepted.

## How to run it in local ? 

Create a new project in Rstudio containing the following input files : `Credit_Default_Analysis.Rmd`, `raw_data/credit-1.csv`, `raw_data/prospects.csv`. 
Open `Credit_Default_Analysis.Rmd` in RStudio and click **Knit** to generate the HTML report or PDF report. The input CSV files should be available in `raw_data/`.

