
#include "Viewport/ProjectionText.h"


ProjectionText::ProjectionText( const int &screenWidth_,
                                const int &screenHeight_ )
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

void ProjectionText::resize()
{
    // could use a function that updates the font size at run-time.
    m_label->setScreenSize(screenWidth,screenHeight);
}

void ProjectionText::draw()
{
    m_label->renderText(screenWidth*0.5f-(title.length()*2.f),screenHeight-25.f,title.c_str());
}
