#pragma once

class Servo
{
  public:
    virtual void set( float ) = 0;
    virtual void setLimits( float, float ) = 0;
    virtual void invert( void ) = 0;
    virtual void enable( void ) = 0;
    virtual void disable( void ) = 0;
};
