#include "LiveCodingShader.h"

#include <experimental/filesystem>
#include <cinder/app/AppBase.h>
#include "LiveFile.h"

struct LiveCodingShader::Impl
{
    Impl(std::string vertex_shader, std::string fragment_shader, std::string geometry_shader)
        : vertex_shader(vertex_shader)
        , fragment_shader(fragment_shader)
        , geometry_shader(geometry_shader)
    {
        Load();
    }
    ~Impl(){}
    
    bool Update()
    {
        for (auto& file : shader_dependencies)
        {
            if (file.IsExpired())
            {
                Load();
                return true;
            }
        }
        if (first_time_called)
        {
            first_time_called = false;
            return true;
        }
        return false;
    }

    void AddShaderIfNotEmpty(const std::string& shader_file)
    {
        if (!shader_file.empty())
        {
            shader_dependencies.emplace_back(ci::app::getAssetPath(shader_file));
        }
    }

    static cinder::DataSourceRef TryLoadAsset(const std::string& asset)
    {
        try
        {
            if (asset.empty())
            {
                return {};
            }
            return ci::app::loadAsset(asset);
        }
        catch(...)
        {
            return {};
        }
    }

    void Load()
    {
        shader_error.clear();
        shader_dependencies.clear();
        AddShaderIfNotEmpty(vertex_shader);
        AddShaderIfNotEmpty(fragment_shader);
        AddShaderIfNotEmpty(geometry_shader);

        try
        {
            program = ci::gl::GlslProg::create(TryLoadAsset(vertex_shader), TryLoadAsset(fragment_shader), TryLoadAsset(geometry_shader));
            AddIncludedShaderDependencies();
        }
        catch (std::exception &ex)
        {
            shader_error = ex.what();
        }
    }

    void AddIncludedShaderDependencies()
    {
        auto files = program->getIncludedFiles();
        for (auto& file : files)
        {
            shader_dependencies.emplace_back(LiveFile(file));
        }
    }

    std::string vertex_shader;
    std::string fragment_shader;
    std::string geometry_shader;

    ci::gl::GlslProgRef program;
    std::vector<LiveFile> shader_dependencies;
    std::string shader_error;
    bool first_time_called = true;
};

LiveCodingShader::LiveCodingShader(LiveCodingShaderMaker const& maker)
    : m_impl(make_pimpl<Impl>(maker.vertex_shader, maker.fragment_shader, maker.geometry_shader))
{
    on_update_function = maker.on_updated_function;
}


LiveCodingShader::~LiveCodingShader()
{
}

ci::gl::GlslProgRef LiveCodingShader::GetProgram() const
{
    return m_impl->program;
}

void LiveCodingShader::RaiseOnUpdateEvent() const
{
    if (on_update_function != nullptr)
    {
        on_update_function();
    }
}

void LiveCodingShader::Update()
{
    if (m_impl->Update())
    {
        RaiseOnUpdateEvent();
    }
}

std::string LiveCodingShader::GetErrorString() const
{
    return m_impl->shader_error;
}
