# KMeans

![Language](https://img.shields.io/badge/Language-C++-blue.svg)
![IDE](https://img.shields.io/badge/IDE-Code::Blocks_&_Visual_Studio-green.svg)
![Librairie](https://img.shields.io/badge/Librairie-WinBGI_&_OpenMP-red.svg)

### Description

L'algorithmes des K-Means est un algorithme de classification non supervisé. Il permet de regrouper des données en fonction de leur proximité.

![KMeans](https://upload.wikimedia.org/wikipedia/commons/thumb/e/ea/K-means_convergence.gif/220px-K-means_convergence.gif)

---

La version séquentielle de l'algorithme est implémentée dans le fichier `KMeans_sequentielle`. La version parallèle est implémentée dans le fichier `KMeans_parallel_OMP`, elle utilise la librairie OpenMP disponible directement dans les options de compilation de Visual Studio.
Une version avec une interface graphique est disponible dans le fichier `KMeans_winbgi`. Attention elle nécessite l'ide Code::Blocks et la librairie WinBGI.

---

![KMeans](demo.gif)