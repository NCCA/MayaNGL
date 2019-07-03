#pragma once

#include "Base_Selection.h"



struct TransformHandle
{
    private:
        typedef std::array<mc::Position,6> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &scene_view;
        const mc::Projection &projection;

    private:
        mc::Transform m_model;
        mc::View m_view;
        Vertices m_coordinates;
        VAOPtr m_vao;

    public:
        TransformHandle( const mc::View &view_,
                         const mc::Projection &projection_ )
                         :
                         scene_view(view_),
                         projection(projection_),
                         m_model(),
                         m_view(ngl::lookAt(mc::Position::in(),mc::Position::zero(),mc::Direction::up())),
                         m_coordinates{{
                                           mc::Direction::zero(),
                                           mc::Direction::right()*0.1f,
                                           mc::Direction::zero(),
                                           mc::Direction::up()*0.1f,
                                           mc::Direction::zero(),
                                           mc::Direction::in()*0.1f
                                      }},
                         m_vao()
        {;}

        void initialize()
        {
            m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);
            m_vao->bind();
            m_vao->setData(ngl::AbstractVAO::VertexData(m_coordinates.size()*sizeof(mc::Position),m_coordinates[0].m_x));
            m_vao->setNumIndices(m_coordinates.size());
            m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
            m_vao->unbind();
        }

        void loadLineColourShader()
        {
            ngl::ShaderLib *shader = ngl::ShaderLib::instance();
            shader->use(ngl::nglColourShader);

            m_model = scene_view;
            m_model.translate(0.f,0.f,0.f);
            auto MVP = projection * m_view * m_model;

            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",ngl::Vec4(1.f,1.f,1.f,1.f));
        }

        void draw()
        {
            loadLineColourShader();
            m_vao->bind();
            m_vao->draw();
            m_vao->unbind();
        }

        ~TransformHandle() noexcept = default;
};






template<bool visualize_bv_and_ray = false>
class Select final : public Base_Selection<visualize_bv_and_ray>
{
    private:
        bool m_multi_selection;

    private:
        bool alreadySelected(std::size_t id_) const;
        std::size_t getSelectedId(float &shortest_distance_) const;

    public:
        template<typename C>
        explicit Select( const mc::View &view_,
                         const mc::Projection &projection_,
                         const C &camera_ );

        template<typename PRIM>
        void make_selectable(std::size_t id_, PRIM &&prim_, const mc::Transform &transform_);

        void resize(int w_, int h_);
        void enableMultiSelection();
        mc::Position clickedOnObject(const VariantPrim &selectable_) const;
        void pick(int mouse_x, int mouse_y);

        ~Select() noexcept = default;
};
