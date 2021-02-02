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

#include "misc/filesystem.h"
#include "misc/string.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_2(FolderIterator, const char*, int)
MAKE_DEL_0(FolderIterator)

int IMPLEMENT(FolderIterator_init)(FolderIterator *fIterator, const char *path, int skipSpecials)
{
	if(!path) return 1;
	
	//Entrée : on ne peut pas ouvrir le dossier
	if(!(fIterator->dir = opendir(path))) return 1;
	
	fIterator->skipSpecials = skipSpecials;
	fIterator->ent=NULL;
	FolderIterator_next(fIterator);
	
	return 0;
    
	//return provided_FolderIterator_init(fIterator, path, skipSpecials);
}

void IMPLEMENT(FolderIterator_finalize)(FolderIterator *fIterator)
{
    free(fIterator->ent);
	closedir(fIterator->dir);
	//provided_FolderIterator_finalize(fIterator);
}

int IMPLEMENT(FolderIterator_isOver)(const FolderIterator *fIterator)
{
	return fIterator->ent==NULL;
	
	//return provided_FolderIterator_isOver(fIterator);
}

const char* IMPLEMENT(FolderIterator_get)(const FolderIterator *fIterator)
{
	return fIterator->ent->d_name;
	
	//return provided_FolderIterator_get(fIterator);
}

int IMPLEMENT(FolderIterator_isDir)(const FolderIterator *fIterator)
{
	return fIterator->ent->d_type == DT_DIR;
	
    //return provided_FolderIterator_isDir(fIterator);
}

void IMPLEMENT(FolderIterator_next)(FolderIterator *fIterator)
{
	
	struct dirent *dent = readdir(fIterator->dir);
	fIterator->ent = dent;
	
	//Entrée : On souhaite éviter les dossiers "." et ".."
	//		ET le fichier parcouru exite
	//		ET il a un nom
	//		ET il correspond à "." OU à ".."
	//		ET il existe un fichier après
	//	=> on saute le dossier
	if( 	fIterator->skipSpecials
		&& 	dent 
		&& 	dent->d_name
		&& 	(	stringCompare(dent->d_name, "." )==0
			||	stringCompare(dent->d_name, "..")==0
			)
		&&	!FolderIterator_isOver(fIterator)
	  ){
			FolderIterator_next(fIterator);
	}
    
	//provided_FolderIterator_next(fIterator);
}

MAKE_NEW_1(FileIterator, FILE*)
MAKE_DEL_0(FileIterator)

int IMPLEMENT(FileIterator_init)(FileIterator *fIterator, FILE *file)
{
	if(!file) return 1;
	
	fIterator->file = file;
	fIterator->current=NULL;
	
	if(!FileIterator_isOver(fIterator))
		FileIterator_next(fIterator);
	
	return 0;
	
    //return provided_FileIterator_init(fIterator, file);
}

void IMPLEMENT(FileIterator_finalize)(FileIterator *fIterator)
{
	free(fIterator->current);
	
    //provided_FileIterator_finalize(fIterator);
}

int IMPLEMENT(FileIterator_isOver)(const FileIterator *fIterator)
{
	return feof(fIterator->file);
    //return provided_FileIterator_isOver(fIterator);
}

const char* IMPLEMENT(FileIterator_get)(const FileIterator *fIterator)
{
	return fIterator->current;
    //return provided_FileIterator_get(fIterator);
}

void IMPLEMENT(FileIterator_next)(FileIterator *fIterator)
{
	if(fIterator->current) free(fIterator->current);
	
	size_t len = 0;
	int read = 0;
	char *line = NULL;
	
	//Entrée : on peut lire le fichier
	if((read = getline(&line, &len, fIterator->file))!=-1 && line){
				
		//Entrée : Il y en a un retour à la ligne
		//	=> On remplace le char de retour à la ligne par un caractère de fin de chaîne
		if(read>1 && line[read-1]=='\n'){
			line[read-1]='\0';
		}
		
		char *position, *realStr = getRealString(line,'#', &position);
		
		//Entrée : il y a un char '#' non échappé (commentaire)
		//	=> On remplace le char '#' par un caractère de fin de chaîne
		if(position){
			size_t realLength = stringLength(realStr), posLength = stringLength(position);
			realStr[realLength-posLength]='\0';
		}
		
		fIterator->current = duplicateString(realStr);
		free(realStr);
		
	} else {
		fIterator->current = NULL;
	}
	
	free(line);
	
	//provided_FileIterator_next(fIterator);
}
