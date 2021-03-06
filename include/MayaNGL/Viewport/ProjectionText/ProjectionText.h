#pragma once

#include "MayaNGL/Common/Common_Def.hpp"


class ProjectionText
{
    private:
        typedef std::unique_ptr<ngl::Text> TextPtr;

    private:
        int m_screen_width;
        int m_screen_height;
        TextPtr m_label;
        std::string m_title;

    public:
        explicit ProjectionText();

        template<mc::CamView>
        void view_title();

        void initialize();
        void resize(int w_, int h_);
        void draw() const;

        ~ProjectionText() noexcept = default;
};
