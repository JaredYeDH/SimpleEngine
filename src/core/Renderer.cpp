#include "Renderer.h"

Renderer2D::Renderer2D()
:m_Graphics(),
m_ObjectList(0)
{
    
}

Renderer2D::~Renderer2D()
{
}


void Renderer2D::Render()
{
    m_Graphics.Begin();
    for(Object2D* obj : m_ObjectList)
    {
        m_Graphics.Draw(*obj);
    }
    m_Graphics.End();
}


void Renderer2D::AddObject(Object2D* obj)
{
    m_ObjectList.push_back(obj);
}


void Renderer2D::RemoveObject(int index)
{

    //m_ObjectList.remove(index);

}


RenderThread::RenderThread()
{
}

RenderThread::~RenderThread()
{

}


