/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(Diego Yavhed Medinilla Gonzalez 424016797) Ponga su nombre y numero de cuenta aqui.
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/


#include "stdafx.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "corrector.h"
#define DEBUG 0

int esEspacioBlanco(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\0' || c == '.' || c == ',' || c == '(' || c == ')' || c == ';';
}

//Funciones publicas del proyecto
/*****************************************************************************************************************
	DICCIONARIO: Esta funcion crea el diccionario completo
	char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario	
	char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
	int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
	int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void	Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
	FILE* fpDict;
	char texto[NUMPALABRAS];
	char wdetec[TAMTOKEN];
	int cont, contp, iNumE, ltexto, i;
	iNumElementos = 0;

	fopen_s(&fpDict, szNombre, "r");

	if (fpDict != NULL)
	{
		iNumE = 0;
		contp = 0;
		while (fgets(texto, sizeof(texto), fpDict) != NULL)
		{
			ltexto = strlen(texto);

			for (cont = 0; cont < ltexto; cont++)
			{
				if (cont > 0 && (texto[cont] == ' ' || texto[cont] == '\t' || texto[cont] == '\n' || texto[cont] == '\r' || texto[cont] == '\0' || texto[cont] == '.' 
					|| texto[cont] == ',' || texto[cont] == '(' || texto[cont] == ')' || texto[cont] == ';') && !esEspacioBlanco(texto[cont - 1]))
				{
					wdetec[contp] = '\0';
					int palabraExistente = 0;
					for (i = 0; i < iNumE; i++)
					{
						if (strcmp(szPalabras[i], wdetec) == 0)
						{
							iEstadisticas[i]++;
							palabraExistente = 1;
							break;
						}
					}

					if (!palabraExistente)
					{
						strcpy_s(szPalabras[iNumE], TAMTOKEN, wdetec);
						iEstadisticas[iNumE] = 1;
						iNumE++;
					}
					contp = 0;
				}
				else
				{
					if (!(texto[cont] == ' ' || texto[cont] == '\t' || texto[cont] == '\n' || texto[cont] == '\r' || texto[cont] == '\0') && texto[cont] != ',' && texto[cont] != '.' 
						&& texto[cont] != '(' && texto[cont] != ')' && texto[cont] != ';')
					{
						texto[cont] = tolower(texto[cont]);
						wdetec[contp] = texto[cont];
						contp++;
					}
				}
			}
			if (cont > 0 && (texto[cont] == ' ' || texto[cont] == '\t' || texto[cont] == '\n' || texto[cont] == '\r' || texto[cont] == '\0' || texto[cont] == '.' || texto[cont] == ',' 
				|| texto[cont] == '(' || texto[cont] == ')' || texto[cont] == ';') && !esEspacioBlanco(texto[cont - 1])) {
				wdetec[contp] = '\0';
				wdetec[contp] = tolower(wdetec[contp]);
				int palabraExistente = 0;
				for (i = 0; i < iNumE; i++)
				{
					if (strcmp(szPalabras[i], wdetec) == 0)
					{
						iEstadisticas[i]++;
						palabraExistente = 1;
						break;
					}
				}

				if (!palabraExistente)
				{
					strcpy_s(szPalabras[iNumE], TAMTOKEN, wdetec);
					iEstadisticas[iNumE] = 1;
					iNumE++;
				}
				contp = 0;
			}
		}
		iNumElementos = iNumE;

		//bubble sort
		int cont1, cont2, auxi;
		char aux[TAMTOKEN];
		for (cont1 = 0; cont1 < iNumElementos - 1; cont1++)
			for(cont2 = 0; cont2 < iNumElementos - 1; cont2++)
				if (strcmp(szPalabras[cont2], szPalabras[cont2 + 1]) > 0)
				{
					strcpy_s(aux, TAMTOKEN, szPalabras[cont2]);
					strcpy_s(szPalabras[cont2], TAMTOKEN, szPalabras[cont2 + 1]);
					strcpy_s(szPalabras[cont2 + 1], TAMTOKEN, aux);
					auxi = iEstadisticas[cont2];
					iEstadisticas[cont2] = iEstadisticas[cont2 + 1];
					iEstadisticas[cont2 + 1] = auxi;
				}
	}
	else
	{
		if (DEBUG == 1)
			printf("\nNo se abrio el archivo");
	}
}

/*****************************************************************************************************************
	ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
	Regresa las palabras ordenadas por su peso
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas		(
	char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
	int		iNumSugeridas,						//Lista de palabras clonadas
	char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
	int		iEstadisticas[],					//Lista de las frecuencias de las palabras
	int		iNumElementos,						//Numero de elementos en el diccionario
	char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
	int		iPeso[],							//Peso de las palabras en la lista final
	int &	iNumLista)							//Numero de elementos en la szListaFinal
{
	iNumLista = 0;
	int cont, cont2, palabraRepetida = 0, cont3, auxp;
	char auxf[TAMTOKEN];

	for (cont = 0; cont < iNumSugeridas; cont++)
	{
		for (cont2 = 0; cont2 < iNumElementos; cont2++)
		{
			if (strcmp(szPalabrasSugeridas[cont], szPalabras[cont2]) == 0)
			{
				for (cont3 = 0; cont3 < iNumLista; cont3++)
				{
					if (strcmp(szPalabrasSugeridas[cont], szListaFinal[cont3]) == 0)
					{
						palabraRepetida = 1;
						break;
					}
				}

				if (!palabraRepetida)
				{
					strcpy_s(szListaFinal[iNumLista], szPalabrasSugeridas[cont]);
					iPeso[iNumLista++] = iEstadisticas[cont2];
				}
				palabraRepetida = 0;
			}
		}
	}

	for (cont = 0; cont < iNumLista - 1; cont++)
		for (cont2 = 0; cont2 < iNumLista - 1; cont2++)
			if (iPeso[cont2] < iPeso[cont2 + 1])
			{
				strcpy_s(auxf, TAMTOKEN, szListaFinal[cont2 + 1]);
				strcpy_s(szListaFinal[cont2 + 1], TAMTOKEN, szListaFinal[cont2]);
				strcpy_s(szListaFinal[cont2], TAMTOKEN, auxf);
				auxp = iPeso[cont2 + 1];
				iPeso[cont2 + 1] = iPeso[cont2];
				iPeso[cont] = auxp;
			}

}

/*****************************************************************************************************************
	ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
	char *	szPalabraLeida,						// Palabra a clonar
	char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
	int &	iNumSugeridas)						//Numero de elementos en la lista
{

	char aux2[TAMTOKEN];
	int cont, cont2, contp = 0, npalabra;
	iNumSugeridas = 0;
	char dict[39] = "abcdefghijklmnñopqrstuvwxyzáéíóú";


	strcpy_s(aux2, TAMTOKEN, szPalabraLeida);
	npalabra = strlen(szPalabraLeida);

	for (cont = 0; cont < npalabra && npalabra != 1; cont++)
	{
		for (cont2 = 0; cont2 < npalabra; cont2++)
		{
			if (cont2 != cont)
			{
				aux2[contp++] = szPalabraLeida[cont2];
			}
		}
		aux2[contp] = '\0';

		strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, aux2);
		strcpy_s(aux2, TAMTOKEN, szPalabraLeida);
		contp = 0;
	}

	for (cont = 0; cont < npalabra - 1; cont++)
	{
		aux2[cont] = szPalabraLeida[cont + 1];
		aux2[cont + 1] = szPalabraLeida[cont];
		strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, aux2); 
		strcpy_s(aux2, TAMTOKEN, szPalabraLeida); 
	}
	strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, szPalabraLeida);

	for (cont = 0; cont < npalabra; cont++)
	{
		for (cont2 = 0; cont2 < 32; cont2++)
		{
			aux2[cont] = dict[cont2];
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, aux2);
		}
		aux2[cont] = szPalabraLeida[cont];
	}

	for (cont = 1; cont < npalabra + 1; cont++)
	{
		aux2[cont] = szPalabraLeida[cont - 1];
	}
	aux2[cont] = '\0';

	for (cont = 0; cont < npalabra + 1; cont++)
	{
		for (cont2 = 0; cont2 < 32; cont2++)
		{
			aux2[cont] = dict[cont2];
			strcpy_s(szPalabrasSugeridas[iNumSugeridas++], TAMTOKEN, aux2);
		}
		aux2[cont] = szPalabraLeida[cont];
	}

	for (cont = 0; cont < iNumSugeridas - 1; cont++) 
		for (cont2 = 0; cont2 < iNumSugeridas - 1; cont2++) 
			if (strcmp(szPalabrasSugeridas[cont2], szPalabrasSugeridas[cont2 + 1]) > 0) 
			{
				strcpy_s(aux2, TAMTOKEN, szPalabrasSugeridas[cont2]);
				strcpy_s(szPalabrasSugeridas[cont2], TAMTOKEN, szPalabrasSugeridas[cont2 + 1]);
				strcpy_s(szPalabrasSugeridas[cont2 + 1], TAMTOKEN, aux2);
			}

}
