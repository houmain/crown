
#include "World.h"
#include "Graphics.h"
#include "_generated/tiles.h"
#include "_generated/assets.h"
#include <random>
#include <algorithm>

World::World()
  : m_tiles(assets::images::tiles) {
}

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

  auto room_cells = 0;
  const auto place_room = [&](int px, int py, int sx, int sy) {
    for (auto y = py; y < py + sy; ++y)
      for (auto x = px; x < px + sx; ++x) {
        auto& cell = m_cells[y * m_cells_x + x];
        if (cell.type == CellType::wall) {
          cell.type = CellType::room;
          ++room_cells;
        }
      }
  };

  while (room_cells < (cells_x * cells_y) / 2) {
    const auto sx = random(6, 9);
    const auto sy = random(1, 3);
    const auto px = random(1, m_cells_x - sx);
    const auto py = random(1, m_cells_y - sy);
    place_room(px, py, sx, sy);

    // generate some platforms
    for (auto j = 0; j < 1; ++j) {
      const auto sx2 = random(1, sx);
      const auto px2 = px + random(0, sx - sx2);
      const auto py2 = py + random(0, sy);
      for (auto x = px2; x < px2 + sx2; ++x)
        m_cells[py2 * m_cells_x + x].type = (j % 2 ?
          CellType::platform_2 : CellType::platform_1);
    }
  }
}

void World::generate_texture(Graphics& graphics, int width, int height) {
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
  target.bind(width, height);

  const auto draw_tile = [&](int x, int y, const tiles::Tile& tile) {
    graphics.draw(x * tile_size, y * tile_size,
      m_tiles, tile.x, tile.y, tile_size, tile_size);
  };

  const auto cells_x = (width + tile_size - 1) / tile_size;
  const auto cells_y = (height + tile_size - 1) / tile_size;

  graphics.disable_blending();
  for (auto y = 0; y < cells_y; ++y)
    for (auto x = 0; x < cells_x; ++x) {
      const auto type = cell_at(x, y).type;
      draw_tile(x, y, type == CellType::wall ?
        get_tile(tiles::Wall(), wall_at, x, y) :
        get_tile(tiles::Floor(), no_wall_at, x, y));
    }

  graphics.enable_blending();
  for (auto y = 0; y < cells_y; ++y)
    for (auto x = 0; x < cells_x; ++x) {
      const auto type = cell_at(x, y).type;
      if (type == CellType::platform_1 || type == CellType::platform_2) {
        const auto get_platform_sprite = [&]() {
          const auto left = (cell_at(x - 1, y).type == type);
          const auto right = (cell_at(x + 1, y).type == type);
          if (type == CellType::platform_1) {
            if (left && right) return sprites::platform_1_middle;
            if (left) return sprites::platform_1_right;
            if (right) return sprites::platform_1_left;
            return sprites::platform_1_sole;
          }
          if (left && right) return sprites::platform_2_middle;
          if (left) return sprites::platform_2_right;
          if (right) return sprites::platform_2_left;
          return sprites::platform_2_sole;
        };
        graphics.draw(x * tile_size, y * tile_size, get_platform_sprite());
      }
    }
  graphics.flush_drawing();

  m_world_texture = std::move(target);
}

void World::draw(Graphics& graphics) {
  graphics.draw(0, 0, m_world_texture, 0, 0,
    m_world_texture.width(), m_world_texture.height(), 1, 1, false, true);
}

float World::distance_to_ground(float x, float y) const {
  const auto cell_x = std::floor(x / tile_size);
  auto cell_y = std::floor(y / tile_size) + 1;
  auto distance = (cell_y * tile_size) - y;
  while (cell_at(cell_x, cell_y).type == CellType::room) {
    distance += tile_size;
    ++cell_y;
  }
  return distance;
}

float World::distance_to_ceiling(float x, float y) const {
  const auto cell_x = std::floor(x / tile_size);
  auto cell_y = std::floor(y / tile_size);
  auto distance = y - (cell_y * tile_size);
  while (cell_at(cell_x, cell_y - 1).type != CellType::wall) {
    distance += tile_size;
    --cell_y;
  }
  return distance;
}

float World::distance_to_wall_left(float x, float y) const {
  const auto cell_y = std::floor(y / tile_size);
  auto cell_x = std::floor(x / tile_size);
  auto distance = x - (cell_x * tile_size);
  while (cell_at(cell_x - 1, cell_y).type != CellType::wall) {
    distance += tile_size;
    --cell_x;
  }
  return distance;
}

float World::distance_to_wall_right(float x, float y) const {
  const auto cell_y = std::floor(y / tile_size);
  auto cell_x = std::floor(x / tile_size) + 1;
  auto distance = (cell_x * tile_size) - x;
  while (cell_at(cell_x, cell_y).type != CellType::wall) {
    distance += tile_size;
    ++cell_x;
  }
  return distance;
}

