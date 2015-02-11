#ifndef __CARTE_TILESET_H__
#define __CARTE_TILESET_H__

using namespace std;

class Carte_Tileset
{
	public:
		Carte_Tileset(unsigned int** id_tiles, const unsigned int taille_x, const unsigned int taille_y, const string& file_tileset, SDL_Renderer* renderer)
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
					Tile tile_courante(id_tiles[i][j]);
					tiles_ligne.push_back(tile_courante);
				}
				tiles.push_back(tiles_ligne);
			}
		}

		virtual ~Carte_Tileset() 
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

					dst_tile.x = HAUTEUR_TILE*j;
					dst_tile.y = LARGEUR_TILE*i;
					dst_tile.w = LARGEUR_TILE;
					dst_tile.h = HAUTEUR_TILE;

					SDL_RenderCopy(renderer, tileset->getSDL_Texture(), &src_tile, &dst_tile);
				}
			}
		}
	
		Tile getTile(const unsigned int i, const unsigned int j)
		{
			return tiles[i][j];
		}

	private:
		std::vector< std::vector< Tile > > tiles;
		unsigned int largeur, hauteur;
		unsigned int lig_tileset, col_tileset;

		Texture* tileset;
};

#endif
