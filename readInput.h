/**
 * \file    readInput.h
 * \brief   Fonctions utilitaires pour lire/écrire des fichiers et la ligne de commande
 * \author  ASI331
 * \version 1.0
 * \date    9 novembre 2017
 */

#ifndef __READ_INPUT_H__
#define __READ_INPUT_H__

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \struct  iacbc_input_t
 * \brief   Structure contenant les paramètres d'entrée
 * 
 * Structure contenant les paramètres issus du parsing de la ligne de commande :
 *  - bool  enc     true s'il faut chiffrer, false s'il faut déchiffrer
 *  - char* pwd     mot de passe à utiliser pour chiffrer/déchiffrer
 *  - char* IV      IV en hexadécimal (ASCII) à utiliser pour chiffrer/déchiffrer
 *  - char* input   nom du fichier d'entrée à chiffrer/déchiffrer
 *  - char* out     nom du fichier de sortie pour enregistrer le déchiffré/chiffré
 */
typedef struct {
    bool  enc;
    char* pwd;
    char* IV;
    char* input;
    char* out;
} iacbc_input_t;

/**
 * \brief      Parse la ligne de commande
 * \param[in]  argc   Nombre d'arguments
 * \param[in]  argv   Arguments de la ligne de commande
 * \param[out] params Structure dans laquelle seront enregistrés les paramètres
 * \return     true si le parsing s'est bien passé, false sinon
 * 
 * Fonction qui se charge de parser la ligne de commande et qui enregistre les 
 * paramètres dans la structure params
 */
bool parse_input (int argc, char * const *argv, iacbc_input_t* params);

/**
 * \brief      Lit un fichier
 * \param[in]  filename Nom du fichier
 * \param[out] buf      Chaîne dans laquelle sera stockée le contenu lu
 * \param[out] count    Taille de la chaîne
 * \return     true si la lecture s'est bien passée, false sinon
 * 
 * Fonction qui lit le fichier filename et met le contenu dans buf et la taille 
 * du contenu dans count
 */
bool read_file(const char* filename, uint8_t** buf, size_t* count);

/**
 * \brief      Écrit un fichier
 * \param[in]  filename Nom du fichier
 * \param[in]  buf      Chaîne dans laquelle est stockée le contenu à écrit
 * \param[in]  count    Taille de la chaîne
 * \return     true si l'écriture s'est bien passée, false sinon
 * 
 * Fonction qui écrit dans le fichier filename le contenu stocké dans buf de 
 * taille count
 */
bool write_file(const char* filename, const uint8_t* buf, size_t count);


/**
 * \brief      Converti une chaîne hexadécimale (ASCII) en binaire
 * \param[in]  hex     Chaîne de caractères contenant la chaîne hexadécimale
 * \param[in]  hexsize Taille de la chaîne hexadécimale
 * \param[out] bin     Chaîne de caractères dnas laquelle sera stockée le binaire
 * \param[out] binsize Taille de la chaîne binaire
 * \return     true si la conversion s'est bien passée, false sinon
 * 
 * Fonction qui converti la chaîne hexadécimale (ASCII) hex de taille hexsize
 * en binaire dans bin et de taille binsize
 */
bool hex2bin(const char* hex, size_t hexsize, uint8_t** bin, size_t* binsize);

#endif
