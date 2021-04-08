BOGADO GARCIA Maximino
L3 Informatique

Pour plus d'informations sur la machine RAM: http://zanotti.univ-tln.fr/ALGO/II/MachineRAM.html
TP8
Compilateur Algo - RAM |

Les fichiers arc.c/h, lexer.lex, ts.c/h, et parser.y contenus dans ce dossier servent à la création du compilateur.
Le fichier makefile permet de compiler le compilateur.
Le fichier test.algo est un fichier d'exemple qui contient un algorithme pour savoir si un nombre donné est premier (écrit en langage algorithmique).

Lancer la commande make pour compiler les fichiers du compilateur, un fichier binaire arc sera crée.

Pour lancer le programme, faire ./arc <nom du fichier à lire> où le fichier à lire contient un code en langage algorithmique.
A la fin de l'exécution de arc, un fichier "code.ram" sera crée, il contiendra le code ram du fichier lu.
Pour tester le code ram, le simulateur est disponible à cette adresse: http://zanotti.univ-tln.fr/RAM/ram.php

Le compilateur |

	- Un programme commence par le mot-clé DEBUT et finit par le mot-clé FIN

	- La déclaration d'une variable s'utilise avec le mot-clé VAR (exemple: VAR test)

	- L'affectation d'une variable se fait avec la chaine "<-" (exemple: test <- 1)

	- L'écriture dans la sortie standard se fera avec le mot clé AFFICHER, la lecture avec le mot clé LIRE. (exemple: AFFICHER test ou LIRE test)

	- Les opérateurs de comparaison possibles sont >,<,=,>=,<= et !=
	- les opérateurs booléens possibles sont ET, OU et NON (voir exemple de fin)
	- Les opérateurs arithmétiques possibles sont *,/,%,+ et -
	- les parenthèses sont également acceptées

	- La structure conditionnelle s'écrit de la manière suivante, SI-ALORS-SINON-FSI, exemple:
	DEBUT
		VAR test
		VAR exemple
		LIRE test
		LIRE exemple
		SI test=exemple ALORS
			AFFICHER test
		SINON
			AFFICHER exemple
		FSI
	FIN

	Remarque: les indentations sont optionnelles.

	- La structure de boucle s'écrit de la manière suivante, TQ-FAIRE-FTQ, exemple:

	DEBUT
		VAR test
		LIRE test
		TQ test>0 FAIRE
			LIRE test
		FTQ
	FIN

	- La déclaration de tableaux n'est pas encore acceptée.

	- On peut écrire un commentaire en une ligne avec un "#" (exemple: # ceci est un commentaire).

	Exemple de langage algorithmique accepté:

	DEBUT
		VAR i
		VAR faux
		faux <- 0
		i <- 10
		TQ i>=0 FAIRE
			SI i%2=0 ET i<2 ALORS
				AFFICHER i
			SINON
				AFFICHER faux
			FSI
			i <- i-1
		FTQ
	FIN
