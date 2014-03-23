#include <stdbool.h>

typedef struct
{
    unsigned char addr;
    unsigned char* data;
    volatile unsigned char len;

    bool writing;

    // typedef void (*callback) (void);

    volatile unsigned int error;

} tI2CRequest;


typedef struct
{
    tI2CRequest buff[16];
    unsigned char head;
    unsigned char tail;
    unsigned char len;
} tI2CRequestBuffer;


void I2CInit( void );

unsigned int I2CWrite_BusyWait( unsigned char, unsigned char*, unsigned char );
void I2CWrite_Async( unsigned char, unsigned char*, unsigned char );
unsigned int I2CRead_BusyWait( unsigned char, unsigned char*, unsigned char );
void I2CRead_Async( unsigned char, unsigned char*, unsigned char );

void I2C_buildRequest( unsigned char, unsigned char*, unsigned char, bool );
void I2C_startRequest( void );

void I2C0IntHandler( void );

