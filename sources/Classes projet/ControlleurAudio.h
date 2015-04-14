#pragma once
#include <map>
#include "Sons.h"
#include "Joueur.h"

class ControlleurAudio : public Singleton<ControlleurAudio>{
private:
	std::map<int, Sons*> sons;
	Coeur *coeur;
	Pas *pas;
	int ittChaine;
	bool initial;
public:
	
	ControlleurAudio(){}
	
	~ControlleurAudio(){}
	
	void initialiser(int chaines){
		Mix_AllocateChannels(chaines);
		ittChaine = 0;
		std::ifstream fichierNom("sons.txt");
		std::string chemin;
		if (fichierNom.is_open()) {
			while (!fichierNom.eof()) {
				fichierNom >> chemin;
				if (chemin == "fond.ogg"){
					sons[ittChaine] = new Fond(chemin.c_str(), ++ittChaine);
				}
				sons[ittChaine] = new Sons(chemin.c_str(), ++ittChaine);
			}
			fichierNom.close();
		}
		coeur = new Coeur("battementLent.ogg", "battementRapide.ogg", ++ittChaine);
		pas = new Pas("pas1.ogg", "pas2.ogg", ++ittChaine);
		initial = false;
	}
	
	void fermer(){
		for (int i = 0; i < ittChaine; i++) delete sons[i];
		delete coeur;
		delete pas;
	}
	
	void jouerTout(Joueur* joueur){
		for (auto it : sons){
			if (it.second->actif)
				it.second->jouer(joueur);
		}
		coeur->jouer(joueur);
		pas->jouer(joueur);
		if (!initial){
			defVolumeFond(20);
			defVolumeEffet(60);
			coeur->defVolume(10);
			pas->defVolume(8);
			initial = true;
		}
	}
	
	void defVolumeFond(int vol){
		Mix_Volume(17, vol);
	}
	
	void defVolumeEffet(int vol){
		for (auto it : sons){
			if (it.second->obtChaine() != 17){
				Mix_Volume(it.second->obtChaine(), vol);
			}
		}
	}
	
	void defVolumePrinc(int vol){
		Mix_Volume(-1, vol);
	}
	
	void defVolumeSon(int vol, int chaine){ // chaine = -1 pour tout
		for (auto it : sons){
			if ((chaine = -1) || it.second->obtChaine() == chaine){
				it.second->defVolume(vol);
			}
		}
	}
	
	Coeur* obtCoeur(){
		return coeur;
	}
};