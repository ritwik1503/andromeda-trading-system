#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace config {

// US stock universe (curated) — primary tickers used by demos/apps.
// Rename: kUSStockSymbols for clarity and to avoid confusion with future regions.
inline const std::vector<std::string> kUSStockSymbols = {
    // --- Tech & Growth ---
    "AAPL", "MSFT", "GOOGL", "GOOG", "AMZN", "NVDA", "META", "TSLA",
    "ADBE", "CRM",  "INTC",  "AMD",  "CSCO", "ORCL", "TXN",  "AVGO", "NFLX",
    "IBM",  "NOW",  "QCOM",

    // --- Finance ---
    "V", "MA", "JPM", "BAC", "WFC", "GS", "MS", "C", "AXP", "BLK",
    "TROW", "SCHW", "PYPL",

    // --- Healthcare ---
    "UNH", "JNJ", "PFE", "MRK", "ABBV", "LLY", "ABT", "BMY", "AMGN", "CVS",

    // --- Consumer Defensive / Staples ---
    "KO", "PEP", "PG", "MDLZ", "CL", "KMB", "COST", "WMT", "HRL", "KR",

    // --- Consumer Cyclical / Discretionary ---
    "HD", "LOW", "MCD", "NKE", "SBUX", "TGT", "CMG", "DG", "BKNG", "ROST",

    // --- Energy ---
    "XOM", "CVX", "COP", "SLB", "EOG", "OXY", "MPC", "PSX", "VLO", "HAL",

    // --- Industrials / Materials ---
    "CAT", "DE", "MMM", "UNP", "LMT", "GE", "HON", "RTX", "UPS", "FDX",

    // --- Utilities & REITs ---
    "NEE", "DUK", "SO", "ED", "ATO", "O", "SPG", "PLD", "AMT", "VTRS",

    // --- Communication & Media ---
    "VZ", "T", "DIS", "CMCSA", "CHTR",

    // --- Other / Dividend & Steady Earners ---
    "BEN", "ADM", "EMR", "ITW", "DOV", "GD", "MDT", "PPG", "MKC", "CLX"
};

// Optional lookup: human‑readable company names for symbols above.
inline const std::unordered_map<std::string, std::string> kUSStockNames = {
    // Tech & Growth
    {"AAPL", "Apple"},
    {"MSFT", "Microsoft"},
    {"GOOGL", "Alphabet A"},
    {"GOOG", "Alphabet C"},
    {"AMZN", "Amazon"},
    {"NVDA", "NVIDIA"},
    {"META", "Meta"},
    {"TSLA", "Tesla"},
    {"ADBE", "Adobe"},
    {"CRM", "Salesforce"},
    {"INTC", "Intel"},
    {"AMD", "AMD"},
    {"CSCO", "Cisco"},
    {"ORCL", "Oracle"},
    {"TXN", "Texas Instruments"},
    {"AVGO", "Broadcom"},
    {"NFLX", "Netflix"},
    {"IBM", "IBM"},
    {"NOW", "ServiceNow"},
    {"QCOM", "Qualcomm"},

    // Finance
    {"V", "Visa"},
    {"MA", "Mastercard"},
    {"JPM", "JPMorgan Chase"},
    {"BAC", "Bank of America"},
    {"WFC", "Wells Fargo"},
    {"GS", "Goldman Sachs"},
    {"MS", "Morgan Stanley"},
    {"C", "Citigroup"},
    {"AXP", "American Express"},
    {"BLK", "BlackRock"},
    {"TROW", "T. Rowe Price"},
    {"SCHW", "Charles Schwab"},
    {"PYPL", "PayPal"},

    // Healthcare
    {"UNH", "UnitedHealth"},
    {"JNJ", "Johnson & Johnson"},
    {"PFE", "Pfizer"},
    {"MRK", "Merck"},
    {"ABBV", "AbbVie"},
    {"LLY", "Eli Lilly"},
    {"ABT", "Abbott"},
    {"BMY", "Bristol Myers Squibb"},
    {"AMGN", "Amgen"},
    {"CVS", "CVS Health"},

    // Consumer Defensive / Staples
    {"KO", "Coca-Cola"},
    {"PEP", "PepsiCo"},
    {"PG", "Procter & Gamble"},
    {"MDLZ", "Mondelez"},
    {"CL", "Colgate-Palmolive"},
    {"KMB", "Kimberly-Clark"},
    {"COST", "Costco"},
    {"WMT", "Walmart"},
    {"HRL", "Hormel"},
    {"KR", "Kroger"},

    // Consumer Cyclical / Discretionary
    {"HD", "Home Depot"},
    {"LOW", "Lowe's"},
    {"MCD", "McDonald's"},
    {"NKE", "Nike"},
    {"SBUX", "Starbucks"},
    {"TGT", "Target"},
    {"CMG", "Chipotle"},
    {"DG", "Dollar General"},
    {"BKNG", "Booking"},
    {"ROST", "Ross Stores"},

    // Energy
    {"XOM", "Exxon Mobil"},
    {"CVX", "Chevron"},
    {"COP", "ConocoPhillips"},
    {"SLB", "Schlumberger"},
    {"EOG", "EOG Resources"},
    {"OXY", "Occidental"},
    {"MPC", "Marathon Petroleum"},
    {"PSX", "Phillips 66"},
    {"VLO", "Valero"},
    {"HAL", "Halliburton"},

    // Industrials / Materials
    {"CAT", "Caterpillar"},
    {"DE", "Deere"},
    {"MMM", "3M"},
    {"UNP", "Union Pacific"},
    {"LMT", "Lockheed Martin"},
    {"GE", "General Electric"},
    {"HON", "Honeywell"},
    {"RTX", "RTX"},
    {"UPS", "UPS"},
    {"FDX", "FedEx"},

    // Utilities & REITs
    {"NEE", "NextEra Energy"},
    {"DUK", "Duke Energy"},
    {"SO", "Southern Company"},
    {"ED", "Con Edison"},
    {"ATO", "Atmos Energy"},
    {"O", "Realty Income"},
    {"SPG", "Simon Property Group"},
    {"PLD", "Prologis"},
    {"AMT", "American Tower"},
    {"VTRS", "Viatris"},

    // Communication & Media
    {"VZ", "Verizon"},
    {"T", "AT&T"},
    {"DIS", "Disney"},
    {"CMCSA", "Comcast"},
    {"CHTR", "Charter Communications"},

    // Other / Dividend & Steady Earners
    {"BEN", "Franklin Resources"},
    {"ADM", "Archer-Daniels-Midland"},
    {"EMR", "Emerson Electric"},
    {"ITW", "Illinois Tool Works"},
    {"DOV", "Dover"},
    {"GD", "General Dynamics"},
    {"MDT", "Medtronic"},
    {"PPG", "PPG Industries"},
    {"MKC", "McCormick"},
    {"CLX", "Clorox"}
};

}  // namespace config
