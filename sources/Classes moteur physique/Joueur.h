#pragma once
#include "Vecteur3.h"
#include "Objet.h"

class Joueur : public Objet{
private:
	double hauteur;
	double largeur;
	gfx::Modele3D modele3D;
	bool accroupie;

public:
	Joueur(gfx::Modele3D &modele3D, unsigned int ID, double masse, Vecteur3d position, double largeur, double hauteur) : Objet(modele3D, ID, NULL, masse, NULL, position){
		this->hauteur = hauteur;
		this->largeur = largeur;
		this->modele3D = modele3D;
	}

	void appliquerPhysique(Salle &salle, double frametime) {
		if (!Physique::obtInstance().collisionJoueurSalle(*this, salle)) {
			Physique::obtInstance().appliquerGravite(vitesse);
		}
		defPosition(position + (vitesse * frametime));
	}

	double obtHauteur() {
		return hauteur;
	}

	double obtLargeur() {
		return largeur;
	}

	gfx::Modele3D obtModele3D() {
		return modele3D;
	}

	void accroupie() {
		hauteur = 1.00;
	}

	void debout() {
		hauteur = 1.80;
	}
};