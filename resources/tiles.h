
// automatically generated with spright
// https://github.com/houmaster/spright

namespace tiles {

struct Tile {
  int x, y;
};

struct Floor {
  static constexpr Tile tile_0000 { 160, 320 };
  static constexpr Tile tile_0001 { 64, 192 };
  static constexpr Tile tile_0010 { 64, 320 };
  static constexpr Tile tile_0011 { 224, 160 };
  static constexpr Tile tile_0100 { 32, 288 };
  static constexpr Tile tile_0101 { 160, 224 };
  static constexpr Tile tile_0110 { 192, 256 };
  static constexpr Tile tile_0111 { 64, 224 };
  static constexpr Tile tile_1000 { 128, 320 };
  static constexpr Tile tile_1001 { 32, 192 };
  static constexpr Tile tile_1010 { 96, 320 };
  static constexpr Tile tile_1011 { 0, 192 };
  static constexpr Tile tile_1100 { 0, 288 };
  static constexpr Tile tile_1101 { 128, 224 };
  static constexpr Tile tile_1110 { 224, 256 };
  static constexpr Tile tile_1111 { 96, 224 };
  static constexpr Tile tile_0011_xxx0 { 0, 320 };
  static constexpr Tile tile_0110_x0xx { 128, 352 };
  static constexpr Tile tile_0111_x0x0 { 64, 352 };
  static constexpr Tile tile_0111_x0x1 { 0, 352 };
  static constexpr Tile tile_0111_x1x0 { 128, 288 };
  static constexpr Tile tile_1001_xx0x { 32, 320 };
  static constexpr Tile tile_1011_xx00 { 192, 288 };
  static constexpr Tile tile_1011_xx01 { 96, 288 };
  static constexpr Tile tile_1011_xx10 { 64, 288 };
  static constexpr Tile tile_1100_0xxx { 160, 352 };
  static constexpr Tile tile_1101_0x0x { 224, 288 };
  static constexpr Tile tile_1101_0x1x { 32, 352 };
  static constexpr Tile tile_1101_1x0x { 160, 288 };
  static constexpr Tile tile_1110_00xx { 96, 352 };
  static constexpr Tile tile_1110_01xx { 224, 320 };
  static constexpr Tile tile_1110_10xx { 192, 320 };
  static constexpr Tile tile_1111_0000 { 32, 224 };
  static constexpr Tile tile_1111_0001 { 96, 256 };
  static constexpr Tile tile_1111_0010 { 64, 256 };
  static constexpr Tile tile_1111_0011 { 192, 192 };
  static constexpr Tile tile_1111_0100 { 0, 224 };
  static constexpr Tile tile_1111_0101 { 160, 192 };
  static constexpr Tile tile_1111_0110 { 160, 256 };
  static constexpr Tile tile_1111_0111 { 224, 224 };
  static constexpr Tile tile_1111_1000 { 224, 192 };
  static constexpr Tile tile_1111_1001 { 128, 256 };
  static constexpr Tile tile_1111_1010 { 32, 256 };
  static constexpr Tile tile_1111_1011 { 192, 224 };
  static constexpr Tile tile_1111_1100 { 0, 256 };
  static constexpr Tile tile_1111_1101 { 128, 192 };
  static constexpr Tile tile_1111_1110 { 96, 192 };
};

struct Wall {
  static constexpr Tile tile_0000 { 192, 128 };
  static constexpr Tile tile_0001 { 96, 0 };
  static constexpr Tile tile_0010 { 96, 128 };
  static constexpr Tile tile_0011 { 0, 0 };
  static constexpr Tile tile_0100 { 64, 96 };
  static constexpr Tile tile_0101 { 192, 32 };
  static constexpr Tile tile_0110 { 224, 64 };
  static constexpr Tile tile_0111 { 96, 32 };
  static constexpr Tile tile_1000 { 160, 128 };
  static constexpr Tile tile_1001 { 64, 0 };
  static constexpr Tile tile_1010 { 128, 128 };
  static constexpr Tile tile_1011 { 32, 0 };
  static constexpr Tile tile_1100 { 32, 96 };
  static constexpr Tile tile_1101 { 160, 32 };
  static constexpr Tile tile_1110 { 0, 96 };
  static constexpr Tile tile_1111 { 128, 32 };
  static constexpr Tile tile_0011_xxx0 { 32, 128 };
  static constexpr Tile tile_0110_x0xx { 160, 160 };
  static constexpr Tile tile_0111_x0x0 { 96, 160 };
  static constexpr Tile tile_0111_x0x1 { 32, 160 };
  static constexpr Tile tile_0111_x1x0 { 160, 96 };
  static constexpr Tile tile_1001_xx0x { 64, 128 };
  static constexpr Tile tile_1011_xx00 { 224, 96 };
  static constexpr Tile tile_1011_xx01 { 128, 96 };
  static constexpr Tile tile_1011_xx10 { 96, 96 };
  static constexpr Tile tile_1100_0xxx { 192, 160 };
  static constexpr Tile tile_1101_0x0x { 0, 128 };
  static constexpr Tile tile_1101_0x1x { 64, 160 };
  static constexpr Tile tile_1101_1x0x { 192, 96 };
  static constexpr Tile tile_1110_00xx { 128, 160 };
  static constexpr Tile tile_1110_01xx { 0, 160 };
  static constexpr Tile tile_1110_10xx { 224, 128 };
  static constexpr Tile tile_1111_0000 { 64, 32 };
  static constexpr Tile tile_1111_0001 { 128, 64 };
  static constexpr Tile tile_1111_0010 { 96, 64 };
  static constexpr Tile tile_1111_0011 { 224, 0 };
  static constexpr Tile tile_1111_0100 { 32, 32 };
  static constexpr Tile tile_1111_0101 { 192, 0 };
  static constexpr Tile tile_1111_0110 { 192, 64 };
  static constexpr Tile tile_1111_0111 { 0, 64 };
  static constexpr Tile tile_1111_1000 { 0, 32 };
  static constexpr Tile tile_1111_1001 { 160, 64 };
  static constexpr Tile tile_1111_1010 { 64, 64 };
  static constexpr Tile tile_1111_1011 { 224, 32 };
  static constexpr Tile tile_1111_1100 { 32, 64 };
  static constexpr Tile tile_1111_1101 { 160, 0 };
  static constexpr Tile tile_1111_1110 { 128, 0 };
};

} // namespace
