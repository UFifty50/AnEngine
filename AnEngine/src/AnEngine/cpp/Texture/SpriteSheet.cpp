#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Texture/SpriteSheet.hpp"

#include "Renderer/Renderer2D.hpp"
#include "Texture/Sprite.hpp"


namespace AnEngine {
    SpriteSheet::SpriteSheet(const std::string& path, int spriteWidth, int spriteHeight)
        : sheet(Texture2D::create(path)),
          spriteWidth(spriteWidth),
          spriteHeight(spriteHeight),
          texWidth(float(spriteWidth) / sheet->getWidth()),
          texHeight(float(spriteHeight) / sheet->getHeight()),
          spritesPerRow(sheet->getWidth() / spriteWidth) {}


    SpriteSheet::SpriteSheet(const Ref<Texture2D>& texture, int spriteWidth,
                             int spriteHeight)
        : sheet(texture),
          spriteWidth(spriteWidth),
          spriteHeight(spriteHeight),
          texWidth(float(spriteWidth) / sheet->getWidth()),
          texHeight(float(spriteHeight) / sheet->getHeight()),
          spritesPerRow(sheet->getWidth() / spriteWidth) {}

    void SpriteSheet::renderSprite(glm::vec2 index, glm::vec3 position,
                                   float rotation) const {
        const int spritesPerRow = sheet->getWidth() / spriteWidth;
        const int spritesPerColumn = sheet->getHeight() / spriteHeight;

        AE_CORE_ASSERT(index.x < spritesPerRow,
                       "Request for sprite index {0} out of range ({1} sprites per row)",
                       index.x, spritesPerRow);
        AE_CORE_ASSERT(
            index.y < spritesPerColumn,
            "Request for sprite index {0} out of range ({1} sprites per column)", index.y,
            spritesPerColumn);

        if (index.x >= spritesPerRow || index.y >= spritesPerColumn) return;

        const float texX = index.x * spriteWidth / (float)sheet->getWidth();
        const float texY = index.y * spriteHeight / (float)sheet->getHeight();

        glm::mat4x2 texCoords = {
            glm::vec2{texX, texY},
            glm::vec2{texX + texWidth, texY},
            glm::vec2{texX + texWidth, texY + texHeight},
            glm::vec2{texX, texY + texHeight},
        };

        Renderer2D::drawQuad(position, {1.0f, 1.0f}, rotation, sheet,
                             ShaderUniformVector{ShaderUniform("texCoords", texCoords)});
    }

    Sprite SpriteSheet::getSprite(glm::vec2 index) const {
        return Sprite(MakeRef<SpriteSheet>(*this), index);
    }

    /*void SpriteSheet::renderSheetWithIndices(glm::vec3 position, float rotation) const {
        auto attrs = ShaderUniformVector{
            ShaderUniform{
                "tint",
                glm::vec4(1.0f),
            },
            ShaderUniform{"texCoords", glm::mat4x2(1.0f)},
        };
        int index = 0;

        for (float x = 0; x <= sheet->getWidth() / 2.0f; x += spriteWidth / 2.0f) {
            for (float y = 0; y <= sheet->getHeight() / 2.0f; y += spriteHeight / 2.0f) {
                const float texX = float(index % spritesPerRow) * texWidth;
                const float texY = float(index / spritesPerRow + 1) * texHeight;

                glm::mat4x2 texCoords = {
                    glm::vec2{texX, texY},
                    glm::vec2{texX + texWidth, texY},
                    glm::vec2{texX + texWidth, texY + texHeight},
                    glm::vec2{texX, texY + texHeight},
                };

                attrs["texCoords"] = texCoords;
                attrs["tint"] = glm::vec4(x / sheet->getWidth() / 2.0f,
                                          y / sheet->getHeight() / 2.0f, 0.0f, 1.0f);
                Renderer2D::drawQuad({position.x + x / 2.0f, position.y + y / 2.0f},
                                     {2.0f, 2.0f}, rotation, sheet, attrs);
                index++;
            }
        }
    }*/
}  // namespace AnEngine
