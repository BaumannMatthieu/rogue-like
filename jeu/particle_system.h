#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__
#include <cstdio>
#include <cstdlib>
#include <ctime>

struct Vecteur2D
{
	float x, y;
};

class Particle
{
	public:
		Particle(const SDL_Rect pos, const struct Vecteur2D vitesse, const SDL_Color color, const Uint32 duree_t) : r(pos), v(vitesse), couleur(color), temps_de_naissance(SDL_GetTicks()), temps_de_vie(duree_t)
		{

		}
		~Particle()
		{

		}

		// précondition : la particule est toujours active 
		void update()
		{
			r.x += v.x;
			r.y += v.y;

			float pourcentage_vie = (float)((SDL_GetTicks() - temps_de_naissance)/(float)(temps_de_vie));
			couleur.a = (255.0*(1.0 - pourcentage_vie));
		}

		Uint32 getTempsDeVie()
		{
			return temps_de_vie;
		}

		Uint32 getTempsDeNaissance()
		{
			return temps_de_naissance;
		}

		void afficher(SDL_Renderer* renderer)
		{
    			SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, couleur.a);
    			SDL_RenderFillRect(renderer, &r);
		}

	private:
		SDL_Rect r;
		struct Vecteur2D v;
		SDL_Color couleur;

		Uint32 temps_de_vie;
		Uint32 temps_de_naissance;
};

class ParticleSystem
{
	public:
		ParticleSystem(const Uint32 _tempsDeVie)
		{
			std::srand(time(NULL));
			nombreParticles = 10;
			largeurParticle = 6;
			hauteurParticle = 6;
			struct Vecteur2D vitesse = {sqrt(2)/2, sqrt(2)/2};
			//creation des particules
			for(unsigned int i = 0; i < nombreParticles; i++)
			{
				SDL_Color couleur = {255, rand() % 256, 0, 255};
				SDL_Rect pos_particle = {rand() % WINDOW_LARGEUR, rand() % WINDOW_HAUTEUR, largeurParticle, hauteurParticle};
				Particle* particle = new Particle(pos_particle, vitesse, couleur, _tempsDeVie);
				particles.push_back(particle);
			}
		}

		~ParticleSystem()
		{
			for(unsigned int i = 0; i < particles.size(); i++) {
				delete particles[i];
			}
		}

		void update()
		{	
			for(unsigned int i = 0; i < particles.size(); i++) {
				if(SDL_GetTicks() - particles[i]->getTempsDeNaissance() > particles[i]->getTempsDeVie()) {
					delete particles[i];
					particles.erase(particles.begin() + i);

					// On cree une nouvelle particule
					SDL_Rect pos_particle = {rand() % WINDOW_LARGEUR, rand() % WINDOW_HAUTEUR, largeurParticle, hauteurParticle};
					struct Vecteur2D vitesse = {2, 2};
					SDL_Color couleur = {255, rand() % 256, 0, 255};
					Particle* particle = new Particle(pos_particle, vitesse, couleur, 2000);
					particles.push_back(particle);
				} else {
					particles[i]->update();
				}
			}
		}

		void afficher(SDL_Renderer* renderer)
		{
			for(unsigned int i = 0; i < particles.size(); i++) {
				particles[i]->afficher(renderer);
			}
		}

		
		
	private:
		std::vector< Particle* > particles;
		SDL_Rect point_depart;
		Uint8 largeurParticle, hauteurParticle;
		Uint8 nombreParticles;
};

#endif
