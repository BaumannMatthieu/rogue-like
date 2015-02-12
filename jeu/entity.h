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
		
	protected:
		Texture* sprite;
		enum Entity entity; 
		SDL_Rect pos_src, pos_dst;
		int vie;
		bool mort;
		
		Uint8 vitesse;
};

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
		} 

		virtual ~Ennemi()
		{
			
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
};

#endif
