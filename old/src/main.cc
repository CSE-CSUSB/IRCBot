#include "include/bot.h"

int main()
{
    Bot bot = Bot("NICK dallbot\r\n","USER dallbot a b :dallbee.com\r\n");
    bot.start();

    return 0;
}