#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#define STOCK_AMOUNT 1000
#define TRIES_UNTIL_SELLING 500000

void getMACD(const float *stocks, const float *macd);

void getSignal(const float *macd, const float *signal);

bool isWorthSelling(const float *stocks, int day, const float *bought, int i);

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


void getSignal(float *macd, float *signal) {
    for (int i = 0; i < STOCK_AMOUNT; i++) {
        if (i <= 35)
            signal[i] = 0.0;
        else {
            signal[i] = getEMA(9, macd, i);
        }
    }
}

void getMACD(float *stocks, float *macd) {
    for (int i = 0; i < STOCK_AMOUNT; i++) {
        if (i <= 26)
            macd[i] = 0.0;
        else {
            macd[i] = getEMA(12, stocks, i) - getEMA(26, stocks, i);
        }
    }
}

bool isMACDBigger(float macd, float signal) {
    return signal < macd;
}


float play(float *stocks, const float *macd, const float *signal) {
    int day = 36;
    float money = 1000.0;
    auto *waitForAction = new int[1000];
    auto *bought = new float[1000];
    for (int i = 0; i < 1000; i++) {
        bought[i] = 0.0;
    }
    bool macdOverSignal = isMACDBigger(macd[day], signal[day]);

    while (day != 999) {
        day++;
        if (macdOverSignal != isMACDBigger(macd[day], signal[day])) {

            if (isMACDBigger(macd[day], signal[day]) && money > 0) {
                float amount = (money / stocks[day]) * 0.5f;
                money -= amount * stocks[day];
                bought[day] = amount;
            } else if (!isMACDBigger(macd[day], signal[day])) {
                for (int i = 0; i <= day; i++) {
                    if (bought[i] == 0.0)
                        continue;
                    if (bought[i] * stocks[day] > bought[i] * stocks[i] || waitForAction[i] >= TRIES_UNTIL_SELLING) {
                        money += bought[i] * stocks[day];
                        bought[i] = 0;
                    } else if (waitForAction[i] <= TRIES_UNTIL_SELLING) {
                        waitForAction[i]++;
                    }
                }
            }

        }
        macdOverSignal = isMACDBigger(macd[day], signal[day]);
    }

    for (int i = 0; i <= 999; i++) {
        money += bought[i] * stocks[day];
    }

    return money;
}

int main() {
    float *stocks = readFromFile(const_cast<char *>("wig20.txt"));
    auto *macd = new float[STOCK_AMOUNT];
    auto *signal = new float[STOCK_AMOUNT];

    getMACD(stocks, macd);
    getSignal(macd, signal);

    saveToFile(const_cast<char *>("macd.txt"), macd);
    saveToFile(const_cast<char *>("signal.txt"), signal);

    play(stocks, macd, signal);
    std::cout << play(stocks, macd, signal);

    free(macd);
    free(signal);
    free(stocks);
    return 0;
}
