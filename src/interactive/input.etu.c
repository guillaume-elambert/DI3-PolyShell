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

#include "interactive/all.h"
#include "misc/./string.h"

// #########################################################################
// #########################################################################
// #########################################################################

MAKE_NEW_0(Input)
MAKE_DEL_0(Input)

int IMPLEMENT(Input_init)(Input *input)
{
	input->current = NULL;
	return 0;
    //return provided_Input_init(input);
}

void IMPLEMENT(Input_finalize)(Input *input)
{
	if(input->current){
		Cell *droite = input->current->next;
		while(droite){
			Cell *tmp = droite->next;
			Cell_delete(droite);
			droite = tmp;
		}
		
		Cell *gauche = input->current->previous;
		while(gauche){
			Cell *tmp = gauche->previous;
			Cell_delete(gauche);
			gauche = tmp;
		}
		Cell_delete(input->current);
	}
	
    //provided_Input_finalize(input);
}

void IMPLEMENT(Input_clear)(Input *input)
{
	Input_finalize(input);
	Input_init(input);
	
    //provided_Input_clear(input);
}

size_t IMPLEMENT(Input_size)(const Input *input)
{	
	size_t size = 0;
	if(input->current){
		for(Cell *droite = input->current; droite!=NULL; droite = droite->next){
			size +=Bucket_size(&droite->bucket);
		}
		
		for(Cell *gauche = input->current->previous; gauche!=NULL; gauche = gauche->previous){
			size +=Bucket_size(&gauche->bucket);
		}
	}
	return size;
	
    //return provided_Input_size(input);
}

char IMPLEMENT(Input_get)(const Input *input)
{
	//Entrée : Liste vide OU Curseur après le dernier caractère
	if (!input->current | (!input->current->next && input->pos > input->current->bucket.top)){
		return '\0';
	}

	return input->current->bucket.content[input->pos];
    //return provided_Input_get(input);
}

int IMPLEMENT(Input_insert)(Input *input, char c)
{
	//Entrée : liste vide
	if(!input->current){
		Cell *newCell = Cell_new();
		if(!newCell){
			return 1;
		}
		Bucket_insert(&newCell->bucket,0,c);
		input->current = newCell;
		input->pos = 1;
	} else {

		//Entrée: le bucket est plein
		if(Bucket_full(&input->current->bucket)){
			Cell *newCell = Cell_new();
			if(!newCell){
				return 1;
			}
			Cell_insertAfter(input->current,newCell);

			//Entrée : on est entre 1 et bucket top
			//	=> On décale le contenu
			if(input->pos <= input->current->bucket.top){
				Bucket_move(&input->current->bucket, input->pos,&newCell->bucket);
			} else { //	=> on initialise la position manuellement car sinon erreur
				input->current=newCell;
				input->pos = 0;
			}
			
		}
		Bucket_insert(&input->current->bucket, input->pos, c);
		Input_moveRight(input);
	}
	return 0;
    //return provided_Input_insert(input, c);
}

int IMPLEMENT(Input_backspace)(Input *input)
{
	//Entrée : liste vide OU début de chaîne
	if(!input->current || (input->pos == 0 && !input->current->previous)){
		return 1;
	}
    
	if(Input_moveLeft(input)) return 1;
	
	return Input_del(input);

	//return provided_Input_backspace(input);
}

int IMPLEMENT(Input_del)(Input *input)
{
	//On retourne un code d'erreur si la cellule courante est nulle 
	//OU si on est au-dela du sommet du bucket et qu'il n'y a pas de cellule suivante
	if(!input->current || (input->pos > input->current->bucket.top && !input->current->next)){
		return 1;
	}
	
	Bucket_remove(&input->current->bucket, input->pos); //On supprime le caractère dans le bucket
	
	//Entrée : le bucket est vide et on peut se déplacer à droite
	if( Bucket_empty(&input->current->bucket) && input->current->next ) {
		Cell *tmp = input->current,
			 *droite = input->current->next,
			 *gauche = input->current->previous;
		
		droite->previous=gauche;		//On modifie la liste chaînée
		if(gauche)gauche->next=droite;	//de sorte d'écraser le
		input->current=droite;			//contenu de input->current
		
		Cell_delete(tmp); //On libère l'espace mémoire de la cellule écrasée
		
		input->pos=0;

	}
	//Entrée : le curseur est après la fin du bucket et on peut se déplacer à droite 
	else if (input->pos > input->current->bucket.top && input->current->next){
		--input->pos;
		if(Input_moveRight(input)) return 1;		
	}
	
    return 0;
	
	
	//return provided_Input_del(input);
}

int IMPLEMENT(Input_moveLeft)(Input *input)
{
	//Entrée : liste vide ou début de chaîne
	if(!input->current || (input->pos==0 && !input->current->previous)){
		return 1;
	}

	//Entrée : début de bucket => on passe au bucket précédent
	if(input->pos==0){
		input->current = input->current->previous;
		input->pos = input->current->bucket.top;
	} else {
		--input->pos;
	}
	return 0;
    //return provided_Input_moveLeft(input);
}


int IMPLEMENT(Input_moveRight)(Input *input)
{
	//Entrée : liste vide ou fin de chaîne
    if(!input->current || (input->pos > input->current->bucket.top && !input->current->next)){
        return 1;
    }

    //Entrée : fin de bucket => on passe au bucket précédent
    if(input->pos == input->current->bucket.top && input->current->next){
        input->current = input->current->next;
		input->pos = 0;
    } else {
		++input->pos;
	}
    return 0;
	
    //return provided_Input_moveRight(input);
}

char* IMPLEMENT(Input_toString)(const Input *input)
{
	Input myInput = *input;
	size_t inputSize = Input_size(&myInput), i = 0;
	char *str=malloc(sizeof(char)*inputSize+1);
	if(!str) return NULL;
	
	while(!Input_moveLeft(&myInput));
	
	while(i<inputSize){
		str[i]=Input_get(&myInput);
		Input_moveRight(&myInput);
		++i;
	}
	str[i]='\0';
	return str;
	
    //return provided_Input_toString(input);
}

void IMPLEMENT(InputIterator_initIterator)(const Input *input, InputIterator *inputIterator)
{
	//Copie du curseur
	*inputIterator = *input;
	
	//On se place sur la première cellule
	if(inputIterator->current){
		//Première cellule
		while(inputIterator->current->previous){
			inputIterator->current=inputIterator->current->previous;
		}
		
		//Premier caractère
		inputIterator->pos = 0;
	}
    //provided_InputIterator_initIterator(input, inputIterator);
}

int IMPLEMENT(InputIterator_equals)(const InputIterator *x, const InputIterator *other)
{
	return x->current == other->current && x->pos == other->pos;
    //return provided_InputIterator_equals(x, other);
}

int IMPLEMENT(InputIterator_isOver)(const InputIterator *inputIterator)
{
	return 		( !inputIterator->current)
			||	( !inputIterator->current->next && inputIterator->pos > inputIterator->current->bucket.top );
    //return provided_InputIterator_isOver(inputIterator);
}

void IMPLEMENT(InputIterator_next)(InputIterator *inputIterator)
{
	Input_moveRight(inputIterator);
    //provided_InputIterator_next(inputIterator);
}

char IMPLEMENT(InputIterator_get)(const InputIterator *inputIterator)
{
	return Input_get(inputIterator);
    //return provided_InputIterator_get(inputIterator);
}

int IMPLEMENT(Input_load)(Input *input, const char *cmd)
{
	if(!cmd || !input){
		return 1;
	}
	
	Input_clear(input);
	
	//Parcours de la chaîne de caractères
	while(*cmd!='\0'){
		if(Input_insert(input,*cmd)!=0){
			return 1;
		}
		++cmd;
	}
	return 0;
    //return provided_Input_load(input, cmd);
}

char* IMPLEMENT(Input_getEditedWord)(const Input *input)
{
	//Si current pas déclaré ou début de chaîne
	if(!input->current || (input->pos==0 && !input->current->previous)) return NULL;
	
	Input myInput = *input;
	int length=0,strBegin=0;
	
	//Parcours des char à gauche jusqu'à un espace ou début de chaîne
	while( (strBegin=!Input_moveLeft(&myInput)) && Input_get(&myInput)!=' '){
		length++;
	}
	
	char *str=malloc(sizeof(char)*length+1);
	if(!str) return NULL;
	
	char c;
	int i=0;
	
	//Gère le cas où on a pas pu se décaler car début de chaîne
	if(!strBegin){
		str[i]=Input_get(&myInput);
		++i;
	}
	
	//Copie des char jusqu'à la position actuelle
	while(!Input_moveRight(&myInput) && i<length && (c=Input_get(&myInput)) != ' '){
		str[i]=c;
		++i;
	}
	str[i]='\0';
	
	
	return str;
    
	//return provided_Input_getEditedWord(input);
}
