#include <nfc.h>
#include <stdint.h>
#include <unistd.h>

int card_arr[4] = {
    0x3cb4ea27,
    0x22721904,
    0x3cbafa97,
    0x2272d304,
};

LPSTR reader = NULL;

void init_NFC()
{
    establishContext();
    listReaders();
}

void destruct_NFC()
{
    freeListReader();
    releaseContext();
}

void establishContext()
{
    LONG status = SCardEstablishContext(SCARD_SCOPE_SYSTEM, NULL, NULL, &applicationContext);
    if (status == SCARD_S_SUCCESS)
    {
        printf("Context established\n");
    }
    else
    {
        printf("Establish context error: %s\n", pcsc_stringify_error(status));
        exit(1);
    }
}
void listReaders()
{
    DWORD readers = SCARD_AUTOALLOCATE;
    LONG status = SCardListReaders(applicationContext, NULL, (LPSTR)&reader, &readers);

    if (status == SCARD_S_SUCCESS)
    {
        char *p = reader;
        while (*p)
        {
            printf("Reader found: %s\n", p);
            p += strlen(p) + 1;
        }
    }
    else
    {
        printf("List reader error: %s\n", pcsc_stringify_error(status));
        exit(1);
    }
}
void freeListReader()
{
    LONG status = SCardFreeMemory(applicationContext, reader);
    if (status == SCARD_S_SUCCESS)
    {
        printf("Reader list free\n");
    }
    else
    {
        printf("Free reader list error: %s\n", pcsc_stringify_error(status));
        exit(1);
    }
}

void releaseContext()
{
    LONG status = SCardReleaseContext(applicationContext);
    if (status == SCARD_S_SUCCESS)
    {
        printf("Context released\n");
    }
    else
    {
        printf("Release context error: %s\n", pcsc_stringify_error(status));
        exit(1);
    }
}
bool connectToCard()
{
    activeProtocol = -1;

    LONG status = SCardConnect(applicationContext, reader, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &connectionHandler, &activeProtocol);
    if (status == SCARD_S_SUCCESS)
    {
        return true;
    }
    else
    {
        // printf("Card connection: %s\n", pcsc_stringify_error(status));
        return false;
    }
}
void disconnectFromCard()
{
    LONG status = SCardDisconnect(connectionHandler, SCARD_LEAVE_CARD);
    if (status == SCARD_S_SUCCESS)
    {
        // printf("Disconnected from card\n");
    }
    else
    {
        printf("Card deconnection error: %s\n", pcsc_stringify_error(status));
        exit(1);
    }
}

void getCardInformation()
{
    BYTE ATR[MAX_ATR_SIZE] = "";
    DWORD ATRLength = sizeof(ATR);
    char readerName[MAX_READERNAME] = "";
    DWORD readerLength = sizeof(readerName);
    DWORD readerState;
    DWORD readerProtocol;

    LONG status = SCardStatus(connectionHandler, readerName, &readerLength, &readerState, &readerProtocol, ATR, &ATRLength);
    if (status == SCARD_S_SUCCESS)
    {
        printf("\n");
        printf("Name of the reader: %s\n", readerName);
        printf("ATR: ");
        for (int i = 0; i < ATRLength; i++)
        {
            printf("%02X ", ATR[i]);
        }
        printf("\n\n");
    }
    else
    {
        printf("Get card information error: %s\n", pcsc_stringify_error(status));
        exit(1);
    }
}

int sendCommand(uint8_t command[], unsigned short commandLength)
{
    const SCARD_IO_REQUEST *pioSendPci;
    SCARD_IO_REQUEST pioRecvPci;
    uint8_t response[300];
    unsigned long responseLength = sizeof(response);
    int ret;
    switch (activeProtocol)
    {
    case SCARD_PROTOCOL_T0:
        pioSendPci = SCARD_PCI_T0;
        break;
    case SCARD_PROTOCOL_T1:
        pioSendPci = SCARD_PCI_T1;
        break;
    default:
        printf("Protocol not found\n");
        exit(1);
    }

    LONG status = SCardTransmit(connectionHandler, pioSendPci, command, commandLength, &pioRecvPci, response, &responseLength);
    if (status == SCARD_S_SUCCESS)
    {
        memcpy(&ret, response, sizeof(int));
        return ret;
    }
    else
    {
        printf("Send command error: %s\n", pcsc_stringify_error(status));
        exit(1);
    }
}
