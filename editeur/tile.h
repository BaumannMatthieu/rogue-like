#ifndef TILE_H
#define TILE_H

#define LARGEUR_TILE 16
#define HAUTEUR_TILE 16

class Tile
{
	public:
		Tile(const unsigned int _id_tile, const bool _collision)
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

		void inverserCollision()
		{
			collision = !collision;
		}

		bool getCollision()
		{
			return collision;
		}
	private:
		bool collision;
		unsigned int id_tile;
};

#endif
