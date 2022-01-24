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
            
            bool operator==(Handle<T> const&) const
            {
                throw std::logic_error("Not implemented");
            }

            bool operator!=(Handle<T> const&) const
            {
                throw std::logic_error("Not implemented");
            }
            
            //Handle<T>& operator=(Handle<T> const&)
            //{
            //    throw std::logic_error("Not implemented");
            //}
        };

        template<class T>
        class InvalidHandle
        {
        };
    }
}
