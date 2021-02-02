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

#include "misc/string.h"
#include "misc/ferror.h"

// #########################################################################
// #########################################################################
// #########################################################################

char IMPLEMENT(toLowerCase)(char c)
{	
	if(c>='A' && c<='Z'){
		c = c - 'A' + 'a';
	}
	
	return c;
	
	
    //return provided_toLowerCase(c);
}

char IMPLEMENT(toUpperCase)(char c)
{
	
	if(c>='a' && c<='z'){
		c= c - 'a' + 'A';
	}


	return c;
    
	
	//return provided_toUpperCase(c);
}

size_t IMPLEMENT(stringLength)(const char *str)
{
	if(!str) return 0;
	
	size_t i=0;
	
	//Parcours de la chaîne
	//	=> Incrémentation compteur
    while (str[i] != '\0'){
		i++;
	}
	return i;
	
	
	//return provided_stringLength(str);
}

char* IMPLEMENT(duplicateString)(const char *str)
{
	if(!str) return NULL;
	
	size_t strLength = stringLength(str) +1;
	char *strCopy = malloc(sizeof(char)*strLength);
	
	if(strCopy){
		copyStringWithLength(strCopy,str,strLength);
	}
	
	return strCopy;
    //return provided_duplicateString(str);
}

const char* IMPLEMENT(findFirst)(const char *str, const char *separators)
{
    int finded = 0;
	size_t strLength = stringLength(str),sepLength = stringLength(separators), i = 0, j=0;
	
	//Tant qu'on a pas trouvé d'occurence d'un caractère de separators dans str
	//Et pas fin de chaîne
	while(finded == 0 && i<strLength){
		j=0;
		//Parcours de separators
		while(finded == 0 && j<sepLength){
			//On arrête dès qu'on trouve
			if(str[i] == separators[j]){
				finded=1;
			}
			j++;
		}
		i++;
	}
	
	//Retourne NULL si 
	return (finded==0?NULL:&str[i-1]);
	
	//return provided_findFirst(str, separators);
}

char* IMPLEMENT(findLast)(char *str, char c)
{
    char *memory = 0;
	size_t strLength = stringLength(str), i = 0;
	
	//Parcours de la chaîne
	while(i<strLength){
		
		//On stock la position à caque caractère trouvé
		if(str[i] == c){
			memory=&str[i];
		}
		
		i++;
	}
	
	return memory;
	
	//return provided_findLast(str, c);
}

int IMPLEMENT(stringCompare)(const char *str1, const char *str2)
{
	while(*str1 && *str1 == *str2){
		str1++;
		str2++;
	}
	return *str1 - *str2;
	
    //return provided_stringCompare(str1, str2);
}

const char* IMPLEMENT(indexOfString)(const char *foin, const char *aiguille, int csensitive)
{
    int finded = 0;
	
	if(stringLength(foin) >= stringLength(aiguille)){
		
		while(*foin && finded == 0){
			if(startWith(foin,aiguille,csensitive) != NULL){
				finded=1;
			}
			++foin;
		}
	}
	
	//Retourne NULL si on aiguille pas trouvée dans foin
	//sinon retourne l'emplacment mémoire du 1er caratère de aiguille dans foin
	return finded == 0? NULL : --foin;
	
	
	//return provided_indexOfString(foin, aiguille, csensitive);
}

char* IMPLEMENT(concatenateStrings)(const char *str1, const char *str2, size_t minDestSize)
{
	size_t i, str1Length = stringLength(str1), str2Length = stringLength(str2);
	if(minDestSize == (size_t)0){
		minDestSize=str1Length+str2Length+1;
	}
		
	char *concatStr = malloc(sizeof(char)*minDestSize);

	if(concatStr){
		copyStringWithLength(concatStr,str1,minDestSize);//  = duplicateString(str1);// concatStr = str1;
		i=0;
		while(str2[i]!='\0' && i<minDestSize){
			concatStr[str1Length+i] = str2[i];
			++i;
		}
		concatStr[str1Length+i]='\0';
	}
	return concatStr;
	
    //return provided_concatenateStrings(str1, str2, minDestSize);
}

void IMPLEMENT(copyStringWithLength)(char *dest, const char * src, size_t destSize)
{
	if(destSize == 0){
		fatalError("destSize == 0");
	}
	
	if(src){
		size_t i = 0;
		while(i<destSize-1 && src[i]!='\0'){
			dest[i] = src[i];
			++i;
		}	
		dest[i]='\0';
	}
	
    //provided_copyStringWithLength(dest, src, destSize);
}

char* IMPLEMENT(mkReverse)(char *str)
{	
	size_t i=0, strLength = stringLength(str);
	char tmp;
	
	//On ne parcours que la moitié de la chaîne
	while(i<strLength/2){
		/* On stocke de manière temporaire l'un des 2
		 * caractères à échanger puis on fait l'échange
		 */
		tmp=str[i];
		str[i]=str[strLength-i-1];
		str[strLength-i-1]=tmp;
		++i;
	}
	
	return str;
    //return provided_mkReverse(str);
}

const char* IMPLEMENT(startWith)(const char *str, const char *prefix, int csensitive)
{
	if(!str) return NULL;
	
	int stop = 1;
	
	if(stringLength(str) >= stringLength(prefix)){
		stop = 0;
		//Parcours de la chaîne de préfixes
		while(*prefix && stop == 0){
			//Si comparaison non sensilbe à la casse => comparaison avec caractères en majuscule
			//Sinon comparaison normale
			// => Arrête le parcours si différence
			if(    (csensitive == 0 && toUpperCase(*str) != toUpperCase(*prefix)) 
				|| (csensitive != 0 && *prefix != *str)){
				stop = 1;
			}
			++prefix;
			++str;
		}
	}
	
	//Retourne NULL si on a trouvé une différence
	//sinon retourne l'emplacment mémoire du caratère, dans str, suivant le préfixe
	return stop == 1 ? NULL : str;
	
	
    //return provided_startWith(str, prefix, csensitive);
}

int IMPLEMENT(belongs)(char c, const char *str)
{
	int finded = 0;
	size_t strLength = stringLength(str), i = 0;
	
	while(finded == 0 && i<strLength){
		if(str[i] == c){
			finded=1;
		}
		i++;
	}
	
	return finded;
	
	
    //return provided_belongs(c, str);
}

char* IMPLEMENT(subString)(const char *start, size_t length)
{
	char *subString = malloc(sizeof(char)*++length);
	if(!subString) return NULL;
	copyStringWithLength(subString,start,length);
	return subString;
	
    //return provided_subString(start, length);
}

void IMPLEMENT(mkCommon)(char *result, const char *str)
{
	int i=0;
	
	//On parcours les chaînes jusqu'à ce qu'on trouve une différence
	while(result[i]==str[i] && str[i]!='\0' && result[i]!='\0'){
		++i;
	}
	copyStringWithLength(result,str,i+1);
	
    //provided_mkCommon(result, str);
}

int IMPLEMENT(isNotEmpty)(const char *str)
{
	int isFill=0;
    size_t strLength = stringLength(str), i=0;
	
	if(strLength>0){
		if(belongs(' ',str)==1){
			while(isFill==0 && i<strLength){
				if(str[i] != ' '){
					isFill=1;
				}
			i++;
			}
		} else {
			isFill = 1;
		}
	} 
	
	return isFill;
	
	//return provided_isNotEmpty(str);
}

char* IMPLEMENT(getProtString)(const char *str, char c)
{
	if(!str) return NULL;
	
	size_t strLength=stringLength(str),i=0,j=0;
	char *strCopy;
	
	if(belongs(c,str)){
		strCopy=malloc(sizeof(char)*2*strLength+1);
		if(!strCopy) return NULL;

		while(i<strLength){
			//On copie les caractères (en prenant comptes du nombre de char ajoutés)
			strCopy[i+j]=str[i];
			if(str[i]==c){
				/* 	On incrémente une variable qui permet de savoir le nombre de
					caratères ajoutés et on insère le caractère	après celui a échapper */
				strCopy[i+1+j]=c;
				++j;
			}
			++i;
		}
		strCopy[i+j]='\0';
	} else {
		strCopy=duplicateString(str);
	}
	
	return strCopy;
		
    //return provided_getProtString(str, c);
}

char* IMPLEMENT(getRealString)(const char *str, char c, char **firstNotEscaped)
{	
	if(!str) return NULL;
	
	size_t strLength=stringLength(str),i=0,j=0;
	char *strCopy;
	int first=-1;
	if(firstNotEscaped) *firstNotEscaped=NULL;
	
	//Entrée : la chaîne contient le caractère voulu
	if(belongs(c,str)){
		
		strCopy=malloc(sizeof(char)*strLength+1);
		if(!strCopy) return NULL;

		//Parcours de la chaîne
		while(i<strLength){
			
			//On copie les caractères (en prenant comptes le nombre de char supprimés)
			strCopy[i-j]=str[i];
			if(str[i]==c){
				if(str[i+1]==c){
					//On incrémente une variable pour savoir le nombre de caratères supprimés
					++j;
					++i;
				} else if(first==-1){
					first=i; //On conserve la position du 1er char non echappé
				}
			}
			++i;
		}
		strCopy[i-j]='\0';
	} else strCopy = duplicateString(str);
	
	
	/* On initialise firstNotEscaped à vide si :
	 * 	- caratère non trouvé
	 *  - caractère trouvé mais toujours échappé
	 * Sinon on l'initialise à la position du premier trouvé
	 */
	if(firstNotEscaped) *firstNotEscaped = first==-1?NULL:&strCopy[first];
	
	return strCopy;
	
    //return provided_getRealString(str, c, firstNotEscaped);
}

MAKE_NEW_2(Tokenizer, const char*, const char*)
MAKE_DEL_0(Tokenizer)

int IMPLEMENT(Tokenizer_init)(Tokenizer *tokenizer, const char *str, const char *separators)
{
	
	
	if(!str || !separators) return 1;
	
	tokenizer->str = NULL;
	tokenizer->next = str;
	tokenizer->separators = separators;
	
	Tokenizer_next(tokenizer);
	
	return 0;
	
	
    //return provided_Tokenizer_init(tokenizer, str, separators);
}

void IMPLEMENT(Tokenizer_finalize)(Tokenizer *tokenizer)
{
	(void)tokenizer;
    //provided_Tokenizer_finalize(tokenizer);
}

int IMPLEMENT(Tokenizer_isOver)(const Tokenizer *tokenizer)
{
	return !tokenizer->next && !tokenizer->str;
	
    //return provided_Tokenizer_isOver(tokenizer);
}

char* IMPLEMENT(Tokenizer_get)(const Tokenizer *tokenizer)
{
	size_t i = 0;
	
	//On récupère la taille jusqu'au prochain séparateur
	//	=> si pas de séparateur : tout jusqu'à la fin de la chaîne
	if(tokenizer->next){
		i = tokenizer->next - tokenizer->str;
	} else {
		i = stringLength(tokenizer->str);
	}
	
	char *toReturn = malloc(sizeof(char)*(i+1));
	if(!toReturn) return NULL;
	
	//On copie du début jusqu'à la position du prochain séparateur
	copyStringWithLength(toReturn,tokenizer->str,i+1);
	
	return toReturn;
	
    //return provided_Tokenizer_get(tokenizer);
}

void IMPLEMENT(Tokenizer_next)(Tokenizer *tokenizer)
{
	//Si tokenizer->next est NULL alors les 2 doivent l'être
	if(tokenizer->next){
		int i=0;
		
		//On parcours la chaîne jusqu'à trouver un caractère qui n'est pas un separateur
		while(tokenizer->next && tokenizer->next[i] != '\0' && belongs(tokenizer->next[i],tokenizer->separators)){
			++i;
		}
		
		//Si on est arrivé en fin de chaîne on renvoie NULL car il n'existe pas de caractère après
		tokenizer->str = tokenizer->next[i] == '\0'?NULL:tokenizer->next+i;
		tokenizer->next = findFirst(tokenizer->str,tokenizer->separators);
	} else {
		tokenizer->str = NULL;
	}
	
    //provided_Tokenizer_next(tokenizer);

}
