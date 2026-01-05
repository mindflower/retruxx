#pragma once
#include <map>
#include <renderer/i_renderer.h>

class ObjectsIcons
{
public:
    class Icon
    {
    public:
        Icon();
        int SetIco(int, CStr const&);
        m3d::rend::TexHandle GetIco(int) const;
        ~Icon();

    private:
        m3d::rend::TexHandle m_ico[2];
    };

public:
    ObjectsIcons();
    void Clear(bool);
    m3d::rend::TexHandle GetIcoByName(CStr const&, int) const;
    ~ObjectsIcons();
    int Load(CStr const&, bool);

private:
    int AddIcon(CStr const&, Icon*, bool);

private:
    std::map<CStr, Icon*> m_globalIcons;
    std::map<CStr, Icon*> m_levelIcons;
    CStr m_levelName;
};
