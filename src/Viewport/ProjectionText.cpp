
#include "Viewport/ProjectionText.h"


ProjectionText::ProjectionText( const unsigned &screenWidth_,
                                const unsigned &screenHeight_ )
                                :
                                screenWidth(screenWidth_),
                                screenHeight(screenHeight_),
                                m_label(),
                                title("persp")
{;}

void ProjectionText::initialize()
{
    m_label = std::make_unique<ngl::Text>(QFont("Helvetica",10,QFont::Bold));
    m_label->setColour(1.f,1.f,1.f);
}

void ProjectionText::update()
{
    // could use a function that updates the font size at run-time.
    m_label->setScreenSize(screenWidth,screenHeight);
    m_label->renderText(screenWidth*0.5-(title.length()*2),screenHeight-50,title.c_str());
}
