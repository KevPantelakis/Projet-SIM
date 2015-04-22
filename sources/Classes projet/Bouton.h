#pragma once 
#include "Texte2D.h"
#include "GestionnaireEvenements.h"
#include "GestionnaireRessources.h"
#include "Gestionnaire2D.h"

#include <SDL2/SDL.h>
#include <functional>

class Bouton{ 
private:
	enum Etat{ EN_CLIC, SURVOL, DEFAUT };

	Etat etat;
	gfx::Texte2D* texte;
	std::function<void(Bouton*)> clicRappel;
	std::function<void(Bouton*)> survolRappel;

public:
	Bouton() : Bouton(nullptr, nullptr, Vecteur2f(), "123", NULL){}

	Bouton(std::function<void(Bouton*)> fonctionClic, std::function<void(Bouton*)> fonctionSurvol, Vecteur2f &position, const char* texte, int t){
		etat = DEFAUT;
		clicRappel = fonctionClic;
		survolRappel = fonctionSurvol;
		this->texte = new gfx::Texte2D(texte, gfx::GestionnaireRessources::obtInstance().obtPolice("arial.ttf", "arial20", 20), Vecteur2f(0, 0));
		GestionnaireEvenements::obtInstance().ajouterUnRappel(SDL_MOUSEBUTTONDOWN, std::bind(&Bouton::gererClic, this, std::placeholders::_1));
		GestionnaireEvenements::obtInstance().ajouterUnRappel(SDL_MOUSEMOTION, std::bind(&Bouton::gererSurvol, this, std::placeholders::_1));
		gfx::Gestionnaire2D::obtInstance().ajouterObjet(this->texte);
	}

	~Bouton(){
		gfx::Gestionnaire2D::obtInstance().retObjet(texte);
		delete texte;
	}

	void gererClic(SDL_Event &event){
		if (texte->obtRectangle().contient(event.motion.x, event.motion.y)){
			clicRappel(this);
			etat = EN_CLIC;
		}
		else{
			etat = DEFAUT;
		}
	}
	
	void gererSurvol(SDL_Event &event){
		if (texte->obtRectangle().contient(event.motion.x, event.motion.y)){
			if(etat != SURVOL)
				survolRappel(this);
			etat = SURVOL;
		}
		else{
			etat = DEFAUT;
		}
	}

	void defTexte(const char* texte){
		this->texte->defTexte(texte);
	}

	void defPosition(Vecteur2f &position){
		texte->defPosition(position);
	}



	void defCouleur(SDL_Color &couleur){
		texte->defCouleur(couleur);
	}

	void defRappelClic(std::function<void(Bouton*)> fonction){
		this->clicRappel = fonction;
	}

	void defRappelSurvol(std::function<void(Bouton*)> fonction){
		this->survolRappel = fonction;
	}

};