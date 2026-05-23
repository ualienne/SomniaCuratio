#ifndef SOMNIA_WORLD_TILE_MAP_H
#define SOMNIA_WORLD_TILE_MAP_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace somnia {

struct MapObject {
  std::string name;
  std::string type;
  sf::Vector2f position{0.f, 0.f};
  sf::Vector2f size{0.f, 0.f};
  std::unordered_map<std::string, std::string> props;
};

class TileMap : public sf::Drawable {
 public:
  TileMap() = default;

  TileMap(const TileMap&) = delete;
  TileMap& operator=(const TileMap&) = delete;
  TileMap(TileMap&&) = default;
  TileMap& operator=(TileMap&&) = default;

  bool loadFromFile(const std::string& tmxPath);

  sf::Vector2u mapSizeTiles() const { return m_mapSize; }
  sf::Vector2u tileSizePixels() const { return m_tileSize; }
  sf::Vector2f mapSizePixels() const {
    return {static_cast<float>(m_mapSize.x) * m_tileSize.x,
            static_cast<float>(m_mapSize.y) * m_tileSize.y};
  }

  bool isBlocked(int tileX, int tileY) const;

  const std::vector<MapObject>& collisionObjects() const { return m_collision; }
  const std::vector<MapObject>& triggers() const { return m_triggers; }
  const std::vector<MapObject>& interactables() const {
    return m_interactables;
  }
  const std::vector<MapObject>& doors() const { return m_doors; }
  const std::vector<MapObject>& spawns() const { return m_spawns; }

  const MapObject* findSpawn(const std::string& name) const;

  void drawBackground(
      sf::RenderTarget& target,
      const sf::RenderStates& states = sf::RenderStates::Default) const;
  void drawDecorAbove(
      sf::RenderTarget& target,
      const sf::RenderStates& states = sf::RenderStates::Default) const;

 private:
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  struct LayerMesh {
    sf::VertexArray vertices{sf::PrimitiveType::Triangles};
    std::size_t tilesetIndex = 0;
  };

  std::vector<sf::Texture> m_tilesets;
  std::vector<unsigned int> m_tilesetFirstGids;
  std::vector<unsigned int> m_tilesetColumns;
  std::vector<sf::Vector2u> m_tilesetTileSize;

  std::vector<LayerMesh> m_backgroundMeshes;
  std::vector<LayerMesh> m_decorAboveMeshes;

  sf::Vector2u m_mapSize{0, 0};
  sf::Vector2u m_tileSize{16, 16};

  std::vector<bool> m_blockedGrid;

  std::vector<MapObject> m_collision;
  std::vector<MapObject> m_triggers;
  std::vector<MapObject> m_interactables;
  std::vector<MapObject> m_doors;
  std::vector<MapObject> m_spawns;

  std::size_t findTilesetForGid(unsigned int gid) const;
};

}  // namespace somnia

#endif  // SOMNIA_WORLD_TILE_MAP_H
