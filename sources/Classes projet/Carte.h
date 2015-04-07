#pragma once
#include <map>
#include <tuple>
#include <list>
#include <vector>
#include <fstream>
#include "Modele3D.h"
#include "Porte.h"
#include "Salle.h"
#include "Info.h"
#include "Graphe.h"
#include "GestionnaireChemin.h"
#include "LecteurFichier.h"

typedef std::tuple<unsigned int, unsigned int, bool> Entree;
typedef std::tuple<unsigned int, unsigned int> Sortie;
typedef std::tuple<char*, char*, char*> Modele_Text;

class Carte{
private:
	graphe::Graphe carte;
	std::map<Entree, Sortie> liens;
	std::list<InfoSalle> infosSalles;
	Salle *salleActive;
	std::vector<Modele_Text> cheminsModeleText;
	
	void ajouterLien(Entree entree, Sortie sortie){
		liens[entree] = sortie;
	}
public:
	
	std::tuple<unsigned int, unsigned int> destination(std::tuple<unsigned int, unsigned int, bool> sortie){
		return liens[sortie];
	}
	
	void creer(const unsigned int limite){
		carte.creer(limite);
		int itterateurPorte(0);
		
		int* porte = new int[limite];
		Entree entree;
		Sortie sortie;
		for (unsigned int i = 0; i < limite; ++i)
			porte[i] = 0;
		
		for (unsigned int i = 0; i < limite; ++i){
			for (unsigned int j = 0; j < limite; ++j){
				if (carte.matrice[i * limite + j]){
					entree = std::make_tuple(i, ++itterateurPorte, false);
					sortie = std::make_tuple(j, porte[j]);
					++porte[j];
					ajouterLien(entree, sortie);
					
				}
			}
		}
		
		std::ifstream fichierSalle("salle_text.txt");
		std::ifstream fichierObjet("objet_text.txt");
		
		char* curseur1, *curseur2, *curseur3;
		int itterateur(1);
		while (!fichierSalle.eof()) {
			fichierSalle >> curseur1; fichierSalle >> curseur2; fichierSalle >> curseur3;
			cheminsModeleText.push_back(std::make_tuple(curseur1, curseur2, curseur3));
			++itterateur;
		}
		
		InfoSalle salle;
		int aleatoire;
		
		
		for (unsigned int i = 0; i < limite; ++i){
			salle.ID = i;
			salle.nbrPorte = carte.degreSortant(i);
			aleatoire = rand() % itterateur;
			salle.cheminModele = std::get<0>(cheminsModeleText[aleatoire]);
			salle.cheminTexture = std::get<1>(cheminsModeleText[aleatoire]);
			LecteurFichier::lireBoite(std::get<2>(cheminsModeleText[aleatoire]), salle);
			infosSalles.push_back(salle);
		}
		
		
	}
};