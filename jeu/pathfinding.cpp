#include <iostream>
#include <algorithm>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "pathfinder.h"
#include "constantes.h"

Pathfinder::Pathfinder()
{
}

Pathfinder::Pathfinder(Carte* carte)
{
	_carte = carte;
}

Pathfinder::~Pathfinder()
{
	_carte = NULL;
}
std::vector< sf::Vector2< int > > Pathfinder::A_Star(const unsigned int start_x, const unsigned int start_y, const unsigned int arrive_x, const unsigned int arrive_y)
{
	_carte->getTile(start_x, start_y).setProprietes(0.0f, distance_h(start_x, start_y, arrive_x, arrive_y));
	liste_ouverte.push_back(&_carte->getTile(start_x, start_y));
	while(liste_ouverte.size() != 0)
	{
		Tiles* tile_actu = min_f(liste_ouverte);
		if(!isTile(tile_actu, liste_fermee))
		{
		liste_fermee.push_back(tile_actu);
		for(unsigned int i = 0; i < liste_ouverte.size(); i++)
		{
		if(tile_actu == liste_ouverte[i])
		{
		liste_ouverte.erase(liste_ouverte.begin() + i);
		break;
		}
		}
		}
		if(tile_actu == &_carte->getTile(arrive_x, arrive_y))
		{
		break;
		}
		int tab_j[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
		int tab_i[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
		for(unsigned int i = 0; i < TAILLE_CARTE_X; i++)
		{
		for(unsigned int j = 0; j < TAILLE_CARTE_Y; j++)
		{
		if(tile_actu == &_carte->getTile(i, j))
		{
		for(unsigned int k = 0; k < 8; k++)
		{
		if(i + tab_i[k] < 0 || i + tab_i[k] >= _carte->getTiles().size() || j + tab_j[k] < 0 || j + tab_j[k] >= _carte->getTiles()[i].size())
		{
		continue;
		}
		else
		{
		if(_carte->getTile(i + tab_i[k], j + tab_j[k]).getCollision() == false || isTile(&_carte->getTile(i + tab_i[k], j + tab_j[k]), liste_fermee) == true)
		{
		continue;
		}
		else if(isTile(&_carte->getTile(i + tab_i[k], j + tab_j[k]), liste_ouverte) == false)
		{
		_carte->getTile(i + tab_i[k], j + tab_j[k]).setParent(&_carte->getTile(i, j));
		_carte->getTile(i + tab_i[k], j + tab_j[k]).setProprietes(_carte->getTile(i, j).get_g() + get_tile_g(tab_i[k], tab_j[k]), distance_h(i + tab_i[k], j + tab_j[k], arrive_x, arrive_y));
		liste_ouverte.push_back(&_carte->getTile(i + tab_i[k], j + tab_j[k]));
		}
		else if(isTile(&_carte->getTile(i + tab_i[k], j + tab_j[k]), liste_ouverte) == true)
		{
		if((_carte->getTile(i, j).get_g() + get_tile_g(tab_i[k], tab_j[k])) < _carte->getTile(i + tab_i[k], j + tab_j[k]).get_g())
		{
		_carte->getTile(i + tab_i[k], j + tab_j[k]).setParent(&_carte->getTile(i, j));
		_carte->getTile(i + tab_i[k], j + tab_j[k]).setProprietes(_carte->getTile(i, j).get_g() + get_tile_g(tab_i[k], tab_j[k]), distance_h(i + tab_i[k], j + tab_j[k], arrive_x, arrive_y));
		}
		}
		}
		}
		}
		}
		}
	}

	if(liste_ouverte.size() == 0)
	{
		std::vector< sf::Vector2< int > > start;
		start.push_back(sf::Vector2< int >(start_x, start_y));
		return start;
	}
	return reconstruire_chemin(liste_fermee, &_carte->getTile(start_x, start_y), &_carte->getTile(arrive_x, arrive_y));
}
int Pathfinder::distance_h(const unsigned int tile_x, const unsigned int tile_y, const unsigned int arrive_x, const unsigned int arrive_y)
{
return 10*(abs(arrive_x - tile_x) + abs(arrive_y - tile_y));
}
bool Pathfinder::isTile(const Tiles* tile, const std::vector< Tiles* > tiles)
{
for(unsigned int i = 0; i < tiles.size(); i++)
{
if(tiles[i] == tile)
return true;
}
return false;
}
Tiles* Pathfinder::min_f(std::vector< Tiles* >& tiles)
{
Tiles* tile_min = tiles[0];
for(unsigned int i = 0; i < tiles.size(); i++)
{
if(tiles[i]->get_f() < tile_min->get_f())
tile_min = tiles[i];
}
return tile_min;
}
int Pathfinder::get_tile_g(int i, int j)
{
if(abs(i) + abs(j) == 2)
return 14;
else
return 10;
}
std::vector< sf::Vector2< int > > Pathfinder::reconstruire_chemin(const std::vector< Tiles* >& liste_fermee, const Tiles* start, const Tiles* arrive)
{
std::vector< Tiles* > chemin;
chemin.clear();
do
{
Tiles* tile_actu;
if(chemin.size() == 0)
tile_actu = liste_fermee.back();
else
{
if(isTile((chemin.back())->getParent(), liste_fermee))
{
for(unsigned int i = 0; i < liste_fermee.size(); i++)
{
if((chemin.back())->getParent() == liste_fermee[i])
{
tile_actu = liste_fermee[i];
}
}
}
}
chemin.push_back(tile_actu);
}while(chemin.back() != start);
std::reverse(chemin.begin(), chemin.end());
std::vector< sf::Vector2< int > > chemin_id;
for(unsigned int i = 0; i < chemin.size(); i++)
{
chemin_id.push_back(_carte->getTileId(chemin[i]));
}
return chemin_id;
}

