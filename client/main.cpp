/* =====================================================================================================================
 * File: main.cpp
 * Description: Основная функция проекта
 * Author: soltanoff
 * ================================================================================================================== */
#include <iomanip>
#include "src/core/UDP/udp.h"
#include "src/core/TCP/tcp.h"


int main(int argc, char *argv[]) {
    time_t t = time(NULL);
    tm *aTm = localtime(&t);
    std::cout
            << "[START] TIME: "
            << aTm->tm_year + 1900 << "/" << std::setfill('0') << std::setw(2)
            << aTm->tm_mon + 1 << "/" << std::setfill('0') << std::setw(2)
            << aTm->tm_mday << " " << std::setfill('0') << std::setw(2)
            << aTm->tm_hour << ":" << std::setfill('0') << std::setw(2)
            << aTm->tm_min << ":" << std::setfill('0') << std::setw(2)
            << aTm->tm_sec << std::endl;

    try {
        char answer[2];

        std::cout << "[CLIENT] Select the protocol: 1 - TCP; 2 - UDP [1/2] ";
        answerControl(answer, '1', '2');
        if (answer[0] == '1') {
            CTCPClient tcp;
            tcp.exec();
        } else {
            CUDPClient udp;
            udp.exec();
        }
    }
    catch (std::exception e) {
        std::cerr << "[MAIN] Error: " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "[MAIN] Fatal error: unknown exception in main()" << std::endl;
        return -1;
    }

    return 0;
}