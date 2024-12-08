#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"

int main(void) {
    Graph graph;
    while (1) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1. Charger un reseau et afficher ses listes d'adjacence\n");
        printf("2. Calculer et afficher les predecesseurs\n");
        printf("3. Détecter les composantes connexes\n");
        printf("4. Analyser les sommets\n");
        printf("5. Calculer les niveaux trophiques\n");
        printf("6. Afficher les chaînes alimentaires\n");
        printf("7. Degradation\n");
        printf("8. Predation\n");
        printf("9.Estimation de l'importance relative d'une espèce au sein d'un reseau et simulation de la disparition d'une espece.\n");
        printf("10. Quitter\n");
        printf("Votre choix : ");
        int choix;
        scanf("%d", &choix);

        if (choix == 10) {
            printf("Au revoir !\n");
            break;
        }

        char filename[100];
        if (choix >= 1 && choix <= 8) {
            printf("Choisissez un réseau trophique à analyser :\n");
            printf("1. Foret\n2. Marin\n3. Mangrove\n");
            printf("Entrez votre choix : ");
            int reseau;
            scanf("%d", &reseau);

            switch (reseau) {
                case 1:
                    strcpy(filename, "C:\\Users\\gatsi\\Documents\\GitHub\\Projet-TDG\\foret.txt");
                    break;
                case 2:
                    strcpy(filename, "C:\\Users\\gatsi\\Documents\\GitHub\\Projet-TDG\\marin.txt");
                    break;
                case 3:
                    strcpy(filename, "C:\\Users\\gatsi\\Documents\\GitHub\\Projet-TDG\\mangrove.txt");
                    break;
                default:
                    printf("Choix invalide.\n");
                    continue;
            }

            if (!read_graph(filename, &graph)) {
                printf("Erreur lors du chargement du graphe.\n");
                continue;
            }
            display_graph(&graph);
        }

        if (choix == 2) {
            for (int i = 0; i < graph.node_count; i++) {
                display_predecessors(&graph, i);
            }
        }

        if (choix == 3) {
            int components[MAX_NODES];
            int connected = is_connected(&graph, components);
            if (connected) {
                printf("Le réseau est connexe.\n");
            } else {
                printf("Le réseau n'est pas connexe. Composantes connexes :\n");
                for (int i = 0; i < graph.node_count; i++) {
                    printf("Sommet %s dans la composante %d\n", graph.node_names[i], components[i]);
                }
            }
        }

        if (choix == 4) {
            find_special_nodes(&graph);
        }

        if (choix == 5) {
            int levels[MAX_NODES];
            calculate_trophic_levels(&graph, levels);
            for (int i = 0; i < graph.node_count; i++) {
                printf("Niveau trophique de %s : %d\n", graph.node_names[i], levels[i]);
            }
        }

        if (choix == 6) {
            int sommet;
            printf("Entrez le sommet pour afficher ses chaînes alimentaires : ");
            scanf("%d", &sommet);
            display_successors(&graph, sommet - 1);
        }

        if (choix == 7) {
            // Code pour la dégradation
        }

        if (choix == 8) {
            // Code pour la prédation
        }

        if (choix == 9) {
            estimate_species_importance(&graph);
            char species_name[50];
            printf("Entrez le nom d'une espèce à supprimer : ");
            scanf("%s", species_name);
            simulate_extinction(&graph, species_name);
            estimate_species_importance(&graph);
        }
    }
    return 0;
}
