/* =====================================================================================================================
 * File: main.cpp
 * Description: Основная функция проекта
 * Author: soltanoff
 * ================================================================================================================== */
#include "src/core/TCP/tcp.h"
#include "src/core/UDP/udp.h"
#include <iomanip>


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
        std::unique_ptr<std::thread> tcp_thread(new std::thread([] {
            CTCPListener s;
            s.exec();
        }));
        std::unique_ptr<std::thread> udp_thread(new std::thread([] {
            CUDPListener s;
            s.exec();
        }));

        tcp_thread->join();
        udp_thread->join();
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