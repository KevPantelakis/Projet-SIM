#pragma once
#include <stack>
#include <list>
#include <math.h>
#include "PhaseMenuPause.h"

class PhaseJeu : public Phase{

private:

	Joueur* joueur;
	gfx::Texte2D* texte;
	Objet* objetVise;
	bool toucheRelachee;
	std::stack<unsigned int> cheminRecursif;
	std::list<unsigned int> cheminLogique;
	double iterateur_x, iterateur_z;
	int sensPrecedent, sensActuel;
	
	double exponentielle(double a, double b, double h, double k, double x, int limite){
		double temp = a * pow(M_E, b * (x - h)) + k;
		if (temp < limite){
			return temp;
		}
		return limite;
	}
	
	void santeMentale(){
		double pourcentagePerdu(0);
		if (cheminRecursif.size() > 1){
			pourcentagePerdu += exponentielle(1, 0.2f, 0, -1, cheminRecursif.size(), 5);
		}
		int idActif = cheminLogique.front();
		std::list<unsigned int>::iterator it = cheminLogique.begin();
		for (++it; it != cheminLogique.end(); ++it){
			if (((*it) == idActif) && ((iterateur_x > 2.5f) || (iterateur_z > 2.5))){
				pourcentagePerdu += exponentielle(1, -0.5f, -3.8f, -1, cheminLogique.size(), 5);
				cheminLogique.erase(cheminLogique.begin(), --it);
				iterateur_x = 0;
				iterateur_z = 0;
				break;
			}
		}
		joueur->defSanteMentale((double)joueur->obtSanteMentale() * (pourcentagePerdu / 100.f));
	}
	
	
	void appliquerPhysique(float frameTime) {
		if (joueur->obtVitesse().norme() != 0) {
			if (!Physique::obtInstance().collisionJoueurSalle(Carte::obtInstance().salleActive, joueur)) {
				if (joueur->obtEtat() != STABLE)
					Physique::obtInstance().appliquerGravite(joueur->obtVitesse(), frameTime);
				joueur->defPosition(joueur->obtPosition() + joueur->obtVitesse() * frameTime);
				iterateur_x += joueur->obtVitesse().x * frameTime;
				iterateur_z += joueur->obtVitesse().z * frameTime;
			}
			else{
				if (joueur->obtEtat() != CHUTE)
					joueur->defEtat(CHUTE);
				else if (joueur->obtEtat() == CHUTE){
					if (Physique::obtInstance().collisionAuSol(Carte::obtInstance().salleActive, joueur)){
						joueur->defEtat(STABLE);
						joueur->obtVitesse().y = 0.f;
					}
					joueur->obtVitesse().x = 0.f;
					joueur->obtVitesse().z = 0.f;
				}
			}
		}
		Physique::obtInstance().appliquerPhysiqueSurListeObjet(Carte::obtInstance().salleActive, frameTime);
	}

	bool detectionObjet() {

		std::list<Objet*> liste = Carte::obtInstance().salleActive->obtListeObjet();
		bool objetDetecte = false;

		for (auto it : liste) {
			if (Physique::obtInstance().distanceEntreDeuxPoints(joueur->obtPosition(), it->obtPosition()) < 2) {
				texte->defTexte(new std::string("Press E to open the door"));
				gfx::Gestionnaire2D::obtInstance().ajouterObjet(texte);
				objetDetecte = true;
				objetVise = it;
				return objetDetecte;
			}
		}

		if (!objetDetecte)
			gfx::Gestionnaire2D::obtInstance().retObjet(texte);
		return objetDetecte;
	}

public:

	PhaseJeu() : Phase(){
		joueur = new Joueur(Vecteur3d(-1, 0, 0));
		joueur->ajouterScene();
		texte = new gfx::Texte2D(new std::string("123"), { 0, 0, 0, 255 }, gfx::GestionnaireRessources::obtInstance().obtPolice("arial.ttf", 20), Vecteur2f(300, 200));
		toucheRelachee = false;
		
		//Carte::obtInstance().salleActive = new Salle(new gfx::Modele3D(gfx::GestionnaireRessources::obtInstance().obtModele("SalleCarree4x4.obj"), gfx::GestionnaireRessources::obtInstance().obtTexture("SalleCarree4x4.png")), 2, 0);
		
		Carte::obtInstance().creer(20);
		cheminRecursif.push(Carte::obtInstance().salleActive->obtID());
		cheminLogique.push_back(Carte::obtInstance().salleActive->obtID());
		iterateur_x = 0;
		iterateur_z = 0;
	}

	void rafraichir(float frameTime) {

		if (!this->pause) {

		
			joueur->deplacement(frameTime);
			appliquerPhysique(frameTime);
			detectionObjet();
			//ControlleurAudio::obtInstance().jouerTout(joueur);
			Carte::obtInstance().bougerMur(*joueur, frameTime);
		}

		if (Clavier::toucheAppuyee(SDLK_ESCAPE)) {
			defPause(true);
			SDL_SetRelativeMouseMode(SDL_FALSE);
			SDL_ShowCursor(SDL_ENABLE);
			gfx::Gestionnaire3D::obtInstance().obtCamera()->bloquer();
			GestionnairePhases::obtInstance().defPhaseActive(MENUPAUSE);
			GestionnairePhases::obtInstance().obtPhaseActive()->defPause(false);
			GestionnairePhases::obtInstance().obtPhaseActive()->remplir();
			joueur->deplacement(frameTime);
			appliquerPhysique(frameTime);
			ControlleurAudio::obtInstance().jouer(COEUR, joueur);
			ControlleurAudio::obtInstance().jouer(PAS, joueur);
			detectionObjet();
			ControlleurAudio::obtInstance().jouerTout(joueur);
		}


		if (detectionObjet()){
			if (Clavier::toucheRelachee(SDLK_e) && toucheRelachee){// Touche relach�e bient�t...
				if (objetVise->obtSiPorte()){
					Carte::obtInstance().destination(std::make_tuple(Carte::obtInstance().salleActive->obtID(), objetVise->obtID(), false), *joueur);
					if (Carte::obtInstance().salleActive->obtID() != cheminRecursif.top()){
						for (int i = 0; i < cheminRecursif.size(); ++i)
							cheminRecursif.pop();
					}
					cheminRecursif.push(Carte::obtInstance().salleActive->obtID());
					cheminLogique.push_front(Carte::obtInstance().salleActive->obtID());
					santeMentale();
				}
				else{
					objetVise->appliquerAction(Interagir);
				}
				toucheRelachee = false;
			}
			if (Clavier::toucheAppuyee(SDLK_e))
				toucheRelachee = true;
		}
	}

	void remplir() {

	}

	void defPause(bool pause) {
		this->pause = pause;
	}
};

