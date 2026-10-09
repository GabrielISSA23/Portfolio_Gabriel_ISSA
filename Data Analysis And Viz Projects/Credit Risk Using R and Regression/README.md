# Credit Risk Analysis using Regression

This project estimates client default risk using three regression models: **Linear Probability Model (LPM)**, **Logit**, and **Probit**.

## Project structure

```text
.
├── Credit_Default_Analysis.Rmd
├── Credit_Default_Analysis.html
├── raw_data/
│   ├── credit-1.csv
│   └── prospects.csv
└── Output_plots/
    ├── correlation_matrix.png
    ├── probabilities_comparison.png
    ├── expected_profit_by_prospect.png
    └── scenarii_comparison.png
```

## Workflow

1. Import and transform the credit data; create explanatory variables.
2. Inspect correlations between variables.
3. Estimate and compare LPM, Logit, and Probit models.
4. Predict default probabilities for 10 loan applicants.
5. Calculate expected profit for one-year loans at a 10% interest rate and decide which loans to accept.
6. Compare the baseline results with a downside scenario.

## Run the project

Open `Credit_Default_Analysis.Rmd` in RStudio and click **Knit** to generate the HTML report. The input CSV files should be available in `raw_data/`.

## Requirements

R packages: `stringr`, `ggplot2`, `dplyr`, `tidyr`, and `corrplot`.

> Ensure that you are allowed to publish the input data before committing the CSV files to a public repository.
