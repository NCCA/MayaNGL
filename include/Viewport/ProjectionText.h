#pragma once

#include <ngl/Text.h>


class ProjectionText
{
    private:
        typedef std::unique_ptr<ngl::Text> TextPtr;

    private:
        const int &screenWidth;
        const int &screenHeight;

    private:
        TextPtr m_label;

    public:
        std::string title;

    public:
        ProjectionText( const int &screenWidth_,
                        const int &screenHeight_ );

        void initialize();
        void update();

        ~ProjectionText() noexcept = default;
};
