#ifndef COLOREDRECTANGLE_HPP
#define COLOREDRECTANGLE_HPP

#include "Engine/IObject.hpp"
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

class ColoredRectangle : public Engine::IObject {
public:
    float width, height;
    ALLEGRO_COLOR color;
    float borderThickness = 0;
    ALLEGRO_COLOR borderColor = al_map_rgba(0, 0, 0, 0);
    ColoredRectangle(){};
    ColoredRectangle(float x, float y, float width, float height,
                     ALLEGRO_COLOR fillColor,
                     float borderThickness = 0.0f,
                     ALLEGRO_COLOR borderColor = al_map_rgba(0, 0, 0, 0));
    void Draw() const override;
};

#endif // COLOREDRECTANGLE_HPP
