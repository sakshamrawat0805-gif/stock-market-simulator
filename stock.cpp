// MEMBER 4: Data, persistence and analytics - Transaction, saveData, loadData,
// showHistory, movingAverage, maximumProfit. Copied exactly from the project's
// main.cpp. Stock class is in Stock.h / Stock.cpp.
// Needs from other parts: Holding and Portfolio (Member 3), STARTING_BALANCE (Team Lead).

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

struct Transaction {
    std::string type;
    std::string symbol;
    int quantity = 0;
    double price = 0.0;
    double total = 0.0;
};

// Save application data
void saveData(
    double balance,
    const Portfolio& portfolio,
    const std::vector<Transaction>& transactions
) {
    std::ofstream wallet("data/wallet.txt");

    wallet << std::setprecision(17)
           << balance << '\n';

    std::ofstream holdings("data/portfolio.txt");

    for (const auto& [symbol, holding] : portfolio) {
        holdings << symbol << ' '
                 << holding.quantity << ' '
                 << holding.averagePrice << '\n';
    }

    std::ofstream history("data/transactions.txt");

    for (const auto& transaction : transactions) {
        history << transaction.type << ' '
                << transaction.symbol << ' '
                << transaction.quantity << ' '
                << transaction.price << '\n';
    }
}

void loadData(
    double& balance,
    Portfolio& portfolio,
    std::vector<Transaction>& transactions
) {
    std::ifstream wallet("data/wallet.txt");

    if (!(wallet >> balance)) {
        balance = STARTING_BALANCE;
    }

    std::ifstream holdings("data/portfolio.txt");

    std::string symbol;
    Holding holding;

    while (holdings >> symbol
                 >> holding.quantity
               >> holding.averagePrice) {
        portfolio[symbol] = holding;
    }

    std::ifstream history("data/transactions.txt");

    Transaction transaction;

    while (history >> transaction.type
                   >> transaction.symbol
                   >> transaction.quantity
                   >> transaction.price) {

        transaction.total =
            transaction.quantity * transaction.price;

        transactions.push_back(transaction);}
}
// Transaction history
void showHistory(
    const std::vector<Transaction>& transactions
) {
    std::cout << "\n========== TRANSACTION HISTORY ==========\n";

    if (transactions.empty()) {
        std::cout << "No transactions.\n";
        return;
    }
    for (const auto& item : transactions) {
        std::cout << item.type
                  << ' ' << item.symbol
                  << " qty " << item.quantity
                  << " price Rs " << item.price
                  << " total Rs " << item.total
                  << '\n';
                }}// Calculate moving average
double movingAverage(
    const std::vector<double>& prices,
    int days
) {
    if (prices.empty() || days <= 0) {
        return 0.0;
    }

    const int count =
        std::min<int>(days, prices.size());

    double total = 0.0;

    for (int i = static_cast<int>(prices.size()) - count;
         i < static_cast<int>(prices.size());
         i++) {

        total += prices[i];
    }

    return total / count;}
// Calculate maximum historical profit
double maximumProfit(
    const std::vector<double>& prices
) {
    double lowest =
        std::numeric_limits<double>::max();

    double best = 0.0;

    for (double price : prices) {

        lowest = std::min(lowest, price);

        best = std::max(
            best,
            price - lowest
        );
    }

    return best;
}