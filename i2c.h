#ifndef _I2C_H
#define _I2C_H

#include "system.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


typedef enum
{
    /* No Error */
    I2C_ERROR_NONE,

    /* Slave returned Nack */
    I2C_ERROR_NACK,

    /* Bus Collision Error */
    I2C_ERROR_BUS_COLLISION,

} I2C_ERROR;


typedef enum
{
    I2C_TRANSFER_TYPE_WRITE = 0,

    I2C_TRANSFER_TYPE_READ

}I2C_TRANSFER_TYPE;


typedef enum
{
    I2C_STATE_START_CONDITION,

    I2C_STATE_ADDR_BYTE_1_SEND,

    I2C_STATE_ADDR_BYTE_2_SEND,

    I2C_STATE_READ_10BIT_MODE,

    I2C_STATE_ADDR_BYTE_1_SEND_10BIT_ONLY,

    I2C_STATE_WRITE,

    I2C_STATE_READ,

    I2C_STATE_READ_BYTE,

    I2C_STATE_WAIT_ACK_COMPLETE,

    I2C_STATE_WAIT_STOP_CONDITION_COMPLETE,

    I2C_STATE_IDLE,

} I2C_STATE;


typedef void (*I2C_CALLBACK) (uintptr_t contextHandle);

typedef struct
{
    uint16_t                address;
    uint8_t*                writeBuffer;
    uint8_t*                readBuffer;
    size_t                  writeSize;
    size_t                  readSize;
    size_t                  writeCount;
    size_t                  readCount;
    bool                    forcedWrite;
    I2C_TRANSFER_TYPE       transferType;
    I2C_STATE               state;
    I2C_ERROR               error;
    I2C_CALLBACK            callback;
    uintptr_t               context;

} I2C_OBJ;


typedef struct
{
    /* I2C Clock Speed */
    uint32_t clkSpeed;

} I2C_TRANSFER_SETUP;


void I2C2_Initialize(void);
bool I2C2_Read(uint16_t address, uint8_t *pdata, size_t length);
bool I2C2_Write(uint16_t address, uint8_t *pdata, size_t length);
bool I2C2_WriteRead(uint16_t address, uint8_t* wdata, size_t wlength, uint8_t* rdata, size_t rlength);
bool I2C2_IsBusy(void);
I2C_ERROR I2C2_ErrorGet(void);
void I2C2_CallbackRegister(I2C_CALLBACK callback, uintptr_t contextHandle);
bool I2C2_TransferSetup(I2C_TRANSFER_SETUP* setup, uint32_t srcClkFreq );

void I2C_2_InterruptHandler( void );

#endif