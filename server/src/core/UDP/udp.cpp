/* =====================================================================================================================
 * File: udp.cpp
 * Description: Лисенер UDP
 * Author: soltanoff
 * ================================================================================================================== */
#include "udp.h"


CUDPListener::CUDPListener() {
    this->_socket = socket(AF_INET, SOCK_DGRAM, ServerCfg::UDP_PROTOCOL);
    // service содержит информация о семействе адресов,
    // IP адрес и номер порта
    this->_service.sin_family = AF_INET; // семейство адресов интернет
    this->_service.sin_addr.s_addr = INADDR_ANY;
    this->_service.sin_port = htons(ServerCfg::UDP_PORT); // номер порта

    std::cout << "[UDP|STATUS] Server ready.\n";
};

CUDPListener::~CUDPListener() {
    this->close();
};

void CUDPListener::close() {
    if (this->_socket >= 0) shutdown(this->_socket, 1);
}

bool CUDPListener::prepareListener() {
    if (this->_socket < 0) {
        std::cerr << "[UDP|ERROR: SOCKET] Error at socket()" << std::endl;
        return false;
    }
    if (bind(this->_socket, (sockaddr *) &(this->_service), sizeof(this->_service)) == SOCKET_ERROR) {
        std::cerr << "[UDP|ERROR: sockaddr] bind() failed.\n";
        return false;
    }

    return true;
}

bool CUDPListener::getAnswer(std::uint32_t &receivedBytes, char *receivedBuf, sockaddr_in &clientInfo) {
    uint32_t clientInfoLen = sizeof(clientInfo);
    receivedBytes = (std::uint32_t) recvfrom(
            this->_socket,
            receivedBuf,
            ServerCfg::BUFF_SIZE,
            0,
            (sockaddr *) &clientInfo,
            &clientInfoLen
    );

    if (receivedBytes == 0) {
        std::cout << "[UDP|CLIENT] Connection closed.\n";
        throw 0;
    }
    if (receivedBytes < 0) {
        std::cout << "[UDP|CLIENT] Connection lost.\n";
        throw 0;
    }
    std::cout << "BYTESRECV: " << receivedBytes << std::endl;
    return true;
}

uint32_t CUDPListener::sendMessage(const char *msg, const sockaddr_in &clientInfo) {
    return (uint32_t) sendto(
            this->_socket, msg, ServerCfg::BUFF_SIZE, 0, (struct sockaddr *) &clientInfo, sizeof(clientInfo)
    );
}

void CUDPListener::exec() {
    std::uint32_t receivedBytes = 0;
    if (!this->prepareListener()) return;

    while (true) {
        try {
            sockaddr_in clientInfo;
            char receivedBuf[ServerCfg::BUFF_SIZE] = "";

            this->getAnswer(receivedBytes, receivedBuf, clientInfo);

            {
                std::lock_guard<decltype(MAIN_MUTEX)> scoped_lock(MAIN_MUTEX);
                std::cout
                        << "\n[UDP|Client (" << inet_ntoa(clientInfo.sin_addr) << ":"
                        << ntohs(clientInfo.sin_port) << ")] Bytes recv: " << receivedBytes
                        << " | \nMSG: " << receivedBuf << "\n"
                        << "[UDP|SERVER] SEND ECHO ANSWER";

                /*
                 * Метод осуществляющий поиск всех цифр от 0 до 9 в принятом сообщении и выводит в консоль:
                 * сумму чисел;
                 * все числа, в порядке убывания;
                 * максимальное и минимальное значения.
                 */
                calculateMethod(receivedBuf);
                std::cout << std::endl;
            }

            this->sendMessage(receivedBuf, clientInfo);
        }
        catch (std::exception e) {
            std::cerr << "[UDP|ERROR] UDPListener::exec(): " << e.what() << std::endl;
            this->close();
            return;
        }
        catch (...) {
            std::cerr << "[UDP|ERROR] Server get error in UDPListener::exec().\n";
            this->close();
            return;
        }
    }
}