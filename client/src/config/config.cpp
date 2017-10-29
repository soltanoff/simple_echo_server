#include "config.h"

/*!
 * @private Метод осуществляющий контроль ввода ответа пользователя на булевый вопрос
 * @param answer - ответ пользователя на вопрос [Y,N]
 * @return None
 * @note организовывается ввод ответа пользователя и дальнейшая его проверка ответа на корректность
 */
void answerControl(char *answer, const char var_1, const char var_2) {
    while (true) {
        std::cin.clear();
        std::cin.sync();
        std::cin.getline(answer, std::numeric_limits<std::uint32_t>::max(), '\n');
        if ((strlen(answer) > 1 ||
             (answer[0] != var_1 && answer[0] != var_2 && answer[0] != tolower(var_1) && answer[0] != tolower(var_2)))) {
            std::cout << "Error, try again: [" << var_1 << "/" << var_2 << "] ";
            continue;
        } else break;
    }
}

/*!
 * @private Метод осуществляющий контроль ввода ip-адреса сервера
 * @param ip - корректно введенный пользователем ip-адрес (out)
 * @return None
 * @note организовывается ввод ip-адреса и дальнейшая его проверка на корректность
 */
void ipAddressControl(char *ip) {
    while (true) {
        std::cin.getline(ip, 17, '\n');
        if (inet_addr(ip) == INADDR_NONE) {
            std::cout << "Error, try again: ";
            continue;
        } else break;
    }
}