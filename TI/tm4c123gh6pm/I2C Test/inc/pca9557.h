class PCA9557
{
    public:
        enum IO_LEVEL
        {
            LOW = 0,
            HIGH = 1
        };

        enum IO_DIR
        {
            OUT = 0,
            IN = 1
        };

        PCA9557();
        PCA9557( unsigned char );

        void setDir( IO_DIR, IO_DIR, IO_DIR, IO_DIR, IO_DIR, IO_DIR, IO_DIR, IO_DIR );
        void setDir( unsigned char, IO_DIR );

        void write( IO_LEVEL, IO_LEVEL, IO_LEVEL, IO_LEVEL, IO_LEVEL, IO_LEVEL, IO_LEVEL, IO_LEVEL);
        void write( unsigned char, IO_LEVEL );

        unsigned char read( unsigned char );
        unsigned char read( void );


    private:
        unsigned char addr;
        unsigned char data[2];

        unsigned char _out;
        unsigned char _dir;
};
