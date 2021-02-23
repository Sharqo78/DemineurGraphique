# Démineur graphique


## Introduction
C'est un projet de classe devant être réalisé en C utilisant la bibliothèque SDL2 ainsi que son extension, `SDL_ttf`
J'ai pris le choix d'utiliser le thème de couleurs "Arc" (provenenant du thème Gnome du même nom) car elles ne brûlent pas les yeux de nuit.

## Aspects incomplets 
        - Souci au niveau de la police d'écriture réglée (il faut lancer l'exécutable directement depuis son chemin, il ne faut pas utiliser vscode) (réglé)
        - Rendre les polices d'écritures plus agréables à voir (elles sont étirées et pixellisés) (réglé)
        - Faire en sorte que lorsqu'on clique au niveau des intersections, aucune case s'active. (réglé)
        - Ecrire les chiffres de chaque case (lorsqu'il y'a adjacence) (fait)
        - Possiblement essayer d'utiliser un bitmap pour la mine (fait)
        - Ajouter le système de flags et interrogation sur une case potentiellement minée (à faire)

## Bugs
        - Le drapeau s'enlève lorsqu'on fait un clic gauche sur la case
