#pragma once

#include "Texture.h"
#include <vector>

class SpriteBatch;

class SpriteAtlas {
public:
  struct Frame {
    uint16_t x, y;
    float duration;
  };

  void append_sheet(const std::string& filename, int sprite_width, int sprite_height);
  void append_animation(bool looping, std::vector<Frame> frames);

  void draw(SpriteBatch& batch, int animation_id, float x, float y,
    float position, float alpha = 1);

private:
  struct Sheet {
    Texture texture;
    int sprite_width{ };
    int sprite_height{ };
  };

  struct Animation {
    int sheet_id;
    bool looping;
    float duration;
    std::vector<Frame> frames;
  };

  std::vector<Sheet> m_sheets;
  std::vector<Animation> m_animations;
};
