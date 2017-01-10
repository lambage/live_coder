#include "AppImageSource.h"
#include <cinder/gl/scoped.h>
#include <cinder/app/AppBase.h>
#include <cinder/gl/draw.h>
#include <cinder/gl/Batch.h>
#include <experimental/filesystem>
#include "LiveCodingShader.h"
#include "TextureDirectory.h"

using namespace ci;
namespace efs = std::experimental::filesystem;

struct AppImageSource::Impl
{
    explicit Impl(std::string directory)
    {
        m_camera.setOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        mFont = Font("Times New Roman", 24);
        
        m_live_shader = LiveCodingShaderMaker()
            .FragmentShader(fragment_shader_filename)
            .VertexShader(vertex_shader_filename)
            .OnUpdatedHandler([this]() { mRect = CreateDrawingRectangle(); })
            .Create();

        m_texture_directory = TextureDirectory::Create(directory);
    }

    ~Impl()
    {
        
    }

    ci::gl::BatchRef CreateDrawingRectangle() const
    {
        auto rect = geom::Rect(Rectf(-1.0f, -1.0f, 1.0f, 1.0f)).texCoords(vec2(0.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f), vec2(0.0f, 0.0f));
        return gl::Batch::create(rect, m_live_shader->GetProgram());
    }

    void Render() const
    {
        auto window_size = app::getWindowSize();
        gl::ScopedViewport scopedViewport(ivec2(0), window_size);

        gl::setMatrices(m_camera);
        if (mRect && m_live_shader && m_texture_directory)
        {
            auto glsl = m_live_shader->GetProgram();

            m_texture_directory->Bind(glsl, 0, 1);
            
            mRect->draw();
        }

        auto shader_error = m_live_shader->GetErrorString();
        if (!shader_error.empty())
        {
            auto color = ColorA(1.0f, 0.15f, 0.15f, 1.0f);

            gl::setMatricesWindow(app::getWindowSize());
            gl::drawString(shader_error, vec2(0.0f, 0.0f), color, mFont);
        }
    }

    void Next() 
    {
        if (m_texture_directory)
        {
            m_texture_directory->Next();
        }
    }

    void Previous() 
    {
        if (m_texture_directory)
        {
            m_texture_directory->Previous();
        }
    }

    void Update() 
    {
        if (m_texture_directory)
        {
            m_live_shader->Update();
        }
    }

    ci::CameraOrtho m_camera;
    ci::gl::BatchRef mRect;

    LiveCodingShaderRef m_live_shader;
    TextureDirectoryRef m_texture_directory;

    const std::string vertex_shader_filename = "vertex.glsl";
    const std::string fragment_shader_filename = "fragment.glsl";

    ci::Font mFont;
};

AppImageSource::AppImageSource(std::string path) : m_impl(make_pimpl<Impl>(path))
{
}

AppImageSource::~AppImageSource()
{
}

void AppImageSource::Next() 
{
    m_impl->Next();
}

void AppImageSource::Previous() 
{
    m_impl->Previous();
}

void AppImageSource::Update()
{
    m_impl->Update();
}

void AppImageSource::Render() const
{
    m_impl->Render();
}
