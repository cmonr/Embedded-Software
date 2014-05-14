class CubicStepper {
protected:
    float a0, a1, a2, a3;

public:
    CubicStepper(float p0, float p1, float steps);
    CubicStepper() {}
    float f(float t);
};

