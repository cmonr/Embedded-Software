class LED
{
    int width, height;
    public:
        void set( int, int );
        int area() {return width*height;}

};

void LED::set(int x, int y)
{
    width = x;
    height = y;
}

