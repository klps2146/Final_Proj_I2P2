#include "Character.hpp"
#include <allegro5/allegro.h>
#include "Engine/GameEngine.hpp"
#include "Scene/PlayScene.hpp"
#include <iostream>

PlayScene *Engine::Character::getPlayScene() {
    return dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetActiveScene());
}

namespace Engine {
    const int BlockSize = 64;

    Character::Character(std::string img, float x, float y, float w, float h,
                       float anchorX, float anchorY, float speed, float collisionRadius)
        : Sprite(img, x, y, w, h, anchorX, anchorY, 0, 0, 0, 255, 255, 255, 255),
          speed(speed), direction(0, 0), isMoving(false) {
        this->CollisionRadius = collisionRadius;
        std::cout << "Created Character at (" << x << ", " << y << ") with speed " << speed << std::endl;
        SetSpriteSource(0, 0, 96, 96);
        std::cout << ">>>>>>>>Bitmap width: " << GetBitmapWidth() << ", height: " << GetBitmapHeight() << std::endl;
    }

    void Character::OnKeyDown(int keyCode) {
        // Handle WASD and arrow keys for movement
        switch (keyCode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
                direction.y = -1; // Move up
                isMoving = true;
                break;
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                direction.y = 1;  // Move down
                isMoving = true;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
                direction.x = -1; // Move left
                isMoving = true;
                break;
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                direction.x = 1;  // Move right
                isMoving = true;
                break;
        }
        // Normalize direction to prevent faster diagonal movement
        if (isMoving && direction.Magnitude() > 0) {
            direction = direction.Normalize();
        }
        std::cout << "Key down: " << keyCode << ", Direction: (" << direction.x << ", " << direction.y << ")";
    }

    void Character::OnKeyUp(int keyCode) {
        // Reset direction components when keys are released
        switch (keyCode) {
            case ALLEGRO_KEY_W:
            case ALLEGRO_KEY_UP:
            case ALLEGRO_KEY_S:
            case ALLEGRO_KEY_DOWN:
                direction.y = 0;
                break;
            case ALLEGRO_KEY_A:
            case ALLEGRO_KEY_LEFT:
            case ALLEGRO_KEY_D:
            case ALLEGRO_KEY_RIGHT:
                direction.x = 0;
                break;
        }
        // Check if character is still moving
        isMoving = (direction.x != 0 || direction.y != 0);
        if (!isMoving) {
            Velocity = Point(0, 0);
        }
        std::cout << "Key up: " << keyCode << ", Direction: (" << direction.x << ", " << direction.y << ")";
    }

    void Character::Update(float deltaTime) {
        // Update velocity based on direction and speed
        if (isMoving) {
            Velocity = direction * speed;
            
            SetSpriteSource(96, 0, 96, 96);  
            // std::cout << "MOV " << SourceX
            //                     << " " <<SourceY
            //                     << " " <<SourceW
            //                     << " " <<SourceH << std::endl;
        }
        else{
            SetSpriteSource(0, 0, 96, 96);
            // std::cout << "static " << SourceX
            //                     << " " <<SourceY
            //                     << " " <<SourceW
            //                     << " " <<SourceH << std::endl;
        }

        /* New modification starts here */
        // Calculate new position based on velocity and delta time
        Point newPosition = Position + Velocity * deltaTime;

        // Get map data from PlayScene
        auto& mapState = getPlayScene()->mapState;
        int mapHeight = mapState.size();
        if (mapHeight == 0) return; // Prevent errors if map is empty
        int mapWidth = mapState[0].size();

        // Convert new position to tile coordinates (center of character)
        int tileX = static_cast<int>(newPosition.x / BlockSize);
        int tileY = static_cast<int>(newPosition.y / BlockSize);

        // Check if the tile at the character's center is TILE_WATER
        bool collisionDetected = false;
        if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight) {
            if (mapState[tileY][tileX] == getPlayScene()->TILE_WATER||mapState[tileY][tileX] == getPlayScene()->TILE_ROCK) {
                // Perform circle-rectangle collision check for TILE_WATER
                float tileLeft = tileX * BlockSize;
                float tileRight = tileLeft + BlockSize;
                float tileTop = tileY * BlockSize;
                float tileBottom = tileTop + BlockSize;

                // Find closest point on tile rectangle to character center
                float closestX = std::max(tileLeft, std::min(newPosition.x, tileRight));
                float closestY = std::max(tileTop, std::min(newPosition.y, tileBottom));

                // Calculate distance from character center to closest point
                float dx = newPosition.x - closestX;
                float dy = newPosition.y - closestY;
                float distance = std::sqrt(dx * dx + dy * dy);

                // Collision if distance is less than CollisionRadius
                if (distance < CollisionRadius) {
                    collisionDetected = true;
                }
            }
        }

        // Update position only if no collision is detected
        if (!collisionDetected) {
            Position = newPosition;
        } else {
            Velocity = Point(0, 0); // Stop movement on collision with TILE_WATER
        }
        /* New modification ends here */

        // Update position using Sprite's Update (Position += Velocity * deltaTime)
        Sprite::Update(deltaTime);

        // Optional: Clamp position to screen boundaries
        Point screenSize = GameEngine::GetInstance().GetScreenSize();
        Position.x = std::max(CollisionRadius, std::min(screenSize.x - CollisionRadius, Position.x));
        Position.y = std::max(CollisionRadius, std::min(screenSize.y - CollisionRadius, Position.y));
    }
    
} // namespace Engine