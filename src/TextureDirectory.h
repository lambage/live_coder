#pragma once
#include <cinder/gl/GlslProg.h>
#include "PimpPtr.hpp"

class TextureDirectory;
using TextureDirectoryRef = std::shared_ptr<TextureDirectory>;

class TextureDirectory
{
public:
    explicit TextureDirectory(std::string directory);
    ~TextureDirectory();

    void Next();
    void Previous();

    void Update();
    void Bind(ci::gl::GlslProgRef program, int front, int back = -1);

    static TextureDirectoryRef Create(std::string directory)
    {
        return std::make_shared<TextureDirectory>(directory);
    }

private:
    struct Impl;
   PimplPtr<Impl> m_impl;
    
    void SetCurrentTexture();
};




