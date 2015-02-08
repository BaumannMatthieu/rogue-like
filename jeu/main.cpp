#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#define WINDOW_LARGEUR 800
#define WINDOW_HAUTEUR 600

#define TAILLE_MAP_X 20
#define TAILLE_MAP_Y 20

enum direction{UP, DOWN, RIGHT, LEFT};

#include "texture.h"
#include "tile.h"
#include "carte.h"

#include "entity.h"
#include "personnage.h"

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

std::string parser_fichier_texte(const std::string& chemin_fichier)
{
	std::string chaine = "";
	std::fstream fichier;
	fichier.open(chemin_fichier.c_str(), std::ios::in);
	
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

	return chaine;	
}

Carte* lecture_carte(const std::string& chemin_carte, SDL_Renderer* renderer)
{
	std::string chaine = parser_fichier_texte(chemin_carte);

	unsigned int **id_tiles = new unsigned int*[TAILLE_MAP_Y];
	
	std::stringstream buffer_tiles(chaine);
	
	for(unsigned int j = 0; j < TAILLE_MAP_Y; j++) {
		id_tiles[j] = new unsigned int[TAILLE_MAP_X];
		for(unsigned int i = 0; i < TAILLE_MAP_X; i++) {
			buffer_tiles >> id_tiles[j][i];
		}
	}
	
	// On parse le fichier de collisions
	std::string chaine_collision = parser_fichier_texte("collisions_tileset.txt");
	std::vector< std::vector< char > > collision_tiles;
		
	std::stringstream buffer_collisions(chaine_collision);
	unsigned int taille_tileset;
	buffer_collisions >> taille_tileset;
	for(unsigned int i = 0; i < taille_tileset; i++) {
		std::vector< char > collision_tile(4, 0);
		for(unsigned int j = 0; j < collision_tile.size(); j++) {
			buffer_collisions >> collision_tile[j];
		}
		collision_tiles.push_back(collision_tile);
	}
	
	return new Carte(id_tiles, TAILLE_MAP_X, TAILLE_MAP_Y, collision_tiles, "donjon_tileset.png", renderer); 
}

struct Tile_map
{
	Tile tile;
	unsigned int id_ligne;
	unsigned int id_colonne;
};

struct Tile_map convertPositionToTile(Carte* carte, SDL_Rect pos)
{
	pos.x -= carte->getOffsetX();
	pos.y -= carte->getOffsetY();

	unsigned int i = (pos.y - (pos.y % HAUTEUR_TILE))/HAUTEUR_TILE;
	unsigned int j = (pos.x - (pos.x % LARGEUR_TILE))/LARGEUR_TILE;
	
	struct Tile_map tile_map = {carte->getTile(i, j), i, j};		
	return tile_map;
}



bool collisionEntityEnvironnement(Personnage* entite, Carte* carte, const direction dir)
{
	SDL_Rect pos_feet;
	pos_feet.x = entite->getPosition().x + LARGEUR_ENTITY/2;
	pos_feet.y = entite->getPosition().y + HAUTEUR_ENTITY*3/4;

	SDL_Rect pos_suivante[4];
	pos_suivante[UP].x = pos_feet.x;	
	pos_suivante[UP].y = pos_feet.y - entite->getVitesse();	
	pos_suivante[DOWN].x = pos_feet.x;	
	pos_suivante[DOWN].y = pos_feet.y + HAUTEUR_ENTITY + entite->getVitesse();	
	pos_suivante[RIGHT].x = pos_feet.x + LARGEUR_ENTITY + entite->getVitesse();	
	pos_suivante[RIGHT].y = pos_feet.y;	
	pos_suivante[LEFT].x = pos_feet.x - entite->getVitesse();	
	pos_suivante[LEFT].y = pos_feet.y;	

	struct Tile_map tile_suivante_map = convertPositionToTile(carte, pos_suivante[dir]);
	struct Tile_map tile_actu_map = convertPositionToTile(carte, pos_feet);	
	
	if( tile_actu_map.id_ligne != tile_suivante_map.id_ligne or tile_actu_map.id_colonne != tile_suivante_map.id_colonne )
	{
		if(tile_actu_map.tile.getCollisions()[dir])
			return false;

		return true;
	}

	return true;
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
	fenetre = SDL_CreateWindow("Test SDL 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_LARGEUR, WINDOW_HAUTEUR, SDL_WINDOW_SHOWN);
	 
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
	Personnage guerrier(GUERRIER, "entity.png", renderer);
	bool continuer = true;
	while(continuer)
	{
		SDL_Event event;
			
		while (SDL_PollEvent(&event))
                {
			if(event.type == SDL_QUIT) {
					continuer = false;
			}
                         //Traitement de l'événement */
	                /* switch (event.type)  Quel événement avons-nous ? 
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
				break;		
			}*/
		}
			const Uint8 *keys = SDL_GetKeyboardState(NULL);
			if(keys[SDL_SCANCODE_W] && collisionEntityEnvironnement(&guerrier, carte, UP)) {
				carte->deplacer(UP, guerrier.getVitesse());
			}

			if(keys[SDL_SCANCODE_A] && collisionEntityEnvironnement(&guerrier, carte, LEFT)) {
				carte->deplacer(LEFT, guerrier.getVitesse());
			}

			if(keys[SDL_SCANCODE_S] && collisionEntityEnvironnement(&guerrier, carte, DOWN)) {
				carte->deplacer(DOWN, guerrier.getVitesse());
			}

			if(keys[SDL_SCANCODE_D] && collisionEntityEnvironnement(&guerrier, carte, RIGHT)) {
				carte->deplacer(RIGHT, guerrier.getVitesse());
			}
		
//		}

		SDL_RenderClear(renderer);
	
		carte->afficher(renderer);	
		guerrier.afficher(renderer);	
		
		SDL_RenderPresent(renderer);	
	}

	// On quitte la SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
		 
	return 0;
}
