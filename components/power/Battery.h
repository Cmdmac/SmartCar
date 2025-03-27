#pragma once

class Battery
{
private:
    int estimateCapacity(float voltage);

    int detectPin;
    int percent;
public:
    Battery(int detectPin);
    ~Battery();
    
    void setup() {
        
    }

    int getPercent() {
        return this->percent;
    }
    
    int detect();
};

Battery::Battery(int detectPin) : detectPin(detectPin), percent(0)
{
}

Battery::~Battery()
{
}
