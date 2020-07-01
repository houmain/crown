#pragma once

#include "framework/Texture.h"
#include <vector>

class Graphics;

enum class CellType {
  wall,
  floor,
  platform_1,
  platform_2,
};

enum class CellDecoration {
  none,
  window,
  banner
};

struct Cell {
  CellType type;
  CellDecoration decoration;
};

class World {
public:
  static constexpr int tile_size = 32;

  World();

  void generate(int cells_x, int cells_y, int seed);
  void generate_texture(Graphics& graphics, int width, int height);
  void draw(Graphics& graphics);

  float distance_to_ground(float x, float y) const;
  float distance_to_ceiling(float x, float y) const;
  float distance_to_wall_left(float x, float y) const;
  float distance_to_wall_right(float x, float y) const;

private:
  const Cell& cell_at(int x, int y) const {
    static const Cell s_outside{ CellType::wall, CellDecoration::none };
    return (x < 0 || y < 0 || x >= m_cells_x || y >= m_cells_y ?
      s_outside : m_cells[y * m_cells_x + x]);
  }

  const Texture m_tiles;
  int m_cells_x{ };
  int m_cells_y{ };
  std::vector<Cell> m_cells;
  Texture m_world_texture;
};
