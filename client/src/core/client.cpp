/* =====================================================================================================================
 * File: client.cpp
 * Description: Абстрактный класс клиента
 * Author: soltanoff
 * ================================================================================================================== */
#include "client.h"


void IClient::close() {
    if (this->_socket >= 0) shutdown(this->_socket, 1);
}

void IClient::sendCommand() {
    std::string sendBuf;
    std::cout << "[CLIENT] Command: ";

    std::cin.clear();
    std::cin.sync();
    std::getline(std::cin, sendBuf);

    if (sendBuf.compare("exit") == 0) {
        throw (std::uint32_t) 0;
    }

    if (!sendMessage(sendBuf.c_str()))
        throw 0;
}
