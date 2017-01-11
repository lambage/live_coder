#pragma once
#include "LiveCodingShader.h"
#include <cinder/gl/Batch.h>
#include <cinder/Font.h>
#include "TextureDirectory.h"
#include "LiveFile.h"
#include "Settings/Settings.hpp"

class SlideShow
{
public:
    SlideShow();
    ~SlideShow();

    void Next();
    void Previous();

    void Update();
    void Render() const;

private:
    void ProcessSettingsFile(LiveFile * settings_file);

    ci::CameraOrtho m_camera;
    ci::gl::BatchRef m_rect;

    LiveCodingShaderRef m_live_shader;
    TextureDirectoryRef m_texture_directory;

    ci::Font m_font;

    std::unique_ptr<::Settings::Settings> m_settings;
    std::unique_ptr<LiveFile> m_settings_file;
};

