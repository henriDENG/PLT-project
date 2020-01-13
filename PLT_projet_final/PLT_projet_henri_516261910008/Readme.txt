### Quelques remarques

-> Les codes *.h et *.c sont dans le dossier «src». 

-> Les commandes pour tester les codes : 
gcc -o analyseur_lexical analyseur_lexical.c
./analyseur_lexical

gcc -o analyseur_syntaxique analyseur_syntaxique.c
./analyseur_syntaxique

gcc -o analyseur_semantique analyseur_semantique.c
./analyseur_semantique

gcc -o compile_automate compile_automate.c
./compile_automate

gcc -o executeur executeur.c
./executeur -debug VM
./executeur  VM

-> Les fichiers dans le dossier «resultats» sont les fichiers obtenus par le test avec Dpile.txt. 
     Vous pouvez voir si vous obtenez les mêmes fichiers intermédiaires et finals.

-> La commande pour obtenir le graphe de l'arbre syntaxique :
sh dot.sh