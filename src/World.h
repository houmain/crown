#pragma once

#include "Graphics.h"

enum class CellType {
  wall,
  floor,
  platform,
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
  void generate(int cells_x, int cells_y, int seed);
  int cells_x() const { return m_cells_x; }
  int cells_y() const { return m_cells_y; }
  const std::vector<Cell>& cells() const { return m_cells; }
  const Cell& cell_at(int x, int y) const {
    static const Cell s_outside{ CellType::wall, CellDecoration::none };
    return (x < 0 || y < 0 || x >= m_cells_x || y >= m_cells_y ?
      s_outside : m_cells[y * m_cells_x + x]);
  }

  Texture generate_texture(Graphics& graphics) const;

private:
  int m_cells_x{ };
  int m_cells_y{ };
  std::vector<Cell> m_cells;
};
