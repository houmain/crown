
#include "View.h"
#include "state/State.h"
#include <cmath>

#include <ctime>

View::View(const State* state)
  : m_state(*state),
    m_sprites("sprites.png") {

  m_timeline.apply(&m_radius)
    .then<choreograph::RampTo>(13, 1.5);

  auto target = Target();
  target.bind(target_width(), target_height());

  struct Cell {
    bool floor;
  };
  const auto tile_size = 32;
  const auto cells_x = (target_width() + tile_size - 1) / tile_size;
  const auto cells_y = (target_height() + tile_size - 1) / tile_size;

  auto map = std::vector<Cell>();
  map.resize(cells_y * cells_x);

  const auto wall_at = [&](int x, int y) {
    if (x < 0 || x >= cells_x || y < 0 || y >= cells_y)
      return true;
    return !map[y * cells_x + x].floor;
  };

  std::srand(std::time(nullptr));

  const auto random = [](int min, int max) {
    return (max > min ? min + (std::rand() % (max - min)) : min);
  };

  for (auto i = 0; i < 11; ++i) {
    const auto sx = random(1, 6);
    const auto sy = random(1, 4);
    const auto px = random(0, cells_x - sx);
    const auto py = random(0, cells_y - sy);
    for (auto y = py; y < py + sy; ++y)
      for (auto x = px; x < px + sx; ++x)
        map[y * cells_x + x].floor = true;
  }

  const auto get_floor = [&](int x, int y) {
    auto code = 0;
    if (!wall_at(x - 1, y))     code |= 0b1000'0000;
    if (!wall_at(x,     y - 1)) code |= 0b0100'0000;
    if (!wall_at(x + 1, y))     code |= 0b0010'0000;
    if (!wall_at(x,     y + 1)) code |= 0b0001'0000;
    if (!wall_at(x - 1, y - 1)) code |= 0b0000'1000;
    if (!wall_at(x + 1, y - 1)) code |= 0b0000'0100;
    if (!wall_at(x - 1, y + 1)) code |= 0b0000'0010;
    if (!wall_at(x + 1, y + 1)) code |= 0b0000'0001;

    switch (code) {
      case 0b1111'0000: return sprites::floor_1111_0000;
      case 0b1111'0001: return sprites::floor_1111_0001;
      case 0b1111'0010: return sprites::floor_1111_0010;
      case 0b1111'0011: return sprites::floor_1111_0011;
      case 0b1111'0100: return sprites::floor_1111_0100;
      case 0b1111'0101: return sprites::floor_1111_0101;
      case 0b1111'0110: return sprites::floor_1111_0110;
      case 0b1111'0111: return sprites::floor_1111_0111;
      case 0b1111'1000: return sprites::floor_1111_1000;
      case 0b1111'1001: return sprites::floor_1111_1001;
      case 0b1111'1010: return sprites::floor_1111_1010;
      case 0b1111'1011: return sprites::floor_1111_1011;
      case 0b1111'1100: return sprites::floor_1111_1100;
      case 0b1111'1101: return sprites::floor_1111_1101;
      case 0b1111'1110: return sprites::floor_1111_1110;
    }

    if ((code | 0b0111) == 0b1100'0111) return sprites::floor_1100_0xxx;
    if ((code | 0b1011) == 0b0110'1011) return sprites::floor_0110_x0xx;
    if ((code | 0b1101) == 0b1001'1101) return sprites::floor_1001_xx0x;
    if ((code | 0b1110) == 0b0011'1110) return sprites::floor_0011_xxx0;

    switch (code | 0b1010) {
      case 0b0111'1010: return sprites::floor_0111_x0x0;
      case 0b0111'1011: return sprites::floor_0111_x0x1;
      case 0b0111'1110: return sprites::floor_0111_x1x0;
    }
    switch (code | 0b1100) {
      case 0b1011'1100: return sprites::floor_1011_xx00;
      case 0b1011'1101: return sprites::floor_1011_xx01;
      case 0b1011'1110: return sprites::floor_1011_xx10;
    }
    switch (code | 0b0101) {
      case 0b1101'0101: return sprites::floor_1101_0x0x;
      case 0b1101'0111: return sprites::floor_1101_0x1x;
      case 0b1101'1101: return sprites::floor_1101_1x0x;
    }
    switch (code | 0b0011) {
      case 0b1110'0011: return sprites::floor_1110_00xx;
      case 0b1110'0111: return sprites::floor_1110_01xx;
      case 0b1110'1011: return sprites::floor_1110_10xx;
    }

    switch (code >> 4) {
      case 0b0000: return sprites::floor_0000;
      case 0b0001: return sprites::floor_0001;
      case 0b0010: return sprites::floor_0010;
      case 0b0011: return sprites::floor_0011;
      case 0b0100: return sprites::floor_0100;
      case 0b0101: return sprites::floor_0101;
      case 0b0110: return sprites::floor_0110;
      case 0b0111: return sprites::floor_0111;
      case 0b1000: return sprites::floor_1000;
      case 0b1001: return sprites::floor_1001;
      case 0b1010: return sprites::floor_1010;
      case 0b1011: return sprites::floor_1011;
      case 0b1100: return sprites::floor_1100;
      case 0b1101: return sprites::floor_1101;
      case 0b1110: return sprites::floor_1110;
      case 0b1111: return sprites::floor_1111;
    }
    return sprites::floor_1111;
  };

  const auto get_wall = [&](int x, int y) {
    auto code = 0;
    if (wall_at(x - 1, y))     code |= 0b1000'0000;
    if (wall_at(x,     y - 1)) code |= 0b0100'0000;
    if (wall_at(x + 1, y))     code |= 0b0010'0000;
    if (wall_at(x,     y + 1)) code |= 0b0001'0000;
    if (wall_at(x - 1, y - 1)) code |= 0b0000'1000;
    if (wall_at(x + 1, y - 1)) code |= 0b0000'0100;
    if (wall_at(x - 1, y + 1)) code |= 0b0000'0010;
    if (wall_at(x + 1, y + 1)) code |= 0b0000'0001;

    switch (code) {
      case 0b1111'0000: return sprites::wall_1111_0000;
      case 0b1111'0001: return sprites::wall_1111_0001;
      case 0b1111'0010: return sprites::wall_1111_0010;
      case 0b1111'0011: return sprites::wall_1111_0011;
      case 0b1111'0100: return sprites::wall_1111_0100;
      case 0b1111'0101: return sprites::wall_1111_0101;
      case 0b1111'0110: return sprites::wall_1111_0110;
      case 0b1111'0111: return sprites::wall_1111_0111;
      case 0b1111'1000: return sprites::wall_1111_1000;
      case 0b1111'1001: return sprites::wall_1111_1001;
      case 0b1111'1010: return sprites::wall_1111_1010;
      case 0b1111'1011: return sprites::wall_1111_1011;
      case 0b1111'1100: return sprites::wall_1111_1100;
      case 0b1111'1101: return sprites::wall_1111_1101;
      case 0b1111'1110: return sprites::wall_1111_1110;
    }

    if ((code | 0b0111) == 0b1100'0111) return sprites::wall_1100_0xxx;
    if ((code | 0b1011) == 0b0110'1011) return sprites::wall_0110_x0xx;
    if ((code | 0b1101) == 0b1001'1101) return sprites::wall_1001_xx0x;
    if ((code | 0b1110) == 0b0011'1110) return sprites::wall_0011_xxx0;

    switch (code | 0b1010) {
      case 0b0111'1010: return sprites::wall_0111_x0x0;
      case 0b0111'1011: return sprites::wall_0111_x0x1;
      case 0b0111'1110: return sprites::wall_0111_x1x0;
    }
    switch (code | 0b1100) {
      case 0b1011'1100: return sprites::wall_1011_xx00;
      case 0b1011'1101: return sprites::wall_1011_xx01;
      case 0b1011'1110: return sprites::wall_1011_xx10;
    }
    switch (code | 0b0101) {
      case 0b1101'0101: return sprites::wall_1101_0x0x;
      case 0b1101'0111: return sprites::wall_1101_0x1x;
      case 0b1101'1101: return sprites::wall_1101_1x0x;
    }
    switch (code | 0b0011) {
      case 0b1110'0011: return sprites::wall_1110_00xx;
      case 0b1110'0111: return sprites::wall_1110_01xx;
      case 0b1110'1011: return sprites::wall_1110_10xx;
    }

    switch (code >> 4) {
      case 0b0000: return sprites::wall_0000;
      case 0b0001: return sprites::wall_0001;
      case 0b0010: return sprites::wall_0010;
      case 0b0011: return sprites::wall_0011;
      case 0b0100: return sprites::wall_0100;
      case 0b0101: return sprites::wall_0101;
      case 0b0110: return sprites::wall_0110;
      case 0b0111: return sprites::wall_0111;
      case 0b1000: return sprites::wall_1000;
      case 0b1001: return sprites::wall_1001;
      case 0b1010: return sprites::wall_1010;
      case 0b1011: return sprites::wall_1011;
      case 0b1100: return sprites::wall_1100;
      case 0b1101: return sprites::wall_1101;
      case 0b1110: return sprites::wall_1110;
      case 0b1111: return sprites::wall_1111;
    }
    return sprites::wall_1111;
  };

  for (auto y = 0; y < cells_y; ++y)
    for (auto x = 0; x < cells_x; ++x) {
      const auto get_sprite = [&]() {
        if (wall_at(x, y))
          return get_wall(x, y);
        else
          return get_floor(x, y);


      };
      draw_sprite(x * tile_size, y * tile_size, get_sprite());
    }
  m_batch.flush();

  m_world = std::move(target);
}

void View::draw(float frame_pos) {
  m_timeline.jumpTo(m_state.timeline().time() + frame_pos * m_state.update_interval());

  m_batch.draw(0, 0, m_world, 0, 0, m_world.width(), m_world.height(), 1, 1, false, true);
  m_batch.flush();

  auto x = TWEAKABLE(10);
  auto y = TWEAKABLE(20);
  auto w = 100;
  auto h = 100;
  auto radius = m_radius.value();
  auto text = TWEAKABLE("hallo das ist ein Test99");
  auto color = TWEAKABLE(nvgRGBA, 10, 128, 255, 255);

  nvgBeginFrame(vg, target_width(), target_height(), target_scale());

  nvgBeginPath(vg);
  nvgRoundedRect(vg, x + 0.5f,y + 0.5f, w - 1, h - 1, radius - 0.25f);
  nvgStrokeColor(vg, color);
  nvgStroke(vg);

  nvgFontBlur(vg, TWEAKABLE(0));
  nvgFillColor(vg, nvgRGBA(255, 32, 64, 255));

  nvgFontSize(vg, TWEAKABLE(20));
  nvgText(vg, x + 20 * std::fabs(std::sin(m_timeline.time())), y - 5, text, nullptr);

  nvgEndFrame(vg);

  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  draw_animation(300, 200, sprites::king_pig_idle, 6 * static_cast<float>(m_timeline.time()));
  draw_animation(100+std::fmod(m_timeline.time(), 6) * 70, target_height(), sprites::player_run, 13 * static_cast<float>(m_timeline.time()));

  m_batch.flush();
}

void View::draw_sprite(float x, float y, const sprites::Sprite& sprite) {
  m_batch.draw(x - sprite.pivot_x, y - sprite.pivot_y, m_sprites,
    sprite.x, sprite.y, sprite.w, sprite.h);
}

void View::do_draw_animation(float x, float y,
    const sprites::Sprite* const* frames, float position) {
  const auto& frame = *frames[static_cast<int>(position)];
  draw_sprite(x, y, frame);
}
