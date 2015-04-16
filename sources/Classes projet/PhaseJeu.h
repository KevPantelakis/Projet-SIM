#pragma once
#include "PhaseMenuPause.h"

class PhaseJeu : public Phase{

private:

	Joueur* joueur;

	void appliquerPhysique(float frameTime) {

		if (joueur->obtVitesse().norme() != 0) {
			if (!Physique::obtInstance().collisionJoueurSalle(joueur)) {
				Physique::obtInstance().appliquerGravite(joueur->obtVitesse(), frameTime);
				joueur->defPosition(joueur->obtPosition() + joueur->obtVitesse() * frameTime);
			}
			else
				joueur->defSaut(false);
		}

		Physique::obtInstance().appliquerPhysiqueSurListeObjet(frameTime);

	}

public:

	PhaseJeu() : Phase(){
		joueur = new Joueur(new gfx::Modele3D(gfx::GestionnaireRessources::obtInstance().obtModele("Joueur.obj"), gfx::GestionnaireRessources::obtInstance().obtTexture("Joueur.png")), 0, 100, Vecteur3d(-1, 0, 0));
		joueur->ajouterScene();

		gfx::Gestionnaire3D::obtInstance().defFrustum(45, 800.0 / 600.0, 0.99, 1000);
		Carte::obtInstance().salleActive = new Salle(new gfx::Modele3D(gfx::GestionnaireRessources::obtInstance().obtModele("SalleCarree4x4.obj"), gfx::GestionnaireRessources::obtInstance().obtTexture("SalleCarree4x4.png")), 2, 0);
	}

	void rafraichir(float frameTime) {

		if (!this->pause) {
		
			joueur->deplacement(frameTime);
			appliquerPhysique(frameTime);
			//ControlleurAudio::obtInstance().jouerTout(joueur);
		}

		if (Clavier::toucheAppuyee(SDLK_ESCAPE)) {
			pause = true;
			GestionnairePhases::obtInstance().ajouterPhase(new PhaseMenuPause());
			SDL_SetRelativeMouseMode(SDL_FALSE);
			SDL_ShowCursor(SDL_ENABLE);
			gfx::Gestionnaire3D::obtInstance().obtCamera()->defPause(true);
		}
	}
};