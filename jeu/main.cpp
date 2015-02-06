#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define TAILLE_MAP_X 20
#define TAILLE_MAP_Y 20

enum direction{UP, DOWN, RIGHT, LEFT};

#include "texture.h"
#include "tile.h"
#include "carte.h"

class Curseur
{
	public:
		Curseur(SDL_Renderer* renderer, const std::string& nom_image)
		{
			image = new Texture(renderer, nom_image);
			pos.x = 0;	
			pos.y = 0;
			pos.w = image->getLargeur();
			pos.h = image->getHauteur();
		}

		~Curseur()
		{
			delete image;
		}

		void deplacer(const direction d, const unsigned int largeur_tileset, unsigned int hauteur_tileset)
		{
			switch(d) {
				case UP:
					if(pos.y > 0)
						pos.y -= pos.h;
				break;
				case DOWN:
					if(pos.y < hauteur_tileset - pos.h)
						pos.y += pos.h;
				break;
				case RIGHT:
					if(pos.x < largeur_tileset - pos.w)
						pos.x += pos.w;
				break;
				case LEFT:
					if(pos.x > 0)
						pos.x -= pos.w;
				break;
				default:
				break;
			}
		}

		void deplacer(const unsigned int pos_x, const unsigned int pos_y)
		{
			pos.x = pos_x;
			pos.y = pos_y;
		}

		void afficher(SDL_Renderer* renderer)
		{
			SDL_RenderCopy(renderer, image->getSDL_Texture(), NULL, &pos);
		}

		SDL_Rect getPosition() const
		{
			return pos;
		}
	private:
		Texture* image;
		SDL_Rect pos;
};

void editerCaseTile(Carte* carte, Carte* carte_tileset, Curseur* curseur_map, Curseur* curseur_tileset)
{
	unsigned int i = (-carte->getOffsetX() + curseur_map->getPosition().x)/LARGEUR_TILE;
	unsigned int j = (-carte->getOffsetY() + curseur_map->getPosition().y)/HAUTEUR_TILE;
		
	unsigned int k = curseur_tileset->getPosition().x/LARGEUR_TILE;
	unsigned int l = curseur_tileset->getPosition().y/HAUTEUR_TILE;

	if( i >= 0 && i < TAILLE_MAP_X && j >= 0 && j < TAILLE_MAP_Y )
		carte->setTile(j, i, carte_tileset->getTile(l, k));
}

void editerCaseCollision(Carte* carte, Curseur* curseur_map, Carte* carte_collision)
{
	unsigned int i = (-carte->getOffsetX() + curseur_map->getPosition().x)/LARGEUR_TILE;
	unsigned int j = (-carte->getOffsetY() + curseur_map->getPosition().y)/HAUTEUR_TILE;

	if( i >= 0 && i < TAILLE_MAP_X && j >= 0 && j < TAILLE_MAP_Y ) {
		Tile tile_carte_actuelle = carte->getTile(j, i);
		tile_carte_actuelle.inverserCollision();
		carte->setTile(j, i, tile_carte_actuelle);
		
		Tile tile_collision_actuelle = carte_collision->getTile(j, i);
		tile_collision_actuelle.setIdTile( tile_carte_actuelle.getCollision() ? 1 : 0 );
		carte_collision->setTile( j, i, tile_collision_actuelle );
	}
}


Carte* lecture_carte(const std::string& chemin_carte, SDL_Renderer* renderer)
{
	std::string chaine = "";
	std::fstream fichier;
	fichier.open(chemin_carte.c_str(), std::ios::in);
	
	if(fichier.good()) {
		std::string parser_actu = "";
		while(parser_actu != "$") {
			fichier >> parser_actu;
			chaine += parser_actu + " "; 
		}
		fichier.close();
	} else {
		std::cerr << "Impossible d'ouvrir le fichier" << std::endl;
	}
	
	unsigned int **id_tiles = new unsigned int*[TAILLE_MAP_Y];
	bool **collision_tiles = new bool*[TAILLE_MAP_Y];
	
	int test2;

	int i = 1;
	
	std::stringstream buf(chaine);
	cout << buf.str() << endl;	
	/*while(i != 40*20) {
		int test;
		if(!(buf >> test)) {
			cout << "|";
		} else {
			cout << test << " ";
		}
		i++;
	}*/

	for(unsigned int j = 0; j < TAILLE_MAP_Y; j++) {
		id_tiles[j] = new unsigned int[TAILLE_MAP_X];
		collision_tiles[j] = new bool[TAILLE_MAP_X];
		for(unsigned int i = 0; i < TAILLE_MAP_X; i++) {
			buf >> id_tiles[j][i];
			buf >> collision_tiles[j][i];
		}
	}
	cout << "ok" << endl;

	return new Carte(id_tiles, TAILLE_MAP_X, TAILLE_MAP_Y, collision_tiles, "donjon_tileset.png", renderer); 
}

int main(int argc, char **argv)
{     
	// Notre fenêtre        
	SDL_Window* fenetre(0);
	SDL_Event evenements;
	bool terminer(false);		 
		 
	// Initialisation de la SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
		SDL_Quit();
			 
		return -1;
	}
		 
		 
	// Création de la fenêtre
	fenetre = SDL_CreateWindow("Test SDL 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	 
	if(fenetre == 0)
	{
		std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
		SDL_Quit();
	 
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // Création du renderer
	if(renderer == 0) {
		std::cout << "Erreur lors de la creation du render : " << SDL_GetError() << std::endl;
	}
	
	Carte* carte = lecture_carte("carte1.txt", renderer);

	bool continuer = true;
	while(continuer)
	{
		SDL_Event event;
		while ( SDL_PollEvent(&event) )
                {
                        /* Traitement de l'événement */
	                switch (event.type) /* Quel événement avons-nous ? */
        	        {
				case SDL_WINDOWEVENT: // Événement de la fenêtre
					if (event.window.event == SDL_WINDOWEVENT_CLOSE) { // Fermeture de la fenêtre
						continuer = false;
					}
            			break;
        			case SDL_KEYDOWN: // Événement de relâchement d'une touche clavier
					if (event.key.keysym.sym == SDLK_z) {
						carte->deplacer(UP);
					} else if(event.key.keysym.sym == SDLK_s) {
						carte->deplacer(DOWN);
					} else if(event.key.keysym.sym == SDLK_q) {
						carte->deplacer(LEFT);
					} else if(event.key.keysym.sym == SDLK_d) {
						carte->deplacer(RIGHT);
					} 

            			break;
				default:
				break;		
			}
		}

		SDL_RenderClear(renderer);
	
		carte->afficher(renderer);	
	
		SDL_RenderPresent(renderer);	
	}

	// On quitte la SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
		 
	return 0;
}
