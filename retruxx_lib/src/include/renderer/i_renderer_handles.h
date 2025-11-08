#pragma once
#include <stdexcept>

namespace m3d
{
    namespace rend
    {
        template<class T>
        class Handle
        {
        protected:
            int m_handle = -1;

        public:
            Handle(Handle const& rhs)
            {
                m_handle = rhs.m_handle;
            }

            Handle() {}

            void SetInvalid()
            {
                m_handle = -1;
            }
            
            bool IsValid() const
            {
                return m_handle >= 0;
            }
            
            bool operator==(Handle<T> const& a) const
            {
                return m_handle == a.m_handle;
            }

            bool operator!=(Handle<T> const& a) const
            {
                return m_handle != a.m_handle;
            }
            
            //Handle<T>& operator=(Handle<T> const&)
            //{
            //    RETRUXX_NOT_IMPLEMENTED;
            //}
        };

        template<class T>
        class InvalidHandle
        {
        };
    }
}
