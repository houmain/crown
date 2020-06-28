#pragma once

enum class ObjectType {
  player,
  pig_king,
  pig,
};

class Object {
public:
  void update();

private:
  ObjectType m_type{ };
  float m_x{ };
  float m_y{ };
  float m_velocity_x{ };
  float m_velocity_y{ };
};
