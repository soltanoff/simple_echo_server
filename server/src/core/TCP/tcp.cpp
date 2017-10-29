/* =====================================================================================================================
 * File: tcp.h
 * Description: Лисенер TCP
 * Author: soltanoff
 * ================================================================================================================== */
#include "tcp.h"


void tcpThreadRoutine(std::uint32_t clientNumber, SOCKET clientSocket, sockaddr_in clientInfo) {
    CClientController client(clientNumber + 1, clientSocket, clientInfo);

    while (true) {
        try {
            std::uint32_t code = client.exec();
            if (code == -1) {
                std::cerr << "[SERVER] Client #" << clientNumber + 1 << " disconnected.\n";
                client.close();
                return;
            }
        }
        catch (std::uint32_t) {
            client.close();
            return;
        }
        catch (...) {
            std::cerr << "[ERROR] Server get error in thread_routine().\n";
            client.close();
            return;
        }
    }
}

CTCPListener::CTCPListener() {
    this->_socket = socket(AF_INET, SOCK_STREAM, ServerCfg::TCP_PROTOCOL);
    // service содержит информация о семействе адресов,
    // IP адрес и номер порта
    this->_service.sin_family = AF_INET; // семейство адресов интернет
    this->_service.sin_addr.s_addr = INADDR_ANY;
    this->_service.sin_port = htons(ServerCfg::TCP_PORT); // номер порта

    std::cout << "[TCP|STATUS] Server ready.\n";
};

CTCPListener::~CTCPListener() {
    this->close();
}

void CTCPListener::close() {
    this->_clientSockets.clear();
    this->_clientThreads.clear();

    if (this->_socket >= 0) shutdown(this->_socket, 1);
}

bool CTCPListener::prepareListener() {
    if (this->_socket < 0) {
        std::cerr << "[TCP|ERROR: SOCKET] Error at socket()" << std::endl;
        return false;
    }
    if (bind(this->_socket, (sockaddr *) &(this->_service), sizeof(this->_service)) == SOCKET_ERROR) {
        std::cerr << "[TCP|ERROR: sockaddr] bind() failed.\n";
        return false;
    }
    if (listen(this->_socket, ServerCfg::BACKLOG) == SOCKET_ERROR) {
        std::cerr << "[TCP|ERROR: TCP LISTEN] Error listening on socket.\n";
        return false;
    }

    return true;
}

void CTCPListener::acceptSocket(SOCKET &acceptSocket, sockaddr_in &clientInfo) {
    int clientInfoLen = sizeof(clientInfo);
    acceptSocket = SOCKET_ERROR;

    while (acceptSocket == SOCKET_ERROR)
        acceptSocket = accept(this->_socket, (sockaddr *) &clientInfo, (socklen_t *) &clientInfoLen);
}

bool CTCPListener::acceptConnection(const SOCKET &acceptSocket, const uint32_t &clientNumber) {
    std::int32_t receivedBytes;
    char receivedBuf[ServerCfg::BUFF_SIZE] = "";

    send(acceptSocket, "ACCEPT", strlen("ACCEPT"), 0);
    receivedBytes = (std::int32_t) recv(acceptSocket, receivedBuf, ServerCfg::BUFF_SIZE, 0);

    if (receivedBytes == 0) {
        std::cout << "[TCP|SERVER] Client #" << clientNumber + 1 << " disconnected.\n";
        return false;
    }
    if (receivedBytes < 0) {
        std::cout << "[TCP|SERVER] Connection lost with client #" << clientNumber + 1 << std::endl;
        return false;
    }

    return true;
}

void
CTCPListener::connectUser(const SOCKET &acceptSocket, const sockaddr_in &clientInfo, const std::uint32_t usersCount) {
    try {
        std::cout << "[TCP|SERVER] Client #"
                  << usersCount + 1
                  << " connected. "
                  << inet_ntoa(clientInfo.sin_addr) << std::endl;

        if (this->acceptConnection(acceptSocket, usersCount)) {
            std::shared_ptr<std::thread> thread(
                    new std::thread(tcpThreadRoutine, usersCount, acceptSocket, clientInfo)
            );

            this->_clientSockets.push_back(acceptSocket);
            this->_clientThreads.push_back(thread);
        }
    }
    catch (std::exception e) {
        std::cerr << "[TCP|ERROR] CServer::connect_user(): " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "[TCP|ERROR] Server get error in CServer::connect_user().\n";
    }
}

void CTCPListener::exec() {
    std::uint32_t count = 0;
    if (!this->prepareListener()) return;

    while (true) {
        try {
            SOCKET acceptSocket;
            sockaddr_in clientInfo;
            this->acceptSocket(acceptSocket, clientInfo);
            this->connectUser(acceptSocket, clientInfo, count);
            count++;
        }
        catch (std::exception e) {
            std::cerr << "[TCP|ERROR] TCPListener::exec(): " << e.what() << std::endl;
            this->close();
            return;
        }
        catch (...) {
            std::cerr << "[TCP|ERROR] Server get error in TCPListener::exec().\n";
            this->close();
            return;
        }
    }
}