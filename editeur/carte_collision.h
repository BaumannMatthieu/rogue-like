#ifndef __CARTE_COLLISION_H__
#define __CARTE_COLLISION_H__

using namespace std;

class Carte_Collision
{
	public:
		Carte_Collision(const unsigned int taille_x, const unsigned int taille_y, std::vector< bool>** collision_tiles, const string& file_tileset, SDL_Renderer* renderer) : offset_x(0), offset_y(0)
		{
			largeur = taille_x;
			hauteur = taille_y;

			tileset = new Texture(renderer, file_tileset);
			
			// On suppose que le tileset ne contient que des tiles de mêmes dimensions.
			lig_tileset = tileset->getHauteur()/HAUTEUR_TILE;
			col_tileset = tileset->getLargeur()/LARGEUR_TILE;
			
			collisions = collision_tiles;
		}

		virtual ~Carte_Collision() 
		{
			delete tileset;
		}

		void inverserCollisionDirection(const unsigned int i, const unsigned int j, const direction dir)
		{
			collisions[i][j].at(dir) = !collisions[i][j].at(dir);
		}

		void afficher(SDL_Renderer* renderer)
		{
			for(unsigned int i = 0; i < hauteur; i++) {
				for(unsigned int j = 0; j < largeur; j++) {
					for(unsigned int k = 0; k < 4; k++) {
						if(collisions[i][j].at(k) == true) {
							SDL_Rect src_tile, dst_tile;
							src_tile.x = LARGEUR_TILE*(k + 1);
							src_tile.y = 0;
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

		int getOffsetX() const
		{
			return offset_x;
		}
		int getOffsetY() const
		{
			return offset_y;
		}
	private:
		std::vector< bool >** collisions;
		unsigned int largeur, hauteur;
		unsigned int lig_tileset, col_tileset;
		int offset_x, offset_y;		

		Texture* tileset;
};

#endif
