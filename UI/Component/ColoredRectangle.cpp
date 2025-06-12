#include "ColoredRectangle.hpp"

using namespace Engine;

ColoredRectangle::ColoredRectangle(float x, float y, float width, float height, ALLEGRO_COLOR fillColor, float borderThickness, ALLEGRO_COLOR borderColor)
    : IObject(x, y, width, height),
      width(width), height(height),
      color(fillColor),
      borderThickness(borderThickness),
      borderColor(borderColor) {
}

void ColoredRectangle::Draw() const {
    // Fill
    al_draw_filled_rectangle(Position.x, Position.y,
                             Position.x + width, Position.y + height,
                             color);

    // Border
    if (borderThickness > 0.0f) {
        al_draw_rectangle(Position.x, Position.y,
                          Position.x + width, Position.y + height,
                          borderColor, borderThickness);
    }
}
