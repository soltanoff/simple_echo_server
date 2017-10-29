/* =====================================================================================================================
 * File: config.h
 * Description: Файл основных конфигураций сервера
 * Author: soltanoff
 * ================================================================================================================== */
#ifndef CONFIG_H
#define CONFIG_H
/* ================================================================================================================== */
#include <vector>
#include <time.h>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <mutex>
/* ================================================================================================================== */
#define SOCKET int
#define SOCKET_ERROR -1
//! основной мьютекс, синхронизирующий ввод и вывод данных в консоль сервера
extern std::mutex MAIN_MUTEX;
/*!
 * @namespace ServerCfg
 * Основные константы конфигурации сервера
 */
namespace ServerCfg {
    //! локальный ip-адрес сервера
    // const char LOCALHOST[] = "127.0.0.1";
    //! используемый порт сервера [TCP]
    const std::uint16_t TCP_PORT = 8082;
    //! тип протокола сервера [TCP]
    const std::uint16_t TCP_PROTOCOL = IPPROTO_TCP;
    //! используемый порт сервера [UDP]
    const std::uint16_t UDP_PORT = 8083;
    //! тип протокола сервера [UDP]
    const std::uint16_t UDP_PROTOCOL = IPPROTO_UDP;
    //! флаг ошибки открытия сокета (ошибка прослушивания)
    const std::uint16_t BACKLOG = 1;
    //! размер буфера отправляемых и получаемых данных
    const std::uint16_t BUFF_SIZE = 64000;
}
/*!
 * Метод осуществляющий поиск всех цифр от 0 до 9 в принятом сообщении и выводит в консоль:
 * сумму чисел;
 * все числа, в порядке убывания;
 * максимальное и минимальное значения.
 * @param receivedBuf - массив данных, содержащий ответ клиента
 */
void calculateMethod(const char *receivedBuf);
/* ================================================================================================================== */
#endif /* CONFIG_H */
