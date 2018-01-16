#include "Object2D.h"
#include "Graphics2D.h"
void Line::Draw( Graphics2D& g2d ) 
{
     g2d.DrawLine(*this); 
};

void Rect::Draw( Graphics2D& g2d) 
{
     g2d.DrawRect(*this); 
};
void Circle::Draw( Graphics2D& g2d)
{
     g2d.DrawCircle(*this); 
};
void Image::Draw( Graphics2D& g2d) 
{
     g2d.DrawImage(*this); 
};