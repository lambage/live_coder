#include "SlideShow.h"
#include <cinder/app/AppBase.h>
#include <cinder/gl/scoped.h>
#include <cinder/gl/draw.h>

using namespace ci;

const std::string vertex_shader_filename = "vertex.glsl";
const std::string fragment_shader_filename = "fragment.glsl";
const std::string slide_show_settings_filename = "settings.json";

ci::gl::BatchRef CreateDrawingRectangle(LiveCodingShader * shader)
{
    auto rect = cinder::geom::Rect(Rectf(-1.0f, -1.0f, 1.0f, 1.0f)).texCoords(vec2(0.0f, 1.0f), vec2(1.0f, 1.0f), vec2(1.0f, 0.0f), vec2(0.0f, 0.0f));
    return gl::Batch::create(rect, shader->GetProgram());
}

template<typename T>
bool IsSettingsNew(T* settings, T* old_settings)
{
    return old_settings == nullptr || *settings != *old_settings;
}

SlideShow::SlideShow()
{
    m_camera.setOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    m_font = Font("Times New Roman", 24);

    m_live_shader = LiveCodingShaderMaker()
        .FragmentShader(fragment_shader_filename)
        .VertexShader(vertex_shader_filename)
        .OnUpdatedHandler([this]() { m_rect = CreateDrawingRectangle(m_live_shader.get()); })
        .Create();

    m_settings_file = std::make_unique<LiveFile>(app::getAssetPath(slide_show_settings_filename));

    ProcessSettingsFile(m_settings_file.get());
   
}

SlideShow::~SlideShow()
{
}

void SlideShow::Next()
{
    if (m_texture_directory)
    {
        m_texture_directory->Next();
    }
}

void SlideShow::Previous()
{
    if (m_texture_directory)
    {
        m_texture_directory->Previous();
    }
}

void SlideShow::Update()
{
    if (m_live_shader)
    {
        m_live_shader->Update();
    }
}

void SlideShow::Render() const
{
    auto window_size = app::getWindowSize();
    gl::ScopedViewport scopedViewport(ivec2(0), window_size);

    gl::setMatrices(m_camera);
    if (m_rect && m_live_shader && m_texture_directory)
    {
        auto glsl = m_live_shader->GetProgram();

        m_texture_directory->Bind(glsl, 0, 1);

        m_rect->draw();
    }

    auto shader_error = m_live_shader->GetErrorString();
    if (!shader_error.empty())
    {
        auto color = ColorA(1.0f, 0.15f, 0.15f, 1.0f);

        gl::setMatricesWindow(app::getWindowSize());
        gl::drawString(shader_error, vec2(0.0f, 0.0f), color, m_font);
    }
}

void SlideShow::ProcessSettingsFile(LiveFile * settings_file)
{
    // m_texture_directory = TextureDirectory::Create(directory);

    JsonTree json(loadFile(settings_file->GetPath()));

    auto settings = std::make_unique<::Settings::Settings>(json);

    //auto system_settings = std::make_unique<::Settings::System>(json["system"]);
    //auto slide_show_settings = std::make_unique<::Settings::SlideShow>(json["slide_show"]);
    //auto data_import_settings = std::make_unique<::Settings::DataImports>(json["data_imports"]);

    if (!m_settings)
    {
        m_settings = std::move(settings);
    }

    auto slide_show_directory = m_settings->GetSlideShow().GetDirectory();
    if (slide_show_directory.empty()) slide_show_directory = (getHomeDirectory() / "Pictures").string();
    m_texture_directory = TextureDirectory::Create(slide_show_directory);
    
    //if (IsSettingsNew(system_settings.get(), m_system_settings.get()))
    //{
    //    m_system_settings = std::move(system_settings);
    //}

    //if (IsSettingsNew(slide_show_settings.get(), m_slide_show_settings.get()))
    //{
    //    m_slide_show_settings = std::move(slide_show_settings);

    //    auto slide_show_directory = m_slide_show_settings->GetDirectory();
    //    if (slide_show_directory.empty()) slide_show_directory = (getHomeDirectory() / "Pictures").string();

    //    m_image_source = std::make_unique<AppImageSource>(slide_show_directory);
    //}
}
