#pragma once

#include <cinder/gl/GlslProg.h>
#include "PimpPtr.hpp"

class LiveCodingShaderMaker;

class LiveCodingShader
{
public:
    explicit LiveCodingShader(LiveCodingShaderMaker const& maker);
    ~LiveCodingShader();

    void SetOnUpdatedHandler(std::function<void()> handler) { on_update_function = handler; }
   
    ci::gl::GlslProgRef GetProgram() const;

    void Update();
    std::string GetErrorString() const;

private:
    void RaiseOnUpdateEvent() const;
    std::function<void()> on_update_function;
    struct Impl;
    PimplPtr<Impl> m_impl;
};


using LiveCodingShaderRef = std::shared_ptr<LiveCodingShader>;

class LiveCodingShaderMaker
{
public:
    LiveCodingShaderMaker() {}
    ~LiveCodingShaderMaker() {}

    LiveCodingShaderMaker& VertexShader(std::string vertex_shader)
    {
        this->vertex_shader = vertex_shader;
        return *this;
    }

    LiveCodingShaderMaker& FragmentShader(std::string fragment_shader)
    {
        this->fragment_shader = fragment_shader;
        return *this;
    }

    LiveCodingShaderMaker& GeometryShader(std::string geometry_shader)
    {
        this->geometry_shader = geometry_shader;
        return *this;
    }

    LiveCodingShaderMaker& OnUpdatedHandler(std::function<void()> on_updated_function)
    {
        this->on_updated_function = on_updated_function;
        return *this;
    }

    LiveCodingShaderRef Create()
    {
        return std::make_shared<LiveCodingShader>(*this);
    }

private:
    friend class LiveCodingShader;
    std::function<void()> on_updated_function;
    std::string vertex_shader;
    std::string fragment_shader;
    std::string geometry_shader;
};
