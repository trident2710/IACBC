/**
 * \file    readInput.c
 * \brief   Fonctions utilitaires pour lire/écrire des fichiers et la ligne de commande
 * \author  ASI331
 * \version 1.0
 * \date    9 novembre 2017
 */

#include "readInput.h"

// Std
#include <stdio.h>
#include <unistd.h>

// Opts
#include <getopt.h>

// Read/write file
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Str
#include <string.h>


// Input parsing
void __printUsage(const char* argv0) {
    printf("usage: %s [-h] [-e] [-d] -p PWD -I IV -i INPUT -o OUT\n"
           "\n"
           "Chiffre ou déchiffre un fichier\n"
           "\n"
           "  -h, --help            show this help message and exit\n"
           "  -e, --enc             chiffre le fichier d'entrée\n"
           "  -d, --dec             déchiffre le fichier d'entrée\n"
           "  -p PWD, --pwd PWD     mot de passe\n"
           "  -I IV, --IV IV        vecteur d'initialisation\n"
           "  -i INPUT, --in INPUT  fichier d'entrée à chiffrer/déchiffrer\n"
           "  -o OUT, --out OUT     fichier de sortie chiffré/déchiffré\n"
           , argv0);
}

void __init_iacbc_input(iacbc_input_t* i) {
    i->enc   = NULL;
    i->pwd   = NULL;
    i->IV    = NULL;
    i->input = NULL;
    i->out   = NULL;
}

bool __check_iacbc_input(iacbc_input_t* i) {
    return (i->pwd != NULL && i->IV  != NULL && i->input  != NULL && i->out != NULL);
}

void __print_iacbc_input(iacbc_input_t* i) {
    printf("iacbc_input_t : {\n"
           "  enc   : %s\n"
           "  pwd   : %s\n"
           "  IV    : %s\n"
           "  input : %s\n"
           "  out   : %s\n"
           "}\n",
           i->enc ? "true" : "false",
           i->pwd,
           i->IV ,
           i->input ,
           i->out
         );
}

bool parse_input (int argc, char * const *argv, iacbc_input_t* params) {
    int c;
    __init_iacbc_input(params);

    while (1) {
        static struct option long_options[] =
          {
            /* These options set a flag. */
            {"help", no_argument,       0, 'h'},
            {"enc",  no_argument,       0, 'e'},
            {"dec",  no_argument,       0, 'd'},
            {"pwd",  required_argument, 0, 'p'},
            {"IV",   required_argument, 0, 'I'},
            {"in",   required_argument, 0, 'i'},
            {"out",  required_argument, 0, 'o'},
            {0, 0, 0, 0}
          };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "hedp:I:i:o:",
                         long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {

          case 'h':
            __printUsage(argv[0]);
            return false;
            break;

          case 'e':
            params->enc = true;
            break;

          case 'd':
            params->enc = false;
            break;

          case 'p':
            params->pwd = optarg;
            break;

          case 'I':
            params->IV = optarg;
            break;

          case 'i':
            params->input = optarg;
            break;

          case 'o':
            params->out = optarg;
            break;

          case '?':
            __printUsage(argv[0]);
            return false;
            break;

          default:
            __printUsage(argv[0]);
            return false;
        }
    }

    if (!__check_iacbc_input(params)) {
        __printUsage(argv[0]);
        return false;
    }

    return true;
}

// Read/write file

bool read_file(const char* filename, uint8_t** buf, size_t* count) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error while reading file");
        return false;
    }
    
    // Filesize
    size_t filesize = lseek(fd, 0, SEEK_END); lseek(fd, 0, SEEK_SET);
    // Buffer allocation
    *buf = (uint8_t*)malloc((filesize)*sizeof(char));

    // Reading file
    if ((ssize_t)filesize != read(fd, *buf, filesize)) {
        free(*buf);
        perror("Error while reading file");
        return false;
    }

    if (count != NULL) {
      *count = filesize;
    }

    return true;
}

bool write_file(const char* filename, const uint8_t* buf, size_t count) {
    int fd = open(filename, O_WRONLY | O_CREAT, 00644);
    if (fd == -1) {
        perror("Error while writing file");
        return false;
    }

    // Writing file
    if ((ssize_t)count != write(fd, (const void*)buf, count)) {
        perror("Error while writing file");
        return false;
    }

    return true;
}

// hex2bin
char __nibble2bin(const char c) {
    if ('0' <= c && c <= '9'){
        return (char)(c-'0');
    } else if ('a' <= c && c <= 'f') {
        return (char)(c-'a'+10);
    } else if ('A' <= c && c <= 'F') {
        return (char)(c-'A'+10);
    } else {
        return (char)(-1);
    }
}

bool __hex2bin_nomalloc(const char* hex, size_t hexsize, uint8_t* bin, size_t* binsize) {
    if (*binsize < hexsize/2) {
        printf("Error while converting hex2bin : destination buffer not large enough\n");
        return false;
    }
    *binsize = hexsize/2;

    char tmp;
    for (size_t i = 0; i < *binsize; ++i) {
        tmp = __nibble2bin(hex[2*i]);
        if (tmp == -1) {
            *binsize = 0;
            printf("Error while converting hex2bin : '%c' is not an hexadecimal digit\n", hex[2*i]);
            return false;
        }
        bin[i] = tmp << 4;

        tmp = __nibble2bin(hex[2*i+1]);
        if (tmp == -1) {
            *binsize = 0;
            printf("Error while converting hex2bin : '%c' is not an hexadecimal digit\n", hex[2*i+1]);
            return false;
        }
        bin[i] |= tmp;
    }

    return true;
}

bool hex2bin(const char* hex, size_t hexsize, uint8_t** bin, size_t* binsize) {
    *binsize = hexsize/2;
    *bin = (uint8_t*)malloc((*binsize)*sizeof(char));

    if (!__hex2bin_nomalloc(hex, hexsize, *bin, binsize)) {
        free(*bin);
        return false;
    }

    return true;
}
