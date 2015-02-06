#ifndef TEXTURE_H
#define TEXTURE_H

using namespace std;

class Texture
{
	public:
		Texture(SDL_Renderer* renderer, const std::string& nom_image)
		{
			SDL_Surface* surface_image = IMG_Load(nom_image.c_str());
			if(surface_image == NULL)
			{
        			throw "Erreur lors du chargement de l'image : " + std::string(SDL_GetError());
			}
	
			texture_image = SDL_CreateTextureFromSurface(renderer, surface_image);
			if(texture_image == NULL)
			{
        			throw "Erreur lors du chargement de la texture de l'image : " + std::string(SDL_GetError());
			}
			
			largeur = surface_image->w;
			hauteur = surface_image->h;
			SDL_FreeSurface(surface_image);
		}
		~Texture()
		{
			SDL_DestroyTexture(texture_image);
		}
			
		SDL_Texture* getSDL_Texture() const
		{
			return texture_image;
		}

		unsigned int getHauteur() const
		{
			return hauteur;
		}
		
		unsigned int getLargeur() const
		{
			return largeur;
		}
	private:
		SDL_Texture* texture_image;
		unsigned int largeur, hauteur;
};


#endif
