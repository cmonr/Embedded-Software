void initServos( void );

void setServo( unsigned char, float );

void invertServo( unsigned char );

float getServoDuty( unsigned char );

void setServoLimits( unsigned char, float, float );

void WTimer5AIntHandler( void );

void WTimer5BIntHandler( void );
