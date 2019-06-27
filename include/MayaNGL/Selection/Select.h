#pragma once

#include "Base_Selection.h"


template<bool visualize_bv_and_ray = false>
struct Select final : Base_Selection<visualize_bv_and_ray>
{

    struct SelectablePrim
    {
        std::string primitive;
        mc::Transform transform;

        void draw() const
        {
            ngl::ShaderLib *shader = ngl::ShaderLib::instance();
            shader->use(ngl::nglColourShader);
            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

//                auto MVP = projection * view * transform;
//                shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(0.263f,1.f,0.639f,1.f));
            prim->draw(primitive);

            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
    };


    typedef std::unordered_map<std::size_t,SelectablePrim> Selectables;

    Selectables m_selectables;


    private:
        bool m_multi_selection;

    private:
        bool alreadySelected(std::size_t id_) const;
        std::size_t getSelectedId(float &shortest_distance_) const;

    public:
        template<typename C>
        explicit Select( /*const*/ mc::View &view_,
                         /*const*/ mc::Projection &projection_,
                         const C &camera_ );

        template<typename T>
        void make_selectable(std::size_t id_, T &&prim_, const mc::Transform &transform_)
        {
//            m_selectables.emplace(id_,SelectablePrim{std::forward<T>(prim_),transform_});
        }

        void resize(int w_, int h_);
        void enableMultiSelection();
        mc::Position clickedOnObject(const SelectablePrim &selectable_) const;
        void pick(int mouse_x, int mouse_y);

        ~Select() noexcept = default;
};


// variant using void pointers = https://rextester.com/YPB83425
// variant using unique pointers = https://rextester.com/FVWQ1000
