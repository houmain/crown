#pragma once

#include "Graphics.h"

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

class World : public Graphics {
public:
  float distance_to_ground(float x, float y) const;
  float distance_to_ceiling(float x, float y) const;
  float distance_to_wall_left(float x, float y) const;
  float distance_to_wall_right(float x, float y) const;

protected:
  void generate_world(int cells_x, int cells_y, int seed);
  void draw_world();

private:
  void generate_world_texture();

  const Cell& cell_at(int x, int y) const {
    static const Cell s_outside{ CellType::wall, CellDecoration::none };
    return (x < 0 || y < 0 || x >= m_cells_x || y >= m_cells_y ?
      s_outside : m_cells[y * m_cells_x + x]);
  }

  int m_cells_x{ };
  int m_cells_y{ };
  std::vector<Cell> m_cells;
  Texture m_world_texture;
};
