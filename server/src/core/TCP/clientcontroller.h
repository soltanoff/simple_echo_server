/* =====================================================================================================================
 * File: clientcontroller.h
 * Description: Реализация полного взаимодействия с сетевыми клиентами
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
/* ================================================================================================================== */
#include "../../config/config.h"
/* ================================================================================================================= */
/*!
 * @class CClientController
 * Класс описывающий основные поля и методы для реализации сетевого взаимодействия клиента
 */
class CClientController {
public:
    CClientController(std::uint32_t clientNumber, SOCKET clientSocket, sockaddr_in clientInfo);  // , std::mutex& server_mutex);
    ~CClientController() { close(); }
    /*!
     * @public Основной циклический метод класса.
     * @return 0 если работа завершилась без аварий, иначе -1
     * @note основной метод сетевого взаимодействия
     */
    uint32_t exec();
    /*!
     * @public Метод закрывающий сокет объекта.
     * @return None
     * @note закрытие сокета осуществляется с параметром SHUT_WR
     */
    void close();
private:
    sockaddr_in _clientInfo;
    //! @private порядковый номер клиента
    std::uint32_t _clientNumber;
    //! @private активный сокет клиента
    SOCKET _clientSocket;
    /*!
     * @private Метод обработки ответа клиента
     * @param receivedBytes - количество полученных байт (out)
     * @param receivedBuf - массив данных, содержащий ответ (out)
     * @return true - данные получены успешно, инче false
     * @note осуществляет обработку полученного ответа клиента по сети
     */
    bool getAnswer(std::int32_t &receivedBytes, char *receivedBuf);
    /*!
     * @private Метод отправки данных клиенту
     * @param msg - массив отправляемых данных
     * @return true - данные отправленны успешно, иначе false
     * @note осуществляет отправку обработанных данных объекта клиенту по протоколу TCP/IP
     */
    bool sendMessage(const char *msg);
};
/* ================================================================================================================== */
#endif /* SERVERTHREAD_H */
