
#include "Viewport/ProjectionText/ProjectionText.h"


ProjectionText::ProjectionText() : m_screen_width(0),
                                   m_screen_height(0),
                                   m_label(),
                                   title("persp")
{;}

void ProjectionText::initialize()
{
    m_label = std::make_unique<ngl::Text>(QFont("Helvetica",10,QFont::Bold));
    m_label->setColour(1.f,1.f,1.f);
}

void ProjectionText::resize(int w_, int h_)
{
    m_screen_width = w_;
    m_screen_height = h_;
    m_label->setScreenSize(m_screen_width,m_screen_height);
}

void ProjectionText::draw() const
{
    m_label->renderText(m_screen_width*0.5f-(title.length()*2.f),m_screen_height-25.f,title.c_str());
}
