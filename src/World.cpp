
#include "World.h"
#include <random>
#include <algorithm>

void World::generate(int cells_x, int cells_y, int seed) {
  m_cells_x = cells_x;
  m_cells_y = cells_y;
  m_cells.resize(m_cells_y * m_cells_x);
  std::fill(begin(m_cells), end(m_cells), Cell{
    CellType::wall,
    CellDecoration::none
  });

  auto rand = std::minstd_rand(seed);

  const auto random = [&](int min, int max) {
    return (max > min ? min + rand() % (max - min) : min);
  };

  for (auto i = 0; i < 11; ++i) {
    const auto sx = random(1, 6);
    const auto sy = random(1, 4);
    const auto px = random(0, m_cells_x - sx);
    const auto py = random(0, m_cells_y - sy);
    for (auto y = py; y < py + sy; ++y)
      for (auto x = px; x < px + sx; ++x)
        m_cells[y * m_cells_x + x].type = CellType::floor;
  }
}

Texture World::generate_texture(Graphics& graphics) const {

  const auto get_tile = [&](auto tileset, auto check_proximity, int x, int y) {
    using Tileset = decltype(tileset);

    auto code = 0;
    if (check_proximity(x - 1, y))     code |= 0b1000'0000;
    if (check_proximity(x,     y - 1)) code |= 0b0100'0000;
    if (check_proximity(x + 1, y))     code |= 0b0010'0000;
    if (check_proximity(x,     y + 1)) code |= 0b0001'0000;
    if (check_proximity(x - 1, y - 1)) code |= 0b0000'1000;
    if (check_proximity(x + 1, y - 1)) code |= 0b0000'0100;
    if (check_proximity(x - 1, y + 1)) code |= 0b0000'0010;
    if (check_proximity(x + 1, y + 1)) code |= 0b0000'0001;

    switch (code) {
      case 0b1111'0000: return Tileset::tile_1111_0000;
      case 0b1111'0001: return Tileset::tile_1111_0001;
      case 0b1111'0010: return Tileset::tile_1111_0010;
      case 0b1111'0011: return Tileset::tile_1111_0011;
      case 0b1111'0100: return Tileset::tile_1111_0100;
      case 0b1111'0101: return Tileset::tile_1111_0101;
      case 0b1111'0110: return Tileset::tile_1111_0110;
      case 0b1111'0111: return Tileset::tile_1111_0111;
      case 0b1111'1000: return Tileset::tile_1111_1000;
      case 0b1111'1001: return Tileset::tile_1111_1001;
      case 0b1111'1010: return Tileset::tile_1111_1010;
      case 0b1111'1011: return Tileset::tile_1111_1011;
      case 0b1111'1100: return Tileset::tile_1111_1100;
      case 0b1111'1101: return Tileset::tile_1111_1101;
      case 0b1111'1110: return Tileset::tile_1111_1110;
    }

    if ((code | 0b0111) == 0b1100'0111) return Tileset::tile_1100_0xxx;
    if ((code | 0b1011) == 0b0110'1011) return Tileset::tile_0110_x0xx;
    if ((code | 0b1101) == 0b1001'1101) return Tileset::tile_1001_xx0x;
    if ((code | 0b1110) == 0b0011'1110) return Tileset::tile_0011_xxx0;

    switch (code | 0b1010) {
      case 0b0111'1010: return Tileset::tile_0111_x0x0;
      case 0b0111'1011: return Tileset::tile_0111_x0x1;
      case 0b0111'1110: return Tileset::tile_0111_x1x0;
    }
    switch (code | 0b1100) {
      case 0b1011'1100: return Tileset::tile_1011_xx00;
      case 0b1011'1101: return Tileset::tile_1011_xx01;
      case 0b1011'1110: return Tileset::tile_1011_xx10;
    }
    switch (code | 0b0101) {
      case 0b1101'0101: return Tileset::tile_1101_0x0x;
      case 0b1101'0111: return Tileset::tile_1101_0x1x;
      case 0b1101'1101: return Tileset::tile_1101_1x0x;
    }
    switch (code | 0b0011) {
      case 0b1110'0011: return Tileset::tile_1110_00xx;
      case 0b1110'0111: return Tileset::tile_1110_01xx;
      case 0b1110'1011: return Tileset::tile_1110_10xx;
    }

    switch (code >> 4) {
      case 0b0000: return Tileset::tile_0000;
      case 0b0001: return Tileset::tile_0001;
      case 0b0010: return Tileset::tile_0010;
      case 0b0011: return Tileset::tile_0011;
      case 0b0100: return Tileset::tile_0100;
      case 0b0101: return Tileset::tile_0101;
      case 0b0110: return Tileset::tile_0110;
      case 0b0111: return Tileset::tile_0111;
      case 0b1000: return Tileset::tile_1000;
      case 0b1001: return Tileset::tile_1001;
      case 0b1010: return Tileset::tile_1010;
      case 0b1011: return Tileset::tile_1011;
      case 0b1100: return Tileset::tile_1100;
      case 0b1101: return Tileset::tile_1101;
      case 0b1110: return Tileset::tile_1110;
      case 0b1111: return Tileset::tile_1111;
    }
    return Tileset::tile_1111;
  };

  const auto wall_at = [&](int x, int y) {
    return (cell_at(x, y).type == CellType::wall);
  };
  const auto no_wall_at = [&](int x, int y) {
    return !wall_at(x, y);
  };

  auto target = Target();
  target.bind(target_width(), target_height());

  const auto tile_size = graphics.tile_size;
  const auto cells_x = (target_width() + tile_size - 1) / tile_size;
  const auto cells_y = (target_height() + tile_size - 1) / tile_size;
  for (auto y = 0; y < cells_y; ++y)
    for (auto x = 0; x < cells_x; ++x)
      graphics.draw_tile(x * tile_size, y * tile_size,
        wall_at(x, y) ?
          get_tile(tiles::Wall(), wall_at, x, y) :
          get_tile(tiles::Floor(), no_wall_at, x, y));
  graphics.flush_drawing();

  return std::move(target);
}
