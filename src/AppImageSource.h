#pragma once
#include <string>
#include "PimpPtr.hpp"

class AppImageSource
{
public:
    explicit AppImageSource(std::string directory);
    ~AppImageSource();

    void Next();
    void Previous();

    void Update();
    void Render() const;
private:
    struct Impl;
    PimplPtr<Impl> m_impl;
};

