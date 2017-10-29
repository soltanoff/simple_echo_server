/* =====================================================================================================================
 * File: config.h
 * Description: Файл основных конфигураций сервера
 * Author: soltanoff
 * ================================================================================================================== */
#include "config.h"


std::mutex MAIN_MUTEX;


void calculateMethod(const char *receivedBuf) {
    try {
        std::uint32_t numbersSum(0);
        std::vector<std::uint32_t> numbers;
        std::uint32_t receivedSize = (std::uint32_t) strlen(receivedBuf);

        for (auto i = 0; i < receivedSize; i++) {
            if (isdigit(receivedBuf[i])) {
                std::uint32_t temp = (std::uint32_t) receivedBuf[i] - 48;
                numbersSum += temp;
                numbers.push_back(temp);
            }
        }

        if (numbers.size() > 0) {
            std::sort(
                    numbers.begin(), numbers.end(),
                    [](const std::uint32_t a, const std::uint32_t b) { return a > b; }
            );
            std::cout << "\n * Find numbers: ";
            for (auto &x: numbers) std::cout << x << " ";
            std::cout << "\n * Sum numbers: " << numbersSum;
            std::cout << "\n * Max value: " << *std::max_element(numbers.begin(), numbers.end());
            std::cout << "\n * Min value: " << *std::min_element(numbers.begin(), numbers.end());
        } else {
            std::cout << "\n * Numbers not found";
        }
    }
    catch (std::exception e) {
        std::cerr << "\n * calculateMethod: " << e.what();
    }
    catch (...) {
        std::cerr << "\n * Unknown exception in calculateMethod";
    }
}