/* =====================================================================================================================
 * File: tcp.cpp
 * Description: TCP клиент
 * Author: soltanoff
 * ================================================================================================================== */
#include "tcp.h"


CTCPClient::CTCPClient() {
    this->_socket = socket(AF_INET, SOCK_STREAM, ServerCfg::TCP_PROTOCOL);
    // service содержит информация о семействе адресов,
    // IP адрес и номер порта
    this->_service.sin_family = AF_INET; // семейство адресов интернет
    this->_service.sin_addr.s_addr = inet_addr(ServerCfg::LOCALHOST);
    this->_service.sin_port = htons(ServerCfg::TCP_PORT); // номер порта

    std::cout << "[STATUS] Client ready.\n";
};

CTCPClient::~CTCPClient() {
    this->close();
};

bool CTCPClient::prepareClient() {
    char answer[128];
    char serverAddress[16];

    std::cout << "[CLIENT] Do you want to enter server IP address? [Y/N] ";
    answerControl(answer);
    if (answer[0] == 'N' || answer[0] == 'n') {
        strcpy(serverAddress, ServerCfg::LOCALHOST);
        std::cout << "[CLIENT] Connecting to server...\n";
    } else {
        std::cout << "[CLIENT] Enter server IP addres: ";
        ipAddressControl(serverAddress);
        std::cout << "[CLIENT] Try to connect to %s...\n", serverAddress;
    }

    this->_service.sin_addr.s_addr = inet_addr(serverAddress);

    if (this->_socket < 0) {
        std::cout << "[ERROR: TCP SOCKET] Error at socket()" << std::endl;
        return false;
    }
    if (connect(this->_socket, (sockaddr *) &this->_service, sizeof(this->_service)) == SOCKET_ERROR) {
        std::cout << "[ERROR: TCP LISTEN] Error listening on socket.\n";
        return false;
    }

    this->sendMessage("1");
    return true;
}

bool CTCPClient::getAnswer(std::uint32_t &receivedBytes, char *receivedBuf) {
    receivedBytes = (std::uint32_t) recv(this->_socket, receivedBuf, (size_t) ServerCfg::BUFF_SIZE,
                                         0); // MSG_WAITALL)

    if (receivedBytes == 0) {
        std::cout << "[CLIENT] Connection closed.\n";
        throw 0;
    }
    if (receivedBytes < 0) {
        std::cout << "[CLIENT] Connection lost.\n";
        throw 0;
    }
    std::cout << "BYTESRECV: " << receivedBytes << std::endl;
    return true;
}

bool CTCPClient::sendMessage(const char *msg) {
    send(this->_socket, msg, strlen(msg), 0);
    return true;
}

void CTCPClient::exec() {
    std::uint32_t receivedBytes = (std::uint32_t) SOCKET_ERROR;
    if (!this->prepareClient()) return;

    while (true) {
        try {
            char receivedBuf[ServerCfg::BUFF_SIZE] = "";
            this->getAnswer(receivedBytes, receivedBuf);
            std::cout << "[SERVER] > " << receivedBuf << "\n";
            this->sendCommand();
        }
        catch (std::uint32_t) {
            std::cout << "[CLIENT] Terminate.\n";
            this->close();
            return;
        }
        catch (...) {
            std::cout << "[CLIENT] Client get error in CClient::exec().\n";
            this->close();
            return;
        }
    }
}
