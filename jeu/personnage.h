#ifndef __PERSONNAGE_H__
#define __PERSONNAGE_H__

class Personnage : public Entite
{
	public:
		Personnage(const enum Entity _entity, const std::string& chemin_texture, SDL_Renderer* renderer) : Entite(_entity, chemin_texture, renderer)
		{
			pos_dst.x = (WINDOW_LARGEUR - LARGEUR_ENTITY)/2;
			pos_dst.y = (WINDOW_HAUTEUR - HAUTEUR_ENTITY)/2;
			pos_dst.w = LARGEUR_ENTITY;
			pos_dst.h = HAUTEUR_ENTITY;
			
			vitesse = 4;
		} 

		~Personnage()
		{
		}

		virtual void deplacer(const direction dir)
		{
			
		}
};

#endif
