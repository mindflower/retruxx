#pragma once

namespace m3d
{
    namespace rend
    {
        template<class T>
        class Handle
        {
        public:
            void SetInvalid();
            bool IsValid() const;
            bool operator==(Handle<T> const&) const;
            Handle& operator=(Handle<T> const&);

        private:
            int m_handle;
        };

        template<class T>
        class InvalidHandle
        {
        };
    }
}
