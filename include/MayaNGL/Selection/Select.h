#pragma once

#include "Base_Selection.h"



struct Gizmo
{
    private:
        typedef std::array<mc::Position,2> Vertices;
        typedef std::unique_ptr<ngl::AbstractVAO> VAOPtr;

    private:
        const mc::View &view;
        const mc::Projection &projection;
        const mc::Direction &cam_inverse;

    private:
        mc::Transform m_model;
        VAOPtr m_vao;

    public:
        Gizmo( const mc::View &view_,
               const mc::Projection &projection_,
               const mc::Direction &cam_inverse_)
               :
               view(view_),
               projection(projection_),
               cam_inverse(cam_inverse_),
               m_model(),
               m_vao()
        {;}

        void initialize()
        {
            m_vao = ngl::VAOFactory::createVAO("simpleVAO",GL_LINES);

            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();
            prim->createCone("arrow_head",0.2f,0.6f,10,1);
            prim->createTrianglePlane("central",0.25f,0.25f,1,1,cam_inverse);
        }

        void loadLineColourShader(mc::Colour &&colour_)
        {
            ngl::ShaderLib *shader = ngl::ShaderLib::instance();
            shader->use(ngl::nglColourShader);

            auto MVP = projection * view * m_model;

            shader->setUniform("MVP",MVP);
            shader->setUniform("Colour",std::move(colour_));
        }

        void drawLine(mc::Direction &&dir_)
        {
            Vertices vtxs = {{ mc::Direction::zero(), std::move(dir_) }};
            m_vao->bind();
            m_vao->setData(ngl::AbstractVAO::VertexData(vtxs.size()*sizeof(mc::Position),vtxs[0].m_x));
            m_vao->setNumIndices(vtxs.size());
            m_vao->setVertexAttributePointer(0,3,GL_FLOAT,0,0);
            m_vao->draw();
            m_vao->unbind();
        }

        void draw()
        {
            glClear(GL_DEPTH_BUFFER_BIT);

            ngl::VAOPrimitives *prim = ngl::VAOPrimitives::instance();

            m_model.identity();
                loadLineColourShader(mc::Colour(1.f,0.f,0.f,1.f));
                drawLine(mc::Direction::right()*3.f);
                loadLineColourShader(mc::Colour(0.f,1.f,0.f,1.f));
                drawLine(mc::Direction::up()*3.f);
                loadLineColourShader(mc::Colour(0.f,0.f,1.f,1.f));
                drawLine(mc::Direction::in()*3.f);

            m_model.identity();
                m_model.translate(3.f,0.f,0.f);
                m_model.rotateY(90.f);
                loadLineColourShader(mc::Colour(1.f,0.f,0.f,1.f));
                prim->draw("arrow_head");

            m_model.identity();
                m_model.translate(0.f,3.f,0.f);
                m_model.rotateX(-90.f);
                loadLineColourShader(mc::Colour(0.f,1.f,0.f,1.f));
                prim->draw("arrow_head");

            m_model.identity();
                m_model.translate(0.f,0.f,3.f);
                loadLineColourShader(mc::Colour(0.f,0.f,1.f,1.f));
                prim->draw("arrow_head");

            m_model.identity();
//                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                loadLineColourShader(mc::Colour(1.f,1.f,0.f,1.f));
                prim->draw("central");
//                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }

        ~Gizmo() noexcept = default;
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
