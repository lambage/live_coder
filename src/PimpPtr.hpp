#pragma once

template<typename T>
class PimplPtr
{
public:
    PimplPtr(T* impl = nullptr) : impl(impl) {
    }

    PimplPtr(PimplPtr const&) = delete;
    PimplPtr& operator=(PimplPtr const&) = delete;

    PimplPtr(PimplPtr&& other) : impl(other.impl) {
        other.impl = nullptr;
    }

    PimplPtr& operator=(PimplPtr&& other) {
        impl = other.impl;
        other.impl = nullptr;
        return *this;
    }

    ~PimplPtr() {
        delete impl;
    }

    T const& operator*() const {
        return *impl;
    }

    T& operator*() {
        return *impl;
    }

    T const* operator->() const {
        return impl;
    }

    T* operator->() {
        return impl;
    }

private:
    T* impl;
};

template <typename T, typename ... Args>
PimplPtr<T> make_pimpl(Args... args)
{
    return new T(std::forward<Args>(args)...);
}