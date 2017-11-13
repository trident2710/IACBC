# IACBC
encryption with authentification

pour compiler ce programme, executer "make asi331examen" qui va creer l'executable l'appelle "asi331examen".
Exemple de l'execution:
pour chiffrer: "./asi331examen -e -p password -I ffffffffffffffffffffffffffffffaa -i plain_file -o encrypted"
pour déchiffrer: "./asi331examen -d -p password -I ffffffffffffffffffffffffffffffaa -i encrypted -o restored"

Important: IV est valeur hex de taille <=16bytes (moins où égal à la taille du block) dans un format sans prefix "0x"
