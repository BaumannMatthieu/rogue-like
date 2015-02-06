#ifndef CARTE_H
#define CARTE_H

using namespace std;

class Carte
{
	public:
		Carte(unsigned int** id_tiles, const unsigned int taille_x, const unsigned int taille_y, bool** collision_tiles, const string& file_tileset, SDL_Renderer* renderer) : offset_x(0), offset_y(0)
		{
			largeur = taille_x;
			hauteur = taille_y;

			tileset = new Texture(renderer, file_tileset);
			
			// On suppose que le tileset ne contient que des tiles de mêmes dimensions.
			lig_tileset = tileset->getHauteur()/HAUTEUR_TILE;
			col_tileset = tileset->getLargeur()/LARGEUR_TILE;
			
			for(unsigned int i = 0; i < taille_x; i++) {
				vector< Tile > tiles_ligne;
				for(unsigned int j = 0; j < taille_y; j++) {
					Tile tile_courante(id_tiles[i][j], collision_tiles[i][j]);
					tiles_ligne.push_back(tile_courante);
				}
				tiles.push_back(tiles_ligne);
			}
		}

		virtual ~Carte() 
		{
			delete tileset;
		}

		void afficher(SDL_Renderer* renderer)
		{
			for(unsigned int i = 0; i < tiles.size(); i++) {
				for(unsigned int j = 0; j < tiles[i].size(); j++) {
					SDL_Rect src_tile, dst_tile;
					src_tile.x = LARGEUR_TILE*(tiles[i][j].getIdTile() % col_tileset);
					src_tile.y = HAUTEUR_TILE*(tiles[i][j].getIdTile() - (tiles[i][j].getIdTile() % col_tileset))/col_tileset;
					src_tile.w = LARGEUR_TILE;
					src_tile.h = HAUTEUR_TILE;

					dst_tile.x = offset_x + HAUTEUR_TILE*j;
					dst_tile.y = offset_y + LARGEUR_TILE*i;
					dst_tile.w = LARGEUR_TILE;
					dst_tile.h = HAUTEUR_TILE;

					SDL_RenderCopy(renderer, tileset->getSDL_Texture(), &src_tile, &dst_tile);
				}
			}
		}
		void deplacer(const direction dir)
		{
			switch(dir)
			{
				case UP:
					offset_x += 0;
					offset_y += HAUTEUR_TILE;		
				break;
				case DOWN:
					offset_x += 0;
					offset_y += -HAUTEUR_TILE;		
				break;
				case LEFT:
					offset_x += LARGEUR_TILE;
					offset_y += 0;		
				break;
				case RIGHT:
					offset_x += -LARGEUR_TILE;
					offset_y += 0;		
				break;
				default:
				break;
			}
		}

		vector< vector< Tile > > getTiles() const
		{
			return tiles;
		}

		void setTile(const unsigned int i, const unsigned int j, const Tile tile)
		{
			tiles[i][j] = tile;
		}
		Tile getTile(const unsigned int i, const unsigned int j)
		{
			return tiles[i][j];
		}
		int getOffsetX() const
		{
			return offset_x;
		}
		int getOffsetY() const
		{
			return offset_y;
		}
	private:
		std::vector< std::vector< Tile > > tiles;
		unsigned int largeur, hauteur;
		unsigned int lig_tileset, col_tileset;
		int offset_x, offset_y;		

		Texture* tileset;
};

#endif
