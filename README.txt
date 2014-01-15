# Outil minimal de statistiques

(c) Camille Coti, 2014

## Prérequis

Le programme est écrit en C. Il se compile avec la plupart des compilateurs C, comme gcc. Le programme `make` peut être utile pour compiler, mais il n'est pas nécessaire dans l'absolu.

Il a en outre besoin de la bibliothèque de maths `libm`, généralement présente sur à peu près tous les systèmes. 

## Compilation

Un Makefile est fourni dans le répertoire `src`. Il se compile donc avec la commande make. Dans un terminal, placez-vous dans le répertoire `src` et tapez :

make

Si vous ne disposez pas de make sur votre machine, compilez à la main :

gcc -O3 -o moyenne moyenne.c -lm

## Utilisation

Le programme prend en entrée un fichier de données sur deux colonnes. Sur chaque ligne la première colonne contient le paramètre, la deuxième contient la valeur mesurée. Le fichier doit être trié selon la première colonne.

Par exemple :

1	564.254
1 	468.157
1 	655.464
2	7813.872
2	4876.487
2	5966.442
5	451777.445
5	456482.547

Si les données dont vous disposez ne sont pas triées, vous pouvez utiliser l'utilitaire `sort` :

sort -k 1 -n example.dat > sorted.dat

Le programme `moyenne` prend deux paramètres : le nom du fichier d'entrée et le nom du fichier de sortie. 

./moyenne example.dat moy.dat

Ce fichier d'exemples est disponible dans le répertoire `example`.

Il écrit dans le fichier de sortie sur trois colonnes : chaque ligne contient le paramètre, la moyenne et l'écart-type.

## Algorithme de calcul

Afin d'éviter les problèmes de débordement dans la représentation numérique des nombres, l'algorithme utilisé calcule la moyenne et l'écart-type à la volée grâce à l'algorithme de B.P. Welford décrit par D. Knuth dans "The Art of Computer Programming, Vol 2".
