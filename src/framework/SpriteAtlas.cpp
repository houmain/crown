
#include "SpriteAtlas.h"
#include "SpriteBatch.h"

void SpriteAtlas::append_sheet(const std::string& filename,
    int sprite_width, int sprite_height) {
  m_sheets.push_back({ Texture(filename), sprite_width, sprite_height });
}

void SpriteAtlas::append_animation(bool looping, std::vector<Frame> frames) {
  auto duration = 0.0f;
  for (const auto& frame : frames)
    duration += frame.duration;
  m_animations.push_back({ static_cast<int>(m_sheets.size()) - 1,
    looping, duration, std::move(frames) });
}

void SpriteAtlas::draw(SpriteBatch& batch, int animation_id, float x, float y,
    float position, float alpha) {
/*
  const auto& animation = m_animations[animation_id];
  const auto& sheet = m_sheets[animation.sheet_id];

  // wrap or clamp to end
  if (position >= animation.duration) {
    if (animation.looping) {
      position = std::fmod(position, animation.duration);
    }
    else {
      const auto& frame = animation.frames.back();
      batch.draw(x, y,
        sheet.texture,
        frame.x * sheet.sprite_width,
        frame.y * sheet.sprite_height,
        sheet.sprite_width,
        sheet.sprite_height,
        alpha);
      return;
    }
  }

  // advance to frame
  auto prev_frame = cbegin(animation.frames);
  while (position > prev_frame->duration) {
    position -= prev_frame->duration;
    ++prev_frame;
  }

  auto frame = (std::next(prev_frame) == cend(animation.frames) ?
    cbegin(animation.frames) : std::next(prev_frame));
  position /= prev_frame->duration;

  if (position > 0.25f)
    position = (position - 0.25f) * 4;
  else
    position = 0;

  batch.draw(x, y,
    sheet.texture,
    prev_frame->x * sheet.sprite_width,
    prev_frame->y * sheet.sprite_height,
    sheet.sprite_width,
    sheet.sprite_height,
    frame->x * sheet.sprite_width,
    frame->y * sheet.sprite_height,
    position,
    alpha);*/
}
