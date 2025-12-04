#pragma once

class GraphInfo
{
public:
    void AddFiltred(float,int);
    GraphInfo();
    float GetValue(int);
    void AddValue(float);
    float GetAverValue();
protected:
private:
    float valArray[1000];
    int fillOffset;
    float averAll;
    float devider;
    float averVal;
};
