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

#include "system/command.h"
#include "misc/string.h"
#include "interactive/autocomplete.h"
#include "misc/ferror.h"
#include <errno.h>
#include <wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "misc/all.h"
#include "system/all.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_DEL_0(CmdMember)

CmdMember* IMPLEMENT(CmdMember_new)(const char *base)
{
    // MAKE_NEW is not sufficient here because new variables must
    // be registered using the CmdMember_addLivingCmdMember function.
	
	CmdMember *cmdptr = (CmdMember*)malloc(sizeof(CmdMember));
	if(cmdptr){
		if(CmdMember_init(cmdptr, base)){
			free(cmdptr);
			cmdptr = NULL;
		} else if(CmdMember_addLivingCmdMember(cmdptr)){
			CmdMember_delete(cmdptr);
			cmdptr = NULL;
		}
	}
	return cmdptr;
	
    //return provided_CmdMember_new(base);
}

int IMPLEMENT(CmdMember_init)(CmdMember *mbr, const char *base)
{
	mbr->status = 1;
	
	mbr->base = duplicateString(base);
	if(!mbr->base) return 1;
	
	mbr->options = NULL;
	mbr->nbOptions = 0;
	mbr->capacityOption = 0;
	
	//Redirections
	for(int fd=0; fd<3; ++fd){
		mbr->redirections[fd] = NULL;
		mbr->redirectionTypes[fd] = UNDEFINED;
	}
	
	//Chaînage
	mbr->next = NULL;
	mbr->prev = NULL;
	
	//Respecter la convention C
	CmdMember_addOption(mbr, base, 0);
	
	if(!mbr->status){
		CmdMember_finalize(mbr);
		return 1;
	}
	
	return 0;
	
    //return provided_CmdMember_init(mbr, base);
}

void IMPLEMENT(CmdMember_finalize)(CmdMember *mbr)
{
	
	free(mbr->base);
	
	for(unsigned int i=0; i < mbr->nbOptions; ++i){
		free(mbr->options[i]);
	}
	
	free(mbr->options);
	
	for(int fd=0; fd<3; ++fd){
		free(mbr->redirections[fd]);
	}
	
    //provided_CmdMember_finalize(mbr);
}

CmdMember* IMPLEMENT(CmdMember_redirect)(CmdMember *mbr, int fd, const char *filename)
{
	if(fd<0 || fd>2 || !mbr || !filename){
		fatalError("CmdMember_redirect: invalid arguments");
	}
	
	if(mbr->status){
		char * tmp = prependHomeDir(duplicateString(filename));
		if(!tmp) {
			perror("CmdMember_redirect: duplicateString or prependHomeDir has failed");
			mbr->status = 0;
		} else {
			if(mbr->redirections[fd]) free(mbr->redirections[fd]);
			mbr->redirections[fd] = tmp;
			mbr->redirectionTypes[fd] = NORMAL;
		}
	}

	return mbr;
	//return provided_CmdMember_redirect(mbr, fd, filename);
}

CmdMember* IMPLEMENT(CmdMember_appendRedirect)(CmdMember *mbr, int fd, const char *filename)
{
	if(fd<0 || fd>2 || !mbr || !filename){
		fatalError("CmdMember_redirect: invalid arguments");
	}
	
	if(mbr->status){
		char * tmp = prependHomeDir(duplicateString(filename));
		if(!tmp) {
			perror("CmdMember_redirect: duplicateString or prependHomeDir has failed");
			mbr->status = 0;
		} else {
			if(mbr->redirections[fd]) free(mbr->redirections[fd]);
			mbr->redirections[fd] = tmp;
			mbr->redirectionTypes[fd] = APPEND;
		}
	}

	return mbr;
	
    //return provided_CmdMember_appendRedirect(mbr, fd, filename);
}

CmdMember* IMPLEMENT(CmdMember_mergeOutputs)(CmdMember *mbr)
{
	if(mbr->status) mbr->redirectionTypes[2] = FUSION;
	return mbr;
    //return provided_CmdMember_mergeOutputs(mbr);
}

CmdMember* IMPLEMENT(CmdMember_pipe)(CmdMember *m1, CmdMember *m2)
{
	//Entrée : l'une des commande est dans un état non cohérent 
	//		OU on ne peut chaîner les commandes sans défaire un lien existant
	if(!m1->status || !m2->status || m1->next || m2->prev){
		m1->status = 0;
		m2->status = 0;
	} else {
		m1->next = m2;
		m2->prev = m1;
	}
	
	return m2;
	
    //return provided_CmdMember_pipe(m1, m2);
}

/**
 * @brief Fonction récursive qui, a partir d'un chemin, incrémente un compteur et une liste lorsqu'un fichier correspond à l'expression régulière
 * @author Guillaume ELAMBERT
 * @param pathToSearch : chemin vers le dossier où on souhaite faire la recherche
 * @param regex : chaîne de caractère correspondant à une expression régulière
 * @param nbItems : nombre de fichiers correspondant à l'expression régulière
 * @param results : la file qui contiendra les chemins (relatifs au dossier) vers les fichiers dont le nom correspond à l'expression régulière
 * @return Retourne un entier indiquant l'état de son execution : 1 en cas d'erreur, 0 sinon
 */
int findWithRegex(const char *pathToSearch, const char *regex, unsigned *nbItems, Fifo *results){
	
	if(!pathToSearch || !regex || !nbItems || !results) return 1;
	
	FolderIterator fIt;
	if(FolderIterator_init(&fIt, pathToSearch, 1)) return 1;
	
	//cette variable sert à indiquer si on doit continuer les appels récursifs
	//	=> variable =1 si il y a un '/' (sous-dossier) dans la regex 0 sinon
	int continueRecursiveSearch = 0;
	
	char *copyRegex = duplicateString(findFirst(regex,"/"));
	char *nextSubFolder=NULL;
	
	//Entrée : la regex contient un caractère '/' (sous dossier) et on pointe dessus
	//	=> on met en place la nouvelle regex, celle qui s'appliquera au sous dossier
	if(copyRegex){
		size_t copyRegexLen = stringLength(copyRegex);
		char *tmp;
		
		continueRecursiveSearch=1;
		
		
		//Entrée : On est pas en bout de chaîne (= on a spécifié ce que l'on cherche dans le sous dossier)
		//	=> on supprime le caractère '/' au début de la nouvelle regex
		if(*copyRegex+1!='\0'){
			tmp = malloc(sizeof(char)*copyRegexLen);
			if(!tmp) return 1;
			copyStringWithLength(tmp,copyRegex+1,copyRegexLen);
			free(copyRegex);
			copyRegex=duplicateString(tmp);
			--copyRegexLen;
			free(tmp);
		}
		//Entrée : on recherche tout dans le sous dossier
		else {
			free(copyRegex);
			copyRegex = ".*";
			copyRegexLen = 2;
		}
		
		
		size_t lengthToCopy = stringLength(regex)-copyRegexLen;
		tmp=malloc(sizeof(char)*lengthToCopy);
		if(!tmp) return 1;
		copyStringWithLength(tmp,regex,lengthToCopy);
		
		//Regex pour le sous dossier
		nextSubFolder = duplicateString(copyRegex);
		
		free(copyRegex);
		
		//Regex pour le dossier actuel
		copyRegex = tmp;
		
	} else {
		copyRegex = duplicateString(regex);
	}
	
	//On ajoute le caractère '^' car sinon on prend aussi des résultats non voulus
	//Exemple : On souhaite 's.*' => on récupère 'superExemple.txt' mais aussi 'pasSuperExemple.txt'
	char *realRegex = concatenateStrings("^", copyRegex, stringLength(copyRegex)+2);
	
	Pattern preg;
	
	if(Pattern_init(&preg,realRegex)){
		free(realRegex);
		return 1;
	}
	free(realRegex);
	
	char *fileName;
	const char* currentDir = getCurrentDirectory(0);
	size_t currentDirLen = stringLength(currentDir);
	
	//On ajoute un '/' à la fin du dossier où on fait la recherche
	size_t pathToSearchLen = stringLength(pathToSearch)+2;
	char *realPathToSearch = concatenateStrings(pathToSearch,"/", pathToSearchLen);
	
	while(!FolderIterator_isOver(&fIt)){
		
		fileName = duplicateString(FolderIterator_get(&fIt));
		
		//Le fichier ou dossier correspond à l'expression régulière
		if(Pattern_match(&preg,fileName)){
			
			size_t fileNameLen = stringLength(fileName);
			size_t totalLen = pathToSearchLen+fileNameLen;
			
			
			
			
			//Si il existe un '/' (on cherche encore dans un sous-dossier) ET que l'object est un dossier
			//On lance la recherche dans ce dernier
			if(continueRecursiveSearch && FolderIterator_isDir(&fIt)){
					
				//On ajoute un '/' entre le nom du dossier parent et le nom de l'élément fils
				char *newDir = concatenateStrings(realPathToSearch,fileName,pathToSearchLen+fileNameLen+2);
				
				findWithRegex(newDir, nextSubFolder, nbItems, results);
				free(nextSubFolder);
				free(newDir);
				
			//Sinon c'est qu'on à trouvé
			//	=> on ajoute à la file + incrémentation nombre d'éléments trouvés
			} else {
				
				++*nbItems;
				
				if(!Fifo_full(results)){
					
					char *result = concatenateStrings(realPathToSearch, fileName, totalLen + 1 );
					
					//Si le dossier parcouru contient le chemin vers le dossier courrant
					//	=> on modifie la chaîne pour en faire un chemin relatif
					if(startWith(result, currentDir,1) && stringLength(result) > currentDirLen+1){
						char *tmp = malloc(sizeof(char)*(totalLen-currentDirLen+1));
						if(!tmp) return 1;
						copyStringWithLength(tmp,&result[currentDirLen]+1,totalLen-currentDirLen);
						free(result);
						result=tmp;
					}
					
					Fifo_push(results,result);
					free(result);
				}
				
			}
			
		}
		
		FolderIterator_next(&fIt);
		free(fileName);
	}
	
	free(copyRegex);
	free(realPathToSearch);
	FolderIterator_finalize(&fIt);
	Pattern_finalize(&preg);
	
	return 0;
}



CmdMember* IMPLEMENT(CmdMember_addOption)(CmdMember *mbr, const char *option, int expend)
{
	if(!mbr->status) return mbr;
	
	char *args = NULL;
	unsigned nbOptions= mbr->nbOptions;
	int expendFinded = 0;
	
	Fifo results;
	if(Fifo_init(&results, 20, COMPOSE)) return mbr;
	
	
	
	if(option){
		
		args=duplicateString(option);
		
		//Entrée : Interpréter les astérisques comme regex
		if(expend){
			
			//Entrée : la chaîne contient au moins un '*'
			if(belongs('*',args)){
				
				
				//On échappe les caractère '.' pour ensuite remplacer l'echappement par un '\'
				char *newArgs = getProtString(args,'.');
				
				char *tmp = duplicateString(newArgs);
				free(newArgs);
				
				
				//On échappe les caractère '*' pour ensuite remplacer l'echappement par un point
				newArgs = getProtString(tmp,'*');
				free(tmp);
				
				
				size_t newArgsLen = stringLength(newArgs);
				
				//Initialisation de la position sur le 1er point
				char *nextPt = duplicateString(findFirst(newArgs,"."));
				size_t posPt = newArgsLen - stringLength(nextPt);
				
				
				//On remplace chaque première occurence de '.' par un '\'
				while(nextPt){
					
					/*-------- ON GERE LES POINTS ---------*/
					newArgs[posPt] = '\\';
					free(nextPt);
					
					//On vérifie si on est à la fin de la chaîne
					//Si non => on cherche la prochaine occurence de '*'
					nextPt = posPt+2 < newArgsLen ? duplicateString(findFirst(&newArgs[posPt+2],".")):NULL;
					posPt = newArgsLen - stringLength(nextPt);
					
					/*-------------------------------------*/
					
				}
				
				
				//Initialisation de la position sur la 1ère astérisque
				char *nextAst = duplicateString(findFirst(newArgs,"*"));
				size_t posAst = newArgsLen - stringLength(nextAst);
				
				
				//On remplace chaque première occurence de '*' par un point
				while(nextPt || nextAst){					
					
					/*------ ON GERE LES ASTERISQUES ------*/
					newArgs[posAst] = '.';
					free(nextAst);
					
					//On vérifie si on est à la fin de la chaîne
					//Si non => on cherche la prochaine occurence de '*'
					nextAst = posAst+2 < newArgsLen ? duplicateString(findFirst(&newArgs[posAst+2],"*")):NULL;
					posAst = newArgsLen - stringLength(nextAst);
					
					/*-------------------------------------*/
					
				}
				
				free(args);
				args=newArgs;
				
				//On appelle la fonction findWithRegex qui cherche récursivement les fichiers
				//dont le nom correspond à l'expression régulière
				if(findWithRegex(getCurrentDirectory(0), args, &nbOptions, &results)) return mbr;
				
				expendFinded = nbOptions > mbr->nbOptions;
				
				//Si on a pas trouvé on insère NULL
				if(args && !expendFinded){
					free(args);
					args=NULL;
				}
				
			}
			
		}
	}
	if(!expendFinded) ++nbOptions;
	
	
	char **optionsCpy = malloc(sizeof(char *)*nbOptions);
	if(!optionsCpy) return NULL;
	unsigned int i=0;
	
	
	//Copie de l'ancien tableau d'options + ajout nouvelles options
	while(i < nbOptions){
		
		//Copie de l'ancien tableau
		if(i< mbr->nbOptions){
			optionsCpy[i] = mbr->options[i];
		} else {
			//Copie de la/les nouvelle(s) option(s)
			if(!expendFinded){
				
				//Si on a pas fait de rechercher avec la regex ou qu'on a pas eu de résultat
				//On ajoute l'option à la fin du nouveau tableau
				optionsCpy[mbr->nbOptions] = duplicateString(args); 
			} else {
				
				optionsCpy[i] = duplicateString(Fifo_front(&results));
				Fifo_pop(&results);
			}
		}
		
		++i;
	}
	
	mbr->capacityOption = nbOptions;
	mbr->nbOptions = nbOptions;
	
	free(mbr->options);
	mbr->options = optionsCpy;
	
	Fifo_finalize(&results);
	if(args)free(args);
	
	return mbr;
    
	//return provided_CmdMember_addOption(mbr, option, expend);
}

size_t IMPLEMENT(Command_getNbMember)(const Command *cmd)
{
    return provided_Command_getNbMember(cmd);
}


static void __make_redirect__ (const char *file, int flags, int fd){
	
	//On ouvre le fichier  = récupère descripteur de fichier
	const int fdFichier = open(file,flags, 0644);
	
	if(fdFichier == -1 ){
		perror("Command_execute: dup2 has failed");
		exit(1);
	}
	
	//On duplique ce descripteur et on remplace le descripteur
	//initial par cette copie
	if(dup2(fdFichier, fd) == -1){
		perror("Command_execute: dup2 ha failed");
		exit(1);
	}
	
	//On ferme le descripteur de fichier
	if(close(fdFichier)){
		perror("Command_execute: close has failed");
		exit(1);
	}
	
}

static void __soft_fail__ (int pipeGauche[2], int pipeDroite[2]) {
	
	//On ferme tous les pipes "ouverts"
	
	
	if(pipeGauche[0] != -1) {
		close(pipeGauche[0]);
		pipeGauche[0] = -1;
	}
	
	if (pipeGauche[1] != -1) {
		close(pipeGauche[1]);
		pipeGauche[1] = -1;
	}
	
	if(pipeDroite[0] != -1) {
		close(pipeDroite[0]);
		pipeDroite[0] = -1;
	}
	
	if(pipeDroite[1] != -1) {
		close(pipeDroite[1]);
		pipeDroite[1] = -1;
	}
}

int IMPLEMENT(Command_execute)(Command *cmd)
{
	/*-------- Commandes intégrées au shell --------*/
	
	
	//Commande Exit
	if(!stringCompare(cmd->base,"exit")){
		exit(0);
	}
	//Commande Cd
	else if(!stringCompare(cmd->base,"cd")){
		if (cmd->nbOptions == 1) {
			const char *hd;
			userInformation(NULL, &hd, NULL);
			CmdMember_addOption(cmd, hd, 0);
			if (!cmd->status) {
				perror("Command_execute: cd: CmdMember_addOption: failed.");
				exit(1);
			}
		}
		else if (cmd->nbOptions != 2) {
			perror("Command_execute: cd: invalid number of arguments.");
			exit(1);
		}
		if (chdir(cmd->options[1])) {
			perror("Command_execute: cd failed.");
			exit(1);
		}
		return 0;
	}
	
	
	/*------ Fin Commandes intégrées au shell ------*/
	
	
	
	
	/*------------- Commandes Externes -------------*/
	
	
	int nbProcessCrees = 0;
	int codeErreur = 0;
	int pipeGauche[2] = { -1, -1 }; // communication avec la commande qui précède
	int pipeDroite[2] = { -1, -1 }; // communication avec la commande qui suit
	
	//On lance toutes les commandes
	while (cmd) {
		
		//Entrée : cmd->next != NULL 
		//	=> il faut créer un pipe pour communiquer avec la commande qui suit
		if (cmd->next) {
			if (pipe(pipeDroite)) {
				perror("Command_execute: pipe");
				__soft_fail__(pipeGauche, pipeDroite);
				codeErreur = 1;
				break;
			}
		}
		
		//on duplique le processus courant pour faire exécuter cmd->base par un processus fils
		pid_t pid = fork();
		
		switch(pid){
			
			//Erreur de fork
			case -1:
				perror("Command_execute: fork failed");
				__soft_fail__(pipeGauche, pipeDroite);
				codeErreur = 1;
			break;
			
			//Processus fils
			case 0:
				/*------ Redirections ------*/
				
				/*---- Fin Redirections ----*/
				
				//Redirections entrée
				if (cmd->prev) {
					//Vers l'extremité en lecture du pipe de gauche
					if (dup2(pipeGauche[0], 0) == -1) {
						perror("Command_execute: dup2 failed");
						exit(1);
					}
					if (close(pipeGauche[0])) {
						perror("Command_execute: close failed");
						exit(1);
					}
					if (close(pipeGauche[1])) {
						perror("Command_execute: close failed");
						exit(1);
					}
				}				
				else if(cmd->redirectionTypes[0] == NORMAL){
					__make_redirect__(cmd->redirections[0], O_RDONLY, 0);
				}
				
				
				
				//Redirections de Sortie
				 if (cmd->next) {
					//Vers l'extremité en écriture du pipe de droite
					if (dup2(pipeDroite[1], 1) == -1) {
						perror("Command_execute: dup2 failed");
						exit(1);
					}
					if (close(pipeDroite[0])) {
						perror("Command_execute: close failed");
						exit(1);
					}
					if (close(pipeDroite[1])) {
						perror("Command_execute: close failed");
						exit(1);
					}
				}
				else if(cmd->redirectionTypes[1] == NORMAL){
					__make_redirect__(cmd->redirections[1], O_WRONLY | O_TRUNC | O_CREAT, 1);
				}
				else if(cmd->redirectionTypes[1] == APPEND){
					__make_redirect__(cmd->redirections[1], O_WRONLY | O_APPEND | O_CREAT, 1);
				}
				
				
				
				//Redirection des erreurs
				if(cmd->redirectionTypes[2] == NORMAL){
					__make_redirect__(cmd->redirections[2], O_WRONLY | O_TRUNC | O_CREAT, 2);
				}
				else if(cmd->redirectionTypes[2] == APPEND){
					__make_redirect__(cmd->redirections[2], O_WRONLY | O_APPEND | O_CREAT, 2);
				}
				else if(cmd->redirectionTypes[2] == FUSION){
					if(dup2(1,2)==-1){
						perror("Command_execute: dup2 has failed\n");
						exit(1);
					}
				}
				
				
			
				//On remplace le programme executé par le processus fils
				//par un nouveau (cmd->base)
				CmdMember_addOption(cmd,NULL, 0);
				 if (!cmd->status) {
					perror("Command_execute: cd: CmdMember_addOption: failed.");
					exit(1);
				}
				execvp(cmd->base, cmd->options);
				
				//Normalement pas d'execution de ces lignes après execvp
				//(on ne revient pas de execvp en cas de bonne execution)
				perror("Command_execute: execvp failed");
				exit(1);
			break;
			
			//Processus parent
			default :
				//Attend fin execution fils
				++nbProcessCrees;
			break;
		}
		
		
		//On ferme le pipe de gauche car il ne sera plus utile dans la suite
		if (cmd->prev) {
			if (close(pipeGauche[0])) {
				perror("Command_execute: close failed");
				__soft_fail__(pipeGauche, pipeDroite);
				codeErreur = 1;
				break;
			}
			
			
			if (close(pipeGauche[1])) {
				perror("Command_execute: close failed");
				__soft_fail__(pipeGauche, pipeDroite);
				codeErreur = 1;
				break;
			}
		}
		
		//Le pipe de droite à l'itération i est égale au pipe de gauche à l'itération i + 1
		pipeGauche[0] = pipeDroite[0];
		pipeGauche[1] = pipeDroite[1];
		pipeDroite[0] = -1;
		pipeDroite[1] = -1;
		cmd = cmd->next;
	}
	
	
	/*----------- Fin Commandes Externes -----------*/
	
	
	
	//Attente fin de toutes les commandes
	int nbProcessTermines = 0;
	while (nbProcessTermines < nbProcessCrees){
		int status;
		if(wait(&status) != -1){
			++nbProcessTermines;
			if(		!WIFEXITED (status)
				||	WEXITSTATUS (status)
			){
				codeErreur = 1;
			}
		}
	}
	
	
	return codeErreur;
	
	//return provided_Command_execute(cmd);
}
