#pragma once
#include "Modele3D.h"
class Salle{
protected:
	unsigned int ID;
	unsigned int nbrPortes;
	gfx::Modele3D modele;
public:
	Salle(){}

	Salle(gfx::Modele3D modele, unsigned int nbrPorte, int ID){
		nbrPortes = nbrPorte;
		this->ID = ID;
		this->modele = modele;
	}

	unsigned int obtNbrPortes(){
		return nbrPortes;
	}

	unsigned int obtID(){
		return ID;
	}

	void setID(unsigned int ID){
		this->ID = ID;
	}

	Vecteur3d obtPos(){
		return modele.obtPosition();
	}

	void defPosition(int axeX, int axeY, int axeZ){
		modele.defPosition(axeX, axeY, axeZ);
	}

	gfx::Modele3D obtModele(){
		return modele;
	}
};