#pragma once
#include <stdexcept>

namespace m3d
{
    namespace rend
    {
        template<class T>
        class Handle
        {
        public:
            void SetInvalid()
            {
                throw std::logic_error("Not implemented");
            }
            
            bool IsValid() const
            {
                throw std::logic_error("Not implemented");
            }
            
            bool operator==(Handle<T> const&) const
            {
                throw std::logic_error("Not implemented");
            }
            
            Handle<T>& operator=(Handle<T> const&)
            {
                throw std::logic_error("Not implemented");
            }

        private:
            int m_handle;
        };

        template<class T>
        class InvalidHandle
        {
        };
    }
}
