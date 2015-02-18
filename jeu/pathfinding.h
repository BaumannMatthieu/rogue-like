#ifndef __PATHFINDER_H__
#define __PATHFINDER_H__

#include <vector>
#include <algorithm>
#include <map>

using namespace std;

typedef struct
{
	int i, j;
} Pos;

typedef struct
{
	Tile* tile;
	struct Pos pos;
} Noeud;

int heuristique_cout_estime(Noeud depart, Noeud arrive)
{
	return 10*(abs(depart.pos.i - arrive.pos.i) + abs(depart.pos.j - arrive.pos.j));
}

vector< Pos > chemin_reconstruit(map< Noeud, Noeud > chemin, Noeud arrive, Noeud depart) 
{
	vector< Pos > total_path;
	Noeud courant = arrive;

	while(courant.pos != depart.pos) {
		total_path.push_back(courant.pos);	
		courant = chemin[courant];
	}
	
	total_path.push_back(depart.pos);
	reverse(total_path.begin(), total_path.end());
	
	return total_path;
}

vector< Noeud > creer_liste_voisins(Noeud courant, Carte* carte)
{
	vector< Noeud > voisins;
	
	int voisins_x[] = {-1, 1, 0, 0};
	int voisins_y[] = {0, 0, 1, -1};

	for(unsigned int k = 0; k < 4; k++) {
		int hauteur = courant.pos.i + voisins_x[k];
		int largeur = courant.pos.j + voisins_y[k];
	
		if(hauteur >= 0 && hauteur < carte->getTiles().size() && largeur >= 0 && largeur <= carte->getTiles()[hauteur].size()) {
			if(courant.tile->getCollision()[k] == '0') {
				Tile* tile_voisin_actuel = &(carte->getTiles()[hauteur][largeur]);
				Pos pos_voisin_actuel = {largeur, hauteur};
				Noeud voisin_actuel = {tile_voisin_actuel, pos_voisin_actuel};
				
				voisins.push_back(voisin_actuel);
			}
		}
	}
	
	return voisins;
}

bool noeud_dans_liste(Noeud courant, vector< Noeud > liste)
{
	for(unsigned int i = 0; i < liste.size(); i++) {
		if(courant.pos == liste[i].pos) {
			return true;
		}
	}
	return false;
}

void effacer_vector(Noeud courant, vector< Noeud >& liste)
{
	for(unsigned int i = 0; i < liste.size(); i++) {
		if(courant.pos == liste[i].pos) {
			liste.erase(liste.begin() + i);
			break;
		}
	}
}

Noeud minimum_f_score(const vector< Noeud > liste, const map< Noeud, float > f_score)
{
	Noeud noeud_f_mini = liste[0];
	
	for(unsigned int i = 1; i < liste.size(); i++) {
		if(f_score[liste[i]] < f_score[noeud_f_mini]) {
			noeud_f_mini = liste[i];
		}
	}
	return noeud_f_mini;
}

vector< Pos > recherche_chemin(Noeud depart, Noeud arrive, Carte* carte)
{
	vector< Noeud > liste_fermee;
	vector< Noeud > liste_ouverte;

	map< Noeud, Noeud > chemin;
	map< Noeud, int > g_score;	
	map< Noeud, int > f_score;	

	g_score[depart] = 0;
	f_score[depart] = g_score[depart] + heuristique_cout_estime(depart, arrive);
	liste_ouverte.push_back(depart);

	while(liste_ouverte.size() != 0) {
		Noeud courant = minimum_f_score(liste_ouverte, f_score);
		if(courant.pos == arrive.pos) {
			return chemin_reconstruit(chemin, arrive, depart);
		}

		effacer_vector(courant, liste_ouverte);
		liste_fermee.push_back(courant);

		vector< Noeud > voisins = creer_liste_voisins(courant, carte);
		
		for(unsigned int k = 0; k < voisins.size(); k++) {
			Noeud voisin_actuel = voisins[k];
			
			if(noeud_dans_liste(voisin_actuel, liste_fermee))
				continue;

			int tentative_g_score = g_score[courant] + 10;

			if(!noeud_dans_liste(voisin_actuel, liste_ouverte) or tentative_g_score < g_score[voisin_actuel]) {
				chemin[voisin_actuel] = courant;
				g_score[voisin_actuel] = tentative_g_score;
				f_score[voisin_actuel] = g_score[voisin_actuel] + heuristique_cout_estime(voisin_actuel, arrive);
				if(!noeud_dans_liste(voisin_actuel, liste_ouverte) {
					liste_ouverte.push_back(voisin_actuel);
				}
			}
		}	
	}

	return vector< Pos >(1, depart);
}


#endif
