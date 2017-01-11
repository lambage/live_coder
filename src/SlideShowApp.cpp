#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Utilities.h"
#include "cinder/Json.h"
#include "SlideShow.h"

using namespace ci;
using namespace ci::app;

class SlideShowApp : public App {
  public:
	void setup() override;
    void keyDown(KeyEvent event) override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

private:
    std::unique_ptr<SlideShow> m_slide_show;
};



void SlideShowApp::setup()
{
    m_slide_show = std::make_unique<SlideShow>();
}

void SlideShowApp::keyDown(KeyEvent event)
{
    if (event.getChar() == KeyEvent::KEY_ESCAPE)
    {
        quit();
    }
    else if (event.getCode() == KeyEvent::KEY_RIGHT)
    {
        m_slide_show->Next();
    }
    else if (event.getCode() == KeyEvent::KEY_LEFT)
    {
        m_slide_show->Previous();
    }
    else if (event.getCode() == KeyEvent::KEY_F1)
    {
        app::setFullScreen(app::isFullScreen() ^ true);
    }
}

void SlideShowApp::mouseDown( MouseEvent event )
{
}

void SlideShowApp::update()
{
    if (m_slide_show != nullptr)
    {
        m_slide_show->Update();
    }
}

void SlideShowApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 

    if (m_slide_show != nullptr)
    {
        m_slide_show->Render();
    }
}

CINDER_APP( SlideShowApp, RendererGl )
