#pragma once

class CRefTime
{
public:
    class CRefTime & operator+=(class CRefTime const &);
    class CRefTime & operator-=(class CRefTime const &);
    CRefTime(__int64);
    CRefTime(long);
    CRefTime();
    operator __int64() const ;
    class CRefTime & operator=(class CRefTime const &);
    class CRefTime & operator=(__int64);
protected:
private:
    __int64 m_time;
};
