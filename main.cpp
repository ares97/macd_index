#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#define STOCK_AMOUNT 1000

float *readFromFile(char *title) {
    std::ifstream input;
    input.open(title, std::ios::in);
    auto *stocks = new float[STOCK_AMOUNT];

    for (int i = 0; i < STOCK_AMOUNT; i++) {
        input >> stocks[i];
    }

    input.close();
    return stocks;
}

void saveToFile(char *title, float *data) {
    std::ofstream myfile;
    myfile.open(title);

    for (int i = 0; i < STOCK_AMOUNT; i++) {
        myfile << data[i];
        myfile << '\n';
    }
    myfile.close();
}

float getEMA(int N, float *data, int end) {
    double alpha = 2.0 / (N - 1);
    double expr = 1.0 - alpha;
    float numerator = 0.0;
    float denominator = 0.0;


    for (int i = 0; i <= N; i++) {
        double expToPow = pow(expr, i);
        numerator += expToPow * data[end - i];
        denominator += expToPow;
    }

    return numerator / denominator;
}


int main() {
    float *stocks = readFromFile(const_cast<char *>("wig20.txt"));
    auto *macd = new float[STOCK_AMOUNT];
    auto *signal = new float[STOCK_AMOUNT];

    for (int i = 0; i < STOCK_AMOUNT; i++) {
        if (i <= 26)
            macd[i] = 0.0;
        else {
            macd[i] = getEMA(12, stocks, i) - getEMA(26, stocks, i);
        }
    }

    for (int i = 0; i < STOCK_AMOUNT; i++) {
        if (i <= 35)
            signal[i] = 0.0;
        else {
            signal[i] = getEMA(9, macd, i);
        }
    }

    saveToFile(const_cast<char *>("macd.txt"), macd);
    saveToFile(const_cast<char *>("signal.txt"), signal);

    free(macd);
    free(signal);
    free(stocks);
    return 0;
}