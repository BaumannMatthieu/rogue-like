#ifndef __PERSONNAGE_H__
#define __PERSONNAGE_H__

class Personnage
{
	public:
		Personnage(const enum Entity _entity, const std::string& chemin_texture, SDL_Renderer* renderer)
		{
			sprite = new Texture(renderer, chemin_texture);
			entity = _entity;
			pos_src.x = LARGEUR_ENTITY*entity;
			pos_src.y = 0;
			pos_src.w = LARGEUR_ENTITY;
			pos_src.h = HAUTEUR_ENTITY;
			
			pos_dst.x = (WINDOW_LARGEUR - LARGEUR_ENTITY)/2;
			pos_dst.y = (WINDOW_HAUTEUR - HAUTEUR_ENTITY)/2;
			pos_dst.w = LARGEUR_ENTITY;
			pos_dst.h = HAUTEUR_ENTITY;
			
			vitesse = 4;
		} 

		~Personnage()
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
		
	private:
		Texture* sprite;
		enum Entity entity; 
		SDL_Rect pos_src, pos_dst;
		
		Uint8 vitesse;
};

#endif
