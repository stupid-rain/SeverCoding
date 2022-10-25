#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <cstdio>

int main()
{
    char *szValue1 = inet_ntoa("1.2.3.4");
    char *szValue2 = inet_ntoa("10.194.71.60");
    printf("address 1:%s\n",szValue1);
    printf("address 2:%s\n", szValue2);
    return 0;
}