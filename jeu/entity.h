#ifndef __ENTITY_H__
#define __ENTITY_H_

#define LARGEUR_ENTITY 24
#define HAUTEUR_ENTITY 24

enum Entity	{GUERRIER,
		VIKING,
		FANTOME_BLEU,
		FANTOME_VERT,
		FANTOME_ROUGE};	

class Entite
{
	public:
		Entite(const enum Entity _entity, const std::string& chemin_texture, SDL_Renderer* renderer) : vie(100), mort(false)
		{
			sprite = new Texture(renderer, chemin_texture);
			entity = _entity;
			pos_src.x = LARGEUR_ENTITY*entity;
			pos_src.y = 0;
			pos_src.w = LARGEUR_ENTITY;
			pos_src.h = HAUTEUR_ENTITY;
		} 

		virtual ~Entite()
		{
			delete sprite;
		}

		void afficher(SDL_Renderer* renderer)
		{
			SDL_RenderCopy(renderer, sprite->getSDL_Texture(), &pos_src, &pos_dst);
		}

		Uint8 getVitesse()
		{
			return vitesse;
		}
	
		SDL_Rect getPosition()
		{
			return pos_dst;
		}

		void attaque(Entite* cible, const Uint8 degats)
		{
			cible->vie -= degats;
			cout << cible->vie << endl;		
			if(cible->vie <= 0)
				cible->mort = true;
		}

		bool estMort() const
		{
			return mort;
		}
		
		virtual void deplacer(const direction dir) = 0;
		virtual void deplacer_tile(const struct id_tile pos, Carte* carte) = 0;
		
	protected:
		Texture* sprite;
		enum Entity entity; 
		SDL_Rect pos_src, pos_dst;
		int vie;
		bool mort;
		
		Uint8 vitesse;
};

struct id_tile
{
	unsigned int x, y;
};

SDL_Rect ajustementPositionEntity(SDL_Rect pos_dst, Carte* carte)
{
	SDL_Rect position_ajustee = {pos_dst.x + LARGEUR_ENTITY/2 - carte->getOffsetX(), pos_dst.y + HAUTEUR_ENTITY/2 - carte->getOffsetY(), pos_dst.w, pos_dst.h};	

	return position_ajustee;
}

id_tile conversionPositionIdTile(SDL_Rect pos_dst, Carte* carte)
{
	pos_dst = ajustementPositionEntity(pos_dst, carte);
	unsigned int x = (pos_dst.x - (pos_dst.x % LARGEUR_TILE))/LARGEUR_TILE;
	unsigned int y = (pos_dst.y - (pos_dst.y % HAUTEUR_TILE))/HAUTEUR_TILE;
	
	struct id_tile pos_tile = {x, y};
	cout << x << " " << y << endl;
	return pos_tile;
}

class Ennemi : public Entite
{
	public:
		Ennemi(const enum Entity _entity, const std::string& chemin_texture, SDL_Renderer* renderer) : Entite(_entity, chemin_texture, renderer)
		{
			pos_dst.x = (WINDOW_LARGEUR - TAILLE_MAP_X*LARGEUR_TILE - HAUTEUR_ENTITY)/2 + rand() % (TAILLE_MAP_X*LARGEUR_TILE);
			pos_dst.y = (WINDOW_HAUTEUR - TAILLE_MAP_Y*HAUTEUR_TILE - HAUTEUR_ENTITY)/2 + rand() % (TAILLE_MAP_Y*HAUTEUR_TILE);
			pos_dst.w = LARGEUR_ENTITY;
			pos_dst.h = HAUTEUR_ENTITY;

			vitesse = 4;

			deplacement = false;
		} 

		virtual ~Ennemi()
		{
			
		}
			
		void deplacer_tile(const struct id_tile pos, Carte* carte)
		{
			if(pos.x < pos_tile.x) {
				deplacer(RIGHT);
			} else if(pos.x > pos_tile.x) {
				deplacer(LEFT);
			} else if(pos.y < pos_tile.y) {
				deplacer(DOWN);
			} else if(pos.y > pos_tile.y) {
				deplacer(UP);
			}

			//mise à jour du champ pos_tile
			pos_tile = conversionPositionIdTile(pos_dst, carte);
		}
		
		virtual void deplacer(const direction dir)
		{
			switch(dir)
			{
				case UP:
					pos_dst.x += 0;
					pos_dst.y += vitesse;		
				break;
				case DOWN:
					pos_dst.x += 0;
					pos_dst.y += -vitesse;		
				break;
				case LEFT:
					pos_dst.x += vitesse;
					pos_dst.y += 0;		
				break;
				case RIGHT:
					pos_dst.x += -vitesse;
					pos_dst.y += 0;		
				break;
				default:
				break;
			}
		}

	private:
		bool deplacement;
		struct id_tile pos_tile;
};

#endif
