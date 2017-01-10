#include "TextureDirectory.h"
#include <cinder/app/AppBase.h>
#include <cinder/gl/Texture.h>
#include <cinder/Timeline.h>
#include <experimental/filesystem>
#include "LiveFile.h"

using namespace ci;

struct TextureDirectory::Impl
{
    std::vector<LiveFile> files;
    std::vector<LiveFile>::iterator current_file;

    gl::TextureRef current_texture;
    gl::TextureRef back_texture;
    float lerp_value = 0.0f;
    Anim<float> animation;

    void SetCurrentTexture()
    {
        if (current_file != std::end(files))
        {
            auto duration = 1.0f;
            app::timeline().apply(&animation, 0.0f, 1.0f, duration, EaseOutCubic(), [this](const float& start, const float& end, float t) -> auto { return lerp_value = t; });
            back_texture = current_texture;
            current_texture = gl::Texture::create(cinder::loadImage(current_file->GetPath()), gl::Texture::Format().loadTopDown());
        }
        else
        {
            current_texture = nullptr;
        }
    }

    void Next()
    {
        ++current_file;
        if (current_file == std::end(files))
        {
            current_file = std::begin(files);
        }
        SetCurrentTexture();
    }

    void Previous()
    {
        if (current_file == std::begin(files))
        {
            current_file = --files.end();
        }
        else
        {
            --current_file;
        }
        SetCurrentTexture();
    }

    void Update()
    {
        animation();
    }

    void Bind(const gl::GlslProgRef& program, int front, int back) const
    {
        current_texture->bind(0);
        program->uniform("image", 0);
        if (back_texture)
        {
            back_texture->bind(1);
            program->uniform("back_image", 1);
        }
        program->uniform("lerp_value", lerp_value);
    }
};

TextureDirectory::TextureDirectory(std::string directory)
    : m_impl(make_pimpl<Impl>())
{
    std::set<std::string> target_extensions;
    target_extensions.insert(".JPG");
    target_extensions.insert(".JPEG");
    target_extensions.insert(".BMP");
    target_extensions.insert(".GIF");
    target_extensions.insert(".PNG");

    std::experimental::filesystem::path p(directory);
    for (auto& it : std::experimental::filesystem::directory_iterator(p))
    {
        if (std::experimental::filesystem::is_regular_file(it))
        {
            auto extension = it.path().extension().string();
            std::transform(std::begin(extension), std::end(extension), std::begin(extension), ::toupper);
            if (target_extensions.find(extension) != std::end(target_extensions))
            {
                m_impl->files.emplace_back(it);
            }
        }
    }

    m_impl->current_file = m_impl->files.begin();
    SetCurrentTexture();
}


TextureDirectory::~TextureDirectory()
{
    m_impl->current_texture = nullptr;
    m_impl->back_texture = nullptr;
}

void TextureDirectory::Next() 
{
    m_impl->Next();
}

void TextureDirectory::Previous() 
{
    m_impl->Previous();
}

void TextureDirectory::Update() 
{
    m_impl->Update();
}

void TextureDirectory::Bind(ci::gl::GlslProgRef program, int front, int back) 
{
    m_impl->Bind(program, front, back);
}

void TextureDirectory::SetCurrentTexture() 
{
    m_impl->SetCurrentTexture();
}
