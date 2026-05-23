#include "world/TileMap.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <tmxlite/Map.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Property.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Types.hpp>

namespace somnia {

static std::string propertyToString(const tmx::Property& p) {
  using Type = tmx::Property::Type;
  switch (p.getType()) {
    case Type::String:
      return p.getStringValue();
    case Type::Boolean:
      return p.getBoolValue() ? "true" : "false";
    case Type::Int:
      return std::to_string(p.getIntValue());
    case Type::Float:
      return std::to_string(p.getFloatValue());
    case Type::File:
      return p.getFileValue();
    case Type::Colour: {
      const auto c = p.getColourValue();
      char buf[16];
      std::snprintf(buf, sizeof(buf), "#%02X%02X%02X", c.r, c.g, c.b);
      return buf;
    }
    case Type::Object:
      return std::to_string(p.getObjectValue());
    default:
      return "";
  }
}

static MapObject makeObject(const tmx::Object& obj) {
  MapObject out;
  out.name = obj.getName();
  out.type = obj.getType();

  const auto pos = obj.getPosition();
  const auto aabb = obj.getAABB();

  out.position = {pos.x, pos.y};

  if (aabb.width > 0.f && aabb.height > 0.f) {
    out.size = {aabb.width, aabb.height};
  } else {
    out.size = {0.f, 0.f};
  }

  const auto& props = obj.getProperties();
  out.props.reserve(props.size());
  for (const auto& p : props) {
    const std::string& pname = p.getName();
    if (!pname.empty()) {
      out.props.emplace(pname, propertyToString(p));
    }
  }

  return out;
}

bool TileMap::loadFromFile(const std::string& tmxPath) {
  tmx::Map map;
  if (!map.load(tmxPath)) {
    std::cerr << "[TileMap] cannot load " << tmxPath << '\n';
    return false;
  }

  m_mapSize = {map.getTileCount().x, map.getTileCount().y};
  m_tileSize = {map.getTileSize().x, map.getTileSize().y};

  const auto& tilesets = map.getTilesets();
  m_tilesets.clear();
  m_tilesets.reserve(tilesets.size());
  m_tilesetFirstGids.clear();
  m_tilesetColumns.clear();
  m_tilesetTileSize.clear();

  for (const auto& ts : tilesets) {
    sf::Texture tex;
    const std::string& imgPath = ts.getImagePath();
    if (!tex.loadFromFile(imgPath)) {
      std::cerr << "[TileMap] cannot load tileset image: " << imgPath << '\n';
      return false;
    }
    tex.setSmooth(false);
    m_tilesets.push_back(std::move(tex));
    m_tilesetFirstGids.push_back(ts.getFirstGID());
    m_tilesetColumns.push_back(ts.getColumnCount());
    m_tilesetTileSize.push_back({ts.getTileSize().x, ts.getTileSize().y});
  }

  if (m_tilesets.empty()) {
    std::cerr << "[TileMap] map has no tilesets\n";
    return false;
  }

  m_blockedGrid.assign(static_cast<std::size_t>(m_mapSize.x) * m_mapSize.y,
                       false);

  m_backgroundMeshes.clear();
  m_decorAboveMeshes.clear();
  m_collision.clear();
  m_triggers.clear();
  m_interactables.clear();
  m_doors.clear();
  m_spawns.clear();

  for (const auto& layerPtr : map.getLayers()) {
    if (!layerPtr) continue;

    const std::string& layerName = layerPtr->getName();

    if (layerPtr->getType() == tmx::Layer::Type::Tile) {
      const auto& tileLayer = layerPtr->getLayerAs<tmx::TileLayer>();
      const bool isBackground = (layerName == "background");
      const bool isDecorAbove = (layerName == "decor_above");
      if (!isBackground && !isDecorAbove) continue;

      LayerMesh mesh;
      mesh.vertices.setPrimitiveType(sf::PrimitiveType::Triangles);

      const std::size_t totalTiles =
          static_cast<std::size_t>(m_mapSize.x) * m_mapSize.y;
      mesh.vertices.resize(totalTiles * 6);

      std::size_t layerTilesetIdx = 0;
      bool tilesetChosen = false;
      std::size_t vertexCursor = 0;
      const auto& tiles = tileLayer.getTiles();

      for (unsigned int y = 0; y < m_mapSize.y; ++y) {
        for (unsigned int x = 0; x < m_mapSize.x; ++x) {
          const std::size_t tileIdx =
              static_cast<std::size_t>(y) * m_mapSize.x + x;
          const unsigned int gid = tiles[tileIdx].ID;
          if (gid == 0) {
            vertexCursor += 6;
            continue;
          }

          if (!tilesetChosen) {
            layerTilesetIdx = findTilesetForGid(gid);
            tilesetChosen = true;
          }

          const unsigned int firstGid = m_tilesetFirstGids[layerTilesetIdx];
          const unsigned int localId = gid - firstGid;
          const unsigned int cols = m_tilesetColumns[layerTilesetIdx];
          const unsigned int tu = localId % cols;
          const unsigned int tv = localId / cols;

          const float tw =
              static_cast<float>(m_tilesetTileSize[layerTilesetIdx].x);
          const float th =
              static_cast<float>(m_tilesetTileSize[layerTilesetIdx].y);

          const float px = static_cast<float>(x * m_tileSize.x);
          const float py = static_cast<float>(y * m_tileSize.y);

          const float tx0 = static_cast<float>(tu) * tw;
          const float ty0 = static_cast<float>(tv) * th;
          const float tx1 = tx0 + tw;
          const float ty1 = ty0 + th;

          auto& v = mesh.vertices;
          v[vertexCursor + 0].position = {px, py};
          v[vertexCursor + 1].position = {px + tw, py};
          v[vertexCursor + 2].position = {px + tw, py + th};
          v[vertexCursor + 3].position = {px, py};
          v[vertexCursor + 4].position = {px + tw, py + th};
          v[vertexCursor + 5].position = {px, py + th};

          v[vertexCursor + 0].texCoords = {tx0, ty0};
          v[vertexCursor + 1].texCoords = {tx1, ty0};
          v[vertexCursor + 2].texCoords = {tx1, ty1};
          v[vertexCursor + 3].texCoords = {tx0, ty0};
          v[vertexCursor + 4].texCoords = {tx1, ty1};
          v[vertexCursor + 5].texCoords = {tx0, ty1};

          vertexCursor += 6;
        }
      }

      mesh.tilesetIndex = layerTilesetIdx;
      if (isBackground)
        m_backgroundMeshes.push_back(std::move(mesh));
      else
        m_decorAboveMeshes.push_back(std::move(mesh));
    }

    else if (layerPtr->getType() == tmx::Layer::Type::Object) {
      const auto& objGroup = layerPtr->getLayerAs<tmx::ObjectGroup>();

      std::vector<MapObject>* dest = nullptr;
      if (layerName == "collision")
        dest = &m_collision;
      else if (layerName == "triggers")
        dest = &m_triggers;
      else if (layerName == "interactables")
        dest = &m_interactables;
      else if (layerName == "doors")
        dest = &m_doors;
      else if (layerName == "spawns")
        dest = &m_spawns;
      else
        continue;

      const auto& objects = objGroup.getObjects();
      dest->reserve(dest->size() + objects.size());

      for (const auto& obj : objects) {
        dest->push_back(makeObject(obj));
      }
    }
  }

  for (const auto& obj : m_collision) {
    if (obj.size.x <= 0.f || obj.size.y <= 0.f) continue;

    const float ox = obj.position.x;
    const float oy = obj.position.y;
    const float ow = obj.size.x;
    const float oh = obj.size.y;

    const int tx0g = static_cast<int>(ox / m_tileSize.x);
    const int ty0g = static_cast<int>(oy / m_tileSize.y);
    const int tx1g = static_cast<int>((ox + ow - 0.001f) / m_tileSize.x);
    const int ty1g = static_cast<int>((oy + oh - 0.001f) / m_tileSize.y);

    for (int ty = ty0g; ty <= ty1g; ++ty) {
      for (int tx = tx0g; tx <= tx1g; ++tx) {
        if (tx < 0 || ty < 0 || tx >= static_cast<int>(m_mapSize.x) ||
            ty >= static_cast<int>(m_mapSize.y))
          continue;
        m_blockedGrid[static_cast<std::size_t>(ty) * m_mapSize.x + tx] = true;
      }
    }
  }

  std::cerr << "[TileMap] loaded " << tmxPath << " (" << m_mapSize.x << "x"
            << m_mapSize.y << " tiles, " << m_collision.size() << " colliders, "
            << m_spawns.size() << " spawns, " << m_doors.size() << " doors)\n";
  return true;
}

// -----------------------------------------------------------------------------
std::size_t TileMap::findTilesetForGid(unsigned int gid) const {
  std::size_t best = 0;
  unsigned int bestFirst = 0;
  for (std::size_t i = 0; i < m_tilesetFirstGids.size(); ++i) {
    const unsigned int f = m_tilesetFirstGids[i];
    if (f <= gid && f >= bestFirst) {
      best = i;
      bestFirst = f;
    }
  }
  return best;
}

bool TileMap::isBlocked(int tileX, int tileY) const {
  if (tileX < 0 || tileY < 0 || tileX >= static_cast<int>(m_mapSize.x) ||
      tileY >= static_cast<int>(m_mapSize.y))
    return true;
  return m_blockedGrid[static_cast<std::size_t>(tileY) * m_mapSize.x + tileX];
}

const MapObject* TileMap::findSpawn(const std::string& name) const {
  for (const auto& s : m_spawns)
    if (s.name == name) return &s;
  return nullptr;
}

void TileMap::drawBackground(sf::RenderTarget& target,
                             const sf::RenderStates& states) const {
  for (const auto& mesh : m_backgroundMeshes) {
    sf::RenderStates s = states;
    s.texture = &m_tilesets[mesh.tilesetIndex];
    target.draw(mesh.vertices, s);
  }
}

void TileMap::drawDecorAbove(sf::RenderTarget& target,
                             const sf::RenderStates& states) const {
  for (const auto& mesh : m_decorAboveMeshes) {
    sf::RenderStates s = states;
    s.texture = &m_tilesets[mesh.tilesetIndex];
    target.draw(mesh.vertices, s);
  }
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  drawBackground(target, states);
}

}  // namespace somnia
