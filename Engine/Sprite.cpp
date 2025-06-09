#include <allegro5/allegro.h>
#include <memory>
#include "Engine/Point.hpp"
#include "Sprite.hpp"
#include "UI/Component/Image.hpp"

// 這裡有魔改
namespace Engine {
    Sprite::Sprite(std::string img, float x, float y, float w, float h, float anchorX, float anchorY,
                   float rotation, float vx, float vy, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
        : Image(img, x, y, w, h, anchorX, anchorY),
          Rotation(rotation),
          Velocity(Point(vx, vy)),
          Tint(al_map_rgba(r, g, b, a)) {
        SourceX = 0;
        SourceY = 0;
        SourceW = -1; // -1 表示使用整張圖
        SourceH = -1;
        
    }

    void Sprite::SetSpriteSource(float sx, float sy, float sw, float sh) {
        SourceX = sx;
        SourceY = sy;
        SourceW = sw;
        SourceH = sh;
    }

    void Sprite::SetSize(float w, float h) {
        Size.x = w;
        Size.y = h;
    }

    void Sprite::Draw() const {
        float srcW = SourceW > 0 ? SourceW : GetBitmapWidth();
        float srcH = SourceH > 0 ? SourceH : GetBitmapHeight();

        al_draw_tinted_scaled_rotated_bitmap_region(
            bmp.get(),
            SourceX, SourceY, srcW, srcH,      // Source region
            Tint,
            Anchor.x * srcW, Anchor.y * srcH,  // Anchor in source
            Position.x, Position.y,            // Draw position
            Size.x / srcW, Size.y / srcH,      // Scaling factor
            Rotation,                          // Rotation
            0                                  // Flags
        );
    }

    void Sprite::Update(float deltaTime) {
        Position.x += Velocity.x * deltaTime;
        Position.y += Velocity.y * deltaTime;
    }
}


/*
使用方式
  顯示 sprite sheet 中第 1 格（假設每格 32x32）
sprite->SetSpriteSource(32 * 1, 0, 32, 32);
  實際顯示大小 (一定要用x=0, y=0 初始化)
SetSize(96, 96)
*/