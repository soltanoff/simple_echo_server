/* =====================================================================================================================
 * File: clientinteraction.h
 * Description: Реализация полного взаимодействия с сетевыми клиентми
 * Author: soltanoff
 * ================================================================================================================== */
#include "clientcontroller.h"
#include "../listener.h"
#include <cstring>


CClientController::CClientController(std::uint32_t clientNumber, SOCKET clientSocket, sockaddr_in clientInfo) :
        _clientNumber(clientNumber),
        _clientSocket(clientSocket),
        _clientInfo(clientInfo){
}

bool CClientController::getAnswer(std::int32_t &receivedBytes, char *receivedBuf) {
    receivedBytes = (std::int32_t)recv(this->_clientSocket, receivedBuf, ServerCfg::BUFF_SIZE, 0);
    if (receivedBytes == 0) {
        std::cout << "\n[TCP|SERVER] Client #" << this->_clientNumber << " (" << inet_ntoa(this->_clientInfo.sin_addr) << ":"
                  << ntohs(this->_clientInfo.sin_port) << ") disconnected.\n";
        throw (std::uint32_t) 0;
    }
    if (receivedBytes < 0) {
        std::cout << "[TCP|SERVER] Connection lost with client #" << this->_clientNumber + 1 << std::endl;
        throw 0;
    }
    return true;
}

bool CClientController::sendMessage(const char *msg) {
    std::lock_guard<decltype(MAIN_MUTEX)> scoped_lock(MAIN_MUTEX);
    if (!strlen(msg))
        send(this->_clientSocket, " > NONE", 7, 0);
    else
        send(this->_clientSocket, msg, strlen(msg), 0);
    return true;
}

uint32_t CClientController::exec() {
    std::int32_t bytesRecv = SOCKET_ERROR;
    char receivedBuf[ServerCfg::BUFF_SIZE] = "";

    this->getAnswer(bytesRecv, receivedBuf);
    {
        std::lock_guard<decltype(MAIN_MUTEX)> scoped_lock(MAIN_MUTEX);
        std::cout
                << "\n[TCP|Client #" << this->_clientNumber << " (" << inet_ntoa(this->_clientInfo.sin_addr) << ":"
                << ntohs(this->_clientInfo.sin_port) << ")] Bytes recv: " << bytesRecv
                << " | \nMSG: " << receivedBuf << "\n"
                << "[TCP|SERVER] SEND ECHO ANSWER";

        /*
         * Метод осуществляющий поиск всех цифр от 0 до 9 в принятом сообщении и выводит в консоль:
         * сумму чисел;
         * все числа, в порядке убывания;
         * максимальное и минимальное значения.
         */
        calculateMethod(receivedBuf);
        std::cout << std::endl;
    }

    this->sendMessage(receivedBuf);

    return 0;
}

void CClientController::close() {
    shutdown(this->_clientSocket, 1);
}