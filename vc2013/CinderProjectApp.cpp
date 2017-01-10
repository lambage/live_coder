#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Json.h"
#include "AppImageSource.h"

using namespace ci;
using namespace ci::app;

class CinderProjectApp : public App {
  public:
	void setup() override;
    void keyDown(KeyEvent event) override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
    std::unique_ptr<AppImageSource> image_source = nullptr;
};

void CinderProjectApp::setup()
{
    image_source = std::make_unique<AppImageSource>("C:/Users/klamb/Pictures/");

    auto settings_json = getAssetPath("settings.json").string();
    JsonTree json(loadFile(settings_json));

    auto& system = json["system"];

    auto width = system["width"].getValue<int>();
    auto height = system["height"].getValue<int>();

}

void CinderProjectApp::keyDown(KeyEvent event)
{
    if (event.getChar() == KeyEvent::KEY_ESCAPE)
    {
        quit();
    }
    else if (event.getCode() == KeyEvent::KEY_RIGHT)
    {
        image_source->Next();
    }
    else if (event.getCode() == KeyEvent::KEY_LEFT)
    {
        image_source->Previous();
    }
    else if (event.getCode() == KeyEvent::KEY_F1)
    {
        app::setFullScreen(app::isFullScreen() ^ true);
    }
}

void CinderProjectApp::mouseDown( MouseEvent event )
{
}

void CinderProjectApp::update()
{
    image_source->Update();
}

void CinderProjectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 

    if (image_source != nullptr)
    {
        image_source->Render();
    }
}

CINDER_APP( CinderProjectApp, RendererGl )
