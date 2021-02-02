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

#include "interactive/autocomplete.h"
#include "misc/all.h"
#include "system/info.h"
#include <stdio.h>

// #########################################################################
// #########################################################################
// #########################################################################

char* IMPLEMENT(prependHomeDir)(char *str)
{
	const char *homeDir;
	
	//On arrête si pb avec les variables nécessaires ou ne commence pas par '~'
	if(		!str 									//Str est NULL
		||	userInformation(NULL,&homeDir,NULL) !=0 //Problème lors de la récupération des infos
		||	!homeDir 								//Chemin du répertoire de l'utilisateur est NULL
		||	!startWith(str,"~",0) 					//Ne commence pas par '~'
	) return str;

	char *realStr;
	char *position;
	
	
	//On récuppère la chaîne sans échappement
	realStr = getRealString(str,'~',&position);
	
	
	//Entrée : chaîne sans échappement non nulle
	//		ET la chaîne contient un slash OU elle ne fait que 1 de long
	// (on n'entre que dans les cas suivant : "~", "~/xxx", "~~/xxx")
	if( realStr && (belongs('/',realStr) || stringLength(realStr)==1)) {
		
		//Entrée : le caractère '~' n'a pas été échappé
		//	=> on ajoute le chemin vers le répertoire de l'utilisateur au début du chemin initial
		if( position && stringCompare(str,realStr)==0 ) {
			
			char *tmp = duplicateString(startWith(realStr,"~",0));
			size_t dirLength = stringLength(homeDir), realLength = stringLength(tmp);
			
			free(realStr);
			
			//On concatène le chemin vers le répertoire de l'utilisateur avec le chemin initial
			realStr = concatenateStrings(homeDir,tmp,dirLength+realLength+1);
			free(tmp);
		}
		
		free(str);
		str = realStr;
		
	} else free(realStr);
	
	
	return str;
	
	//return provided_prependHomeDir(str);
}

int IMPLEMENT(autocomplete)(const char *prefix, unsigned int limit, unsigned int *nbItems, char **extend, Fifo **results)
{
	
	
	if(!prefix || !limit || !nbItems || !extend || !results) return 1;
	
	
	const char *path = getPATHVar();
	
	
	//On ouvre le dossier courrant
	//const char *currentDir = getCurrentDirectory(0);
	char *dossier = duplicateString(getCurrentDirectory(0));
	char *tmp;
	char *usablePrefix = getRealString(prefix,'*',&tmp);	
	
	
	FolderIterator it;
	Tokenizer tkPath;
	Pattern preg;
	
	int tkInit = Tokenizer_init(&tkPath, path, ":");
	int pregInit = Pattern_init(&preg,"^(~|[.]{1,2})$");
	
	
	//Entrée : Il y a eu une erreur du l'un des init
	//		OU le prefix contient un caractère '*' non echappé
	if(tmp || tkInit || pregInit){
		
		if(!tkInit) Tokenizer_finalize(&tkPath);
		if(!pregInit) Pattern_finalize(&preg);
		
		free(dossier);
		free(usablePrefix);
		return 1;
	}
	free(tmp);
	
	int specialChar = Pattern_match(&preg, usablePrefix);
	Pattern_finalize(&preg);
	
	
	int nbPassages = 0, stop = 0;
	int usePath = stringLength(path)>1?1:0;

	*results = Fifo_new(limit,COMPOSE);
	*extend = NULL;
	*nbItems = 0;
	
	
	while (!stop) {
			
		//On cherche dans le dossier acuel ou dans le chemin spécifié
		if(nbPassages==0) {
			//currentDir=0;
			
			//La chaîne est un chemin vers un dossier/fichier
			if(belongs('/',usablePrefix)){
				
				// Entrée : Chemin incomplet
				//	=> on récupère le dossier où doit chercher (avant dernier '/')
				//	=> on change le préfixe a chercher
				if(usablePrefix[stringLength(usablePrefix)-1]!='/'){
			
					char *lastItem = findLast(usablePrefix,'/');//, *tmp;
					
					size_t lastItemLen 		= stringLength(&lastItem[1]);
					size_t usablePrefixLen 	= stringLength(usablePrefix);
					size_t newDossier		= usablePrefixLen-lastItemLen;
					
					dossier = realloc(dossier,newDossier);
					if(!dossier) return 1;
					copyStringWithLength(dossier, usablePrefix, newDossier);
					
					char *tmp = duplicateString(&lastItem[1]);
					free(usablePrefix);
					usablePrefix = tmp;
					
				}
				//Entrée : Chemin complet vers un dossier
				else {
					
					free(dossier);
					dossier = duplicateString(usablePrefix);
					free(usablePrefix);
					usablePrefix=duplicateString(""); //Sans duplicateString() => erreur ligne 223
				
				}
				//On ne cherche pas dans PATH puisqu'il s'agit d'un chemin
				usePath=0;
			}
		}
		//On cherche dans PATH si pas trouvé dans dossier actuel
		else if(usePath && !Tokenizer_isOver(&tkPath)){
			
			free(dossier);
			dossier = Tokenizer_get(&tkPath);
			
		} else stop = 1;
		
		
		if(!stop){
			//Erreur si l'initialisation du FolderIterator echoue
			if(FolderIterator_init(&it, dossier,1)){
				free(usablePrefix);
				free(dossier);
				return 1;
			}
			
			//Parcours des éléments du dossier courrant
			while(!FolderIterator_isOver(&it)){
				
				char *current = duplicateString(FolderIterator_get(&it));
				
				//On entre si le préfixe n'est pas '~' OU '.' OU '..'
				if( !specialChar ){
					
					//Si l'objet pointé est un dossier : on ajoute un '/' à la fin
					if(FolderIterator_isDir(&it)) {
						
						char *tmp = concatenateStrings(current,"/",0);
						free(current);
						current=duplicateString(tmp);
						free(tmp);
						
					}
					
					const char *suffix = startWith(current, usablePrefix, 0);
					
					//Entrée : le nom de l'objet pointé commence par le préfixe passé en paramètre
					if(suffix){
						
						++*nbItems;
						
						//On enregistre tous les limits 1ers resultats dans *results
						if(!Fifo_full(*results)) Fifo_push(*results, current);
						
						// si 	 prefix  = "Doc"
						// et 	 current = "Documents"
						// alors suffix  = 	   ^
						if(!*extend) *extend = duplicateString(suffix); //Premier fichier ou dossier qui commence par prefix
						else {
							mkCommon(*extend,suffix);
						}
						
					}
				}
				
				FolderIterator_next(&it);
				free(current);
			}
			
			//Si le prefixe ne contient que '.' ou '..' ou '~'
			//	=> on arrête le programme et on complete ext
			if(specialChar){
				++*nbItems;
				free(*extend);
				*extend = duplicateString("/");
				stop = 1;
			}
			
			
			
			//Entrée : on est à la 2e itération (début parcours PATH)
			//		OU on ne souhaite pas chercher dans PATH
			//	=> on avance dans le parcours du Tokeniser de PATH
			if(nbPassages > 0 || !usePath){
				
				
				
				//Entrée : on ne souhaite pas chercher dans PATH
				//		OU le dossier courrant est la variable PATH (cas où PATH ne contient pas de char ':')
				if(		!usePath
					||	(		nbPassages == 1 //Première itération de PATH (évite de comparer tout le temps)
							&& 	stringCompare(dossier,path)==0
						)
				){
					stop = 1;
				} else {
					Tokenizer_next(&tkPath);
				}
			}
			
			++nbPassages;
			FolderIterator_finalize(&it);
		}
		
	}
	
	free(usablePrefix);
	free(dossier);
	
	switch(*nbItems){
		//Si pas de resultat on supprime la FIFO
		case 0:
			Fifo_delete(*results);
			*results=NULL;
		break;
		
		//Si 1 seul résulat on supprime la FIFO
		//On renvoie le seul resultat trouvé
        case 1:
			Fifo_delete(*results);
			*results=NULL;
		break;
		
		//Si plusieurs résultats trouvé on libère extend
        default:
			if(stringLength(*extend)>1){
				Fifo_delete(*results);
				*results=NULL;
			} else {
				free(*extend);
				*extend=NULL;
			}
		break;
                
    }
	
	//printf("%d\t%s\t%s\n",*nbItems, *extend?*extend:"NULL\t", *results?"NON NULL":"NULL");
	
	return 0;
	
    //return provided_autocomplete(prefix, limit, nbItems, extend, results);
}
