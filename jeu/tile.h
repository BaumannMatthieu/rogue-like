#ifndef __TILE_H__
#define __TILE_H__

#define LARGEUR_TILE 16
#define HAUTEUR_TILE 16

using namespace std;

class Tile
{
	public:
		Tile(const unsigned int _id_tile)
		{
			id_tile = _id_tile;
			collision.assign(4, false);
		}

		Tile(const unsigned int _id_tile, const vector< char > _collision)
		{
			collision = _collision;
			id_tile = _id_tile;
		}
		~Tile() 
		{

		}

		unsigned int getIdTile()
		{
			return id_tile;
		}

		void setIdTile(const unsigned int _id_tile)
		{
			id_tile = _id_tile;
		}

		std::vector< char > getCollision()
		{
			return collision;
		}

		void setCollision(const std::vector< char > _collision)
		{
			collision = _collision;
		}

		void inverserCollision(const direction dir)
		{
			collision[dir] = !collision[dir];
		}
	private:
		vector< char > collision;
		unsigned int id_tile;
};

#endif
