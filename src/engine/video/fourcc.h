#pragma once

class FOURCCMap :  public _GUID
{
public:
    FOURCCMap(unsigned long);
    void SetFOURCC(unsigned long);
protected:
private:
    void InitGUID();
};
