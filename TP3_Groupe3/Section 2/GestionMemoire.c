#include<stdbool.h>
#include "./libs/lib.h"

unsigned int calculerNumeroDePage(unsigned long adresse) {
    return (adresse & 0xFFFFFC00) >> 10;
}

unsigned long calculerDeplacementDansLaPage(unsigned long adresse) {
     return adresse & 0x000003FF;
}

unsigned long calculerAdresseComplete(unsigned int numeroDePage, unsigned long deplacementDansLaPage) {
     return (numeroDePage << 10) | deplacementDansLaPage;
}

void rechercherTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned long numeroDePage=calculerNumeroDePage(req->adresseVirtuelle);
	unsigned long DeplacementDansLaPage=calculerDeplacementDansLaPage(req->adresseVirtuelle);
	for (int i = 0; i < TAILLE_TLB; i++) {
		if (mem->tlb->numeroPage[i]==numeroDePage){
			if (mem->tlb->entreeValide[i]){
				req->adressePhysique=calculerAdresseComplete(mem->tlb->numeroCadre[i],DeplacementDansLaPage);
				mem->tlb->dernierAcces[i]=req->date;
				}
				else{
					req->adressePhysique=0;
				}
			break;
		}
	}
		
}

void rechercherTableDesPages(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned long numeroDePage=calculerNumeroDePage(req->adresseVirtuelle);
	unsigned long DeplacementDansLaPage=calculerDeplacementDansLaPage(req->adresseVirtuelle);
		if (mem->tp->entreeValide[numeroDePage]){
		req->adressePhysique=calculerAdresseComplete(mem->tp->numeroCadre[numeroDePage],DeplacementDansLaPage);

	}else{
		req->adressePhysique=0;
	}

}

void ajouterDansMemoire(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned long numeroDePage=calculerNumeroDePage(req->adresseVirtuelle);
	unsigned long DeplacementDansLaPage=calculerDeplacementDansLaPage(req->adresseVirtuelle);
	for (int i = 0; i < TAILLE_MEMOIRE; i++) {
		if (mem->memoire->utilisee[i]==0){
			mem->memoire->numeroPage[i]=numeroDePage;
			mem->memoire->dernierAcces[i]=req->date;
			mem->memoire->dateCreation[i]=req->date;
			mem->memoire->utilisee[i]=1;
			req->adressePhysique=calculerAdresseComplete(i,DeplacementDansLaPage);
		}
	}
}

void mettreAJourTLB(struct RequeteMemoire* req, struct SystemeMemoire* mem) {
	unsigned long FirstInDate = mem->tlb->dateCreation[0];
	unsigned long FirstIn = 0;
for (int i=0; i< TAILLE_TLB;i++){
	if(mem->tlb->entreeValide[i]==0){
		FirstInDate=mem->tlb->dateCreation[i];
		FirstIn=i;
		break;
	}
	if(mem->tlb->dateCreation[i]<FirstInDate){
		FirstInDate=mem->tlb->dateCreation[i];
		FirstIn=i;
	}
}
mem->tlb->dateCreation[FirstIn]=req->date;
mem->tlb->dernierAcces[FirstIn]=req->date;
mem->tlb->entreeValide[FirstIn]=1;
mem->tlb->numeroCadre[FirstIn]=calculerNumeroDePage(req->adressePhysique);
mem->tlb->numeroPage[FirstIn]=calculerNumeroDePage(req->adresseVirtuelle);
}

// NE PAS MODIFIER
int main() {
    evaluate(
		&calculerNumeroDePage, 
		&calculerDeplacementDansLaPage, 
		&calculerAdresseComplete, 
        &rechercherTLB, 
		&rechercherTableDesPages,
		&mettreAJourTLB,
		&ajouterDansMemoire
    );
    return 0;
}
