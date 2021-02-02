/*-------------------------------------------------------------------------*
 | Copyright (C) 2018 Département Informatique de PolyTech Tours.          |
 |                                                                         |
 | This file is part of PolyShell, yet another shell.                      |
 |                                                                         |
 | PolyShell is free software; you can redistribute it and/or modify       |
 | it under the terms of the GNU General Public License as published by    |
 | the Free Software Foundation; either version 3 of the License,          |
 | or (at your option) any later version.                                  |
 |                                                                         |
 | PolyShell is distributed in the hope that it will be useful,            |
 | but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            |
 | GNU General Public License for more details.                            |
 |                                                                         |
 | You should have received a copy of the GNU General Public License       |
 | along with this program. If not, see <http://www.gnu.org/licenses/>.    |
 |                                                                         |
 | Additional permission under GNU GPL version 3 section 7 ---- If you     |
 | modify PolyShell, or any covered work, by linking or combining it with  |
 | libprovided (or a modified version of that library), containing parts   |
 | covered by the terms of the Creative Commons BY-NC-ND 4.0 International |
 | License, the licensors of  PolyShell grant you additional permission    |
 | to convey the resulting work.                                           |
 *-------------------------------------------------------------------------*/

#include "interactive/history.h"
#include "misc/all.h"
#include "interactive/autocomplete.h"
#include <stdio.h>

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(History, const char*, unsigned int)
MAKE_DEL_1(History, const char*)

int IMPLEMENT(History_init)(History *history, const char *filename, unsigned int length)
{
	if(Fifo_init(&history->storage,length, COMPOSE)) return 1;
	
	history->position=history->storage.tail;
	
	//Si fichier spécifié => on souhaite en charger le contenu dans l'historique
	if(filename){
		char *cheminAbs = prependHomeDir(duplicateString(filename));
		if(cheminAbs) {
			FILE *file = fopen(cheminAbs,"r");
			
			if(file){
				FileIterator it;
				if(FileIterator_init(&it, file)==0){
					
					//On parcours les lignes du fichier
					while(!FileIterator_isOver(&it)){
						//On insère la ligne dans l'historique
						History_add(history, FileIterator_get(&it));
						FileIterator_next(&it);
					}
					FileIterator_finalize(&it);
				}
				fclose(file);
			}
			free(cheminAbs);
		}
	}
	
	return 0;
	
    //return provided_History_init(history, filename, length);

}

void IMPLEMENT(History_finalize)(History *history, const char *filename)
{
	if(history){
		//Si un fichier est défini on souhaite y sauvegarder le contenu de l'historique
		if(filename){
			
			char *cheminAbs = prependHomeDir(duplicateString(filename));
			
			if(cheminAbs) {
				FILE *file = fopen(cheminAbs,"w+");
				if(file){
					char *cmd;
					
					//Parcours de l'historique des commandes dans l'ordre décroissant
					while((cmd=getProtString(Fifo_front(&history->storage),'#'))){
						fprintf(file, "%s\n", cmd);
						free(cmd);
						//On supprime la commande la plus ancienne (sinon boucle infinie)
						Fifo_pop(&history->storage);
					}
					free(cmd);
					fclose(file);
				}
				free(cheminAbs);
			}
		}
		//On libère l'espace mémoire alloué pour la file de l'historique
		Fifo_finalize(&history->storage);
	}
	
    //provided_History_finalize(history, filename);
}


void IMPLEMENT(History_clear)(History *history)
{
	Fifo_clear(&history->storage);
	
    //provided_History_clear(history);
}


void IMPLEMENT(History_add)(History *history, const char *cmd)
{
	if(history != NULL && cmd!=NULL && isNotEmpty(cmd)){
		//Si plein, on supprime la commande la plus ancienne
		if (Fifo_full(&history->storage)) {
            Fifo_pop(&history->storage);
        }
		
        Fifo_push(&history->storage, cmd);
        history->position = history->storage.tail; //On met à jour la position dans l'historique
	}
	
    //provided_History_add(history, cmd);
}


const char* IMPLEMENT(History_up)(History *history)
{
	
	if(		!&history->storage 							// Si l'historique est NULL 
		|| 	Fifo_empty(&history->storage)				// OU qu'il n'a pas de contenu
		|| 	history->position==history->storage.head	// OU qu'on est déjà au plus vieux
	) return NULL;										// 	=> On arrête tout
	
	//On met à jour la nouvelle position dans l'historique
	history->position = (history->position-1) % history->storage.capacity;

	return history->storage.storage[history->position];
	
    //return provided_History_up(history);
}


const char* IMPLEMENT(History_down)(History *history)
{
	if(		!&history->storage 							// Si l'historique n'a pas de contenu
		|| 	Fifo_empty(&history->storage)				// OU que le contenu est vide
		|| 	history->position==history->storage.tail	// OU qu'on est déjà au plus récent
	) return NULL;										// On arrête tout
	
	//On met à jour la nouvelle position dans l'historique
	history->position = (history->position+1) % history->storage.capacity;

	//Si on est arrivé sur la queue de fifo (char* initialisée mais sans contenu) => renvoie chaîne vide
	return history->position==history->storage.tail?"":history->storage.storage[history->position];
	
    //return provided_History_down(history);
}