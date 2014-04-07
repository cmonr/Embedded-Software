class I2C
{
    public:
        I2C( tI2CPin_SDA, tI2CPin_SCL );

        void write( unsigned char, unsigned char, unsigned int);
        void read( unsigned char, unsigned char, unsigned int );
    private:
        unsigned int periph;
        unsigned int base;
}

typedef enum
{
    PB3 = 0,
    PA7,
    PE5,
    PD1
} tI2CPin_SDA;

typedef enum
{
    PB2 = 0,
    PA6,
    PE4,
    PD0
} tI2CPin_SCL;


typedef struct
{
    unsigned int periph;
    unsigned int base;
    tIOPin sda;
    unsigned int sda_cfg;
    tIOPin scl;
    unsigned int scl_cfg;

} tI2C_Obj;


// Yeah, I don't care about anything other than I2C0 right now...
tI2C _i2c[4] = {
    { SYSCTL_PERIPH_I2C0, I2C0_BASE, PB3, GPIO_PB3_I2C0SDA, PB2, GPIO_PB2_I2C0SCL },
    {0,0,PA7,0,PA6,0},
    {0,0,PE5,0,PE4,0},
    {0,0,PD1,0,PD0,0}
};
