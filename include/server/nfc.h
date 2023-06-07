#ifndef _NFC
#define _NFC

#include <stdio.h>
#include <stdlib.h>
#include <winscard.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

SCARDCONTEXT applicationContext;

SCARDHANDLE connectionHandler;
DWORD activeProtocol;
extern int card_arr[4];

void init_NFC();

void establishContext();
void listReaders();
void freeListReader();
void releaseContext();
bool connectToCard();
void disconnectFromCard();
void getCardInformation();
int sendCommand(uint8_t command[], unsigned short commandLength);
#endif