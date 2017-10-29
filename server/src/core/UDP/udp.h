/* =====================================================================================================================
 * File: udp.h
 * Description: Лисенер UDP
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef SERVER_UDP_H
#define SERVER_UDP_H
/* ================================================================================================================== */
#include "../../config/config.h"
#include "../listener.h"
/* ================================================================================================================== */
class CUDPListener: private IListener {
public:
    CUDPListener();

    ~CUDPListener();

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
    /*!
     * @private Метод обработки ответа
     * @param receivedBytes - количество полученных байт (out)
     * @param receivedBuf - массив данных, содержащий ответ (out)
     * @param clientInfo - структура содержащая основную информацию о подключенном клиенте (out)
     * @return true - данные получены успешно, инче false
     * @note осуществляет обработку полученного ответа по сети
     */
    bool getAnswer(std::uint32_t &receivedBytes, char *receivedBuf, sockaddr_in &clientInfo);

    /*!
     * @private Метод отправки данных
     * @param msg - массив отправляемых данных
     * @param clientInfo - структура содержащая основную информацию о подключенном клиенте (out)
     * @return true - данные отправленны успешно, иначе false
     * @note осуществляет отправку обработанных данных объекта в пункт назначения
     */
    uint32_t sendMessage(const char *msg, const sockaddr_in &clientInfo);
};
#endif /* SERVER_UDP_H */
