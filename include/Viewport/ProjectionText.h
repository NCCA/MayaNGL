#pragma once

#include <ngl/Text.h>


class ProjectionText
{
    private:
        typedef std::unique_ptr<ngl::Text> TextPtr;

    private:
        const unsigned &screenWidth;
        const unsigned &screenHeight;

    private:
        TextPtr m_label;

    public:
        std::string title;

    public:
        ProjectionText( const unsigned &screenWidth_,
                        const unsigned &screenHeight_ );

        void initialize();
        void update();

        ~ProjectionText() noexcept = default;
};
