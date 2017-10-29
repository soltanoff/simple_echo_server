/* =====================================================================================================================
 * File: tcp.h
 * Description: Лисенер TCP
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef SERVER_TCP_H
#define SERVER_TCP_H
/* ================================================================================================================== */
#include "../../config/config.h"
#include "clientcontroller.h"
#include "../listener.h"
/* ================================================================================================================== */
class CTCPListener: public IListener {
public:
    CTCPListener();

    ~CTCPListener();

    /*!
     * @public Метод закрывающий сокет сервера.
     * @return None
     * @note закрытие сокета с параметром SHUT_WR
     */
    void close();

    /*!
     * @public Основной циклический метод класса.
     * @note основной метод для взаимодействия сервера с клиентами
     */
    void exec();
protected:
    /*!
    * @protected Метод осуществляющий привязку и прослушку сокета
    * @return true если открыть сокет получилось, иначе false
    */
    bool prepareListener();
private:
    //! @private вектор умных указателей на handler'ы потоков клиентов
    std::vector<std::shared_ptr<std::thread>> _clientThreads;
    //! @private вектор активных сокетов клиентов
    std::vector<SOCKET> _clientSockets;

    /*!
     * @private Метод осуществляющий подключение клиента к серверу на уровне сокетов.
     * @param acceptSocket - сокет подключенного клиента
     * @param clientInfo - структура содержащая основную информацию о подключенном клиенте
     * @return None
     * @note осуществляет потверждение подключения клиента к серверу
     */
    void acceptSocket(SOCKET &acceptSocket, sockaddr_in &clientInfo);

    /*!
     * @private Метод потверждающий подключение клиента к серверу
     * @param acceptSocket - сокет подключенного клиента
     * @param clientNumber - номер подключенного клиента
     * @return true - подключение успешно, иначе false
     * @note осуществляет потверждение подключения клиента к серверу (отправляет клиенту статус подключения)
     */
    bool acceptConnection(const SOCKET &acceptSocket, const uint32_t &clientNumber);

    /*!
     * @private Метод регистрирующий подключение клиента .
     * @param acceptSocket - сокет подключенного клиента
     * @param clientInfo - структура содержащая основную информацию о подключенном клиенте
     * @param usersCount - значение счетчика клиентов
     * @return None
     * @note осуществляется регистрация: сокета, ip-адреса клиента, handler'а потока обработки данных клиента
    */
    void connectUser(const SOCKET &acceptSocket, const sockaddr_in &clientInfo, const std::uint32_t usersCount);
};
#endif /* SERVER_TCP_H */
