#include <string.h>
#include <stdio.h>
#include "time.h"
#include "data_protocol.h"

struct {
    char *key;
    void *data;
    char* (*handler)(void*);
    unsigned int keylen;
} typedef tPub;

struct {
    char *key;
    void *data;
    void (*handler)(void*,char*);
    unsigned int keylen;
} typedef tSub;

static tPub pubBuff[MAX_PUBLISHERS];
static tSub subBuff[MAX_SUBSCRIBERS];
static char keyBuff[KEY_BUFF_SIZE];

static unsigned int pubCount = 0,
                    subCount = 0,
                    keyBuffCount = 0;

// This is so it'll be easy to change how messages are sent
static void printError(char* msg) {
    printf("{\"error\":\"%s\"}\n", msg);
}

static void printMessage(char* msg) {
    printf("%s", msg);
}

static int checkAndCopyKey(char *jsonkey, char **keyPtr, unsigned int *keyLenPtr) {
    unsigned int keyLen = strnlen(jsonkey, MAX_KEY_LEN); 
    char *keyCopy = keyBuff + keyBuffCount;
    
    if (keyLen >= MAX_KEY_LEN) {
        printError("key exceeds max length"); 
        return 0;
    }
    
    if (keyBuffCount + keyLen >= KEY_BUFF_SIZE) {
        printError("key cannot fit in buffer"); 
        return 0;
    }
    
    strcpy(keyCopy, jsonkey);
    keyBuffCount += keyLen + 1; // 1 for the null terminator

    *keyPtr = keyCopy;
    *keyLenPtr = keyLen;

    return 1;
}

// Adds a handler that will be called to create a string value for the given key 
// (this will deepcopy jsonkey to an internal buffer)
void AddPublisher(char *jsonkey, void *data, char* (*handler)(void*)) {
    char *keyCopy;
    int success = 0;
    
    if (pubCount >= MAX_PUBLISHERS) {
        printError("publisher limit already reached");
        return;
    }
    
    success = checkAndCopyKey(
        jsonkey, 
        &pubBuff[pubCount].key, 
        &pubBuff[pubCount].keylen
        );

    if (!success) {
        return;
    }

    pubBuff[pubCount].data = data;
    pubBuff[pubCount].handler = handler;

    pubCount += 1;
}

// Add a handler to be called whenever the given key is found in received messages
// (this will deepcopy jsonkey to an internal buffer)
void AddSubscriber(char *jsonkey, void *data, void (*handler)(void*,char*)) {
    char *keyCopy;   
    int success = 0;
 
    if (subCount >= MAX_SUBSCRIBERS) {
        printError("subscriber limit already reached");
        return;
    }
    
    success = checkAndCopyKey(
        jsonkey, 
        &subBuff[subCount].key, 
        &subBuff[subCount].keylen 
        );

    if (!success) {
        return;
    }
    
    subBuff[subCount].data = data;
    subBuff[subCount].handler = handler;

    subCount += 1;
}

const int elemExtra = 6, // 6 for the comma (or beginning bracket), the colon, and four quotes
          msgExtra = 3; // 3 for the closing bracket, ending newline, and null terminator

void createAndPublishMessage(void *data) {
    int i,  
        totalLen = 0;
    char msgBuff[MAX_MSG_SIZE];
    char *msgPtr = msgBuff,
         *format1 = "{\"%s\":\"%s\"",
         *format2 = ",\"%s\":\"%s\"",
         *format = format1;

    for (i = 0; i < pubCount; i++) {
        char *key = pubBuff[i].key,
             *value = pubBuff[i].handler(pubBuff[i].data);
        unsigned int valuelen = strnlen(value, MAX_VAL_LEN),
                     len = pubBuff[i].keylen + valuelen + elemExtra; 

        if (valuelen >= MAX_VAL_LEN) {
            printError("data value string returned by publisher exceeds maximum length");
            return;
        }

        if (MAX_MSG_SIZE < totalLen + len + msgExtra) {
            printError("message exceeds buffer size");
            return;
        }

        sprintf(
            msgPtr, 
            format, 
            pubBuff[i].key, 
            pubBuff[i].handler(pubBuff[i].data)
            );

        format = format2;
        totalLen += len;
        msgPtr += len;
    }

    if (pubCount == 0) {
        sprintf(msgPtr, "{}\n");
    } else {
        sprintf(msgPtr, "}\n");
    }

    printMessage(msgBuff); 
}

// Begins calling all pub handlers and encoding data every 'period' seconds
void BeginPublishing(float period) {
    CallEvery(createAndPublishMessage, 0, period);
}

// Begins parsing messages and calling sub handlers in a blocking loop
void BeginSubscribing() {

}

