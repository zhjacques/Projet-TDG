#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"
#include <limits.h>
#include <math.h>



// Lecture du graphe depuis un fichier
int read_graph(const char *filename, Graph *graph) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    fscanf(file, "%d", &graph->node_count);

    for (int i = 0; i < graph->node_count; i++) {
        fscanf(file, "%s", graph->node_names[i]);
    }

    for (int i = 0; i < graph->node_count; i++) {
        for (int j = 0; j < graph->node_count; j++) {
            fscanf(file, "%d", &graph->adjacency_matrix[i][j]);
        }
    }

    fclose(file);
    return 1;
}

// Affichage du graphe
void display_graph(const Graph *graph) {
    printf("\nGraphe (%d sommets) :\n", graph->node_count);
    for (int i = 0; i < graph->node_count; i++) {
        printf("%d: %s\n", i + 1, graph->node_names[i]);
    }

    printf("\nMatrice d'adjacence :\n");
    for (int i = 0; i < graph->node_count; i++) {
        for (int j = 0; j < graph->node_count; j++) {
            printf("%d ", graph->adjacency_matrix[i][j]);
        }
        printf("\n");
    }
}
// Fonction pour afficher les successeurs d'un sommet
void display_successors(const Graph *graph, int node) {
    printf("\nSuccesseurs de %s : ", graph->node_names[node]);
    int has_successors = 0;
    for (int j = 0; j < graph->node_count; j++) {
        if (graph->adjacency_matrix[node][j]) {
            printf("%s ", graph->node_names[j]);
            has_successors = 1;
        }
    }
    if (!has_successors) {
        printf("Aucun");
    }
    printf("\n");
}

// Fonction pour afficher les prédécesseurs d'un sommet
void display_predecessors(const Graph *graph, int node) {
    printf("\nPredecesseurs de %s : ", graph->node_names[node]);
    int has_predecessors = 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacency_matrix[i][node]) {
            printf("%s ", graph->node_names[i]);
            has_predecessors = 1;
        }
    }
    if (!has_predecessors) {
        printf("Aucun");
    }
    printf("\n");
}

// Affiche le contenu d'un fichier DOT
void afficher_dot(const char *dot_filename) {
    FILE *file = fopen(dot_filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier DOT");
        return;
    }

    printf("\n--- Contenu du fichier DOT : %s ---\n", dot_filename);

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); // Affiche chaque ligne du fichier DOT
    }

    fclose(file);
    printf("\n--- Fin du fichier DOT ---\n");
}


// Vérification de la connexité et identification des composantes connexes
int is_connected(const Graph *graph, int components[]) {
    int visited[MAX_NODES] = {0};
    int component_id = 0;

    for (int i = 0; i < graph->node_count; i++) {
        if (!visited[i]) {
            dfs(graph, i, visited, component_id, components);
            component_id++;
        }
    }

    return component_id == 1; // Retourne vrai si le graphe est connexe
}

// DFS modifié pour marquer les composantes connexes
void dfs(const Graph *graph, int node, int visited[], int component_id, int components[]) {
    visited[node] = 1;
    components[node] = component_id;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacency_matrix[node][i] && !visited[i]) {
            dfs(graph, i, visited, component_id, components);
        }
    }
}

// Recherche de producteurs primaires, prédateurs apex, etc.
void find_special_nodes(const Graph *graph) {
    printf("\n--- Recherche de sommets particuliers ---\n");

    printf("\nProducteurs primaires (sans predecesseurs) : ");
    for (int i = 0; i < graph->node_count; i++) {
        int has_predecessors = 0;
        for (int j = 0; j < graph->node_count; j++) {
            if (graph->adjacency_matrix[j][i]) {
                has_predecessors = 1;
                break;
            }
        }
        if (!has_predecessors) {
            printf("%s ", graph->node_names[i]);
        }
    }

    printf("\n\nPredateurs apex (sans successeurs) : ");
    for (int i = 0; i < graph->node_count; i++) {
        int has_successors = 0;
        for (int j = 0; j < graph->node_count; j++) {
            if (graph->adjacency_matrix[i][j]) {
                has_successors = 1;
                break;
            }
        }
        if (!has_successors) {
            printf("%s ", graph->node_names[i]);
        }
    }

    printf("\n\nEspeces avec une seule source d'alimentation : ");
    for (int i = 0; i < graph->node_count; i++) {
        int count_predecessors = 0;
        for (int j = 0; j < graph->node_count; j++) {
            if (graph->adjacency_matrix[j][i]) {
                count_predecessors++;
            }
        }
        if (count_predecessors == 1) {
            printf("%s ", graph->node_names[i]);
        }
    }
    printf("\n");
}

// Calcul des niveaux trophiques
void calculate_trophic_levels(const Graph *graph, int levels[]) {
    for (int i = 0; i < graph->node_count; i++) {
        levels[i] = 0; // Initialiser tous les niveaux à 0
    }

    // Identifier les producteurs primaires (niveaux trophiques = 1)
    for (int i = 0; i < graph->node_count; i++) {
        int has_predecessors = 0;
        for (int j = 0; j < graph->node_count; j++) {
            if (graph->adjacency_matrix[j][i]) {
                has_predecessors = 1;
                break;
            }
        }
        if (!has_predecessors) {
            levels[i] = 1;
        }
    }

    // Calcul des niveaux trophiques pour les autres sommets
    int updated;
    do {
        updated = 0;
        for (int i = 0; i < graph->node_count; i++) {
            if (levels[i] == 0) { // Si le niveau n'est pas encore calculé
                int max_predecessor_level = 0;
                int can_calculate = 1;
                for (int j = 0; j < graph->node_count; j++) {
                    if (graph->adjacency_matrix[j][i]) {
                        if (levels[j] == 0) {
                            can_calculate = 0;
                            break;
                        }
                        if (levels[j] > max_predecessor_level) {
                            max_predecessor_level = levels[j];
                        }
                    }
                }
                if (can_calculate) {
                    levels[i] = max_predecessor_level + 1;
                    updated = 1;
                }
            }
        }
    } while (updated);
}

// Simulation de la disparition d'une espèce
void simulate_extinction(Graph *graph, const char *species_name) {
    int node = -1;
    for (int i = 0; i < graph->node_count; i++) {
        if (strcmp(graph->node_names[i], species_name) == 0) {
            node = i;
            break;
        }
    }

    if (node == -1) {
        printf("Espece %s introuvable dans le graphe.\n", species_name);
        return;
    }

    // Retirer les arcs associés à l'espèce
    for (int i = 0; i < graph->node_count; i++) {
        graph->adjacency_matrix[node][i] = 0; // Retirer arcs sortants
        graph->adjacency_matrix[i][node] = 0; // Retirer arcs entrants
    }

    printf("L'espece %s a ete supprimee du graphe.\n", species_name);
}

// Simulation de la dynamique des populations
void simulate_population_dynamics(Graph *graph, int initial_populations[], int time_steps) {
    int populations[MAX_NODES];
    double growth_rates[MAX_NODES];
    int carrying_capacities[MAX_NODES];
    double predation[MAX_NODES][MAX_NODES];

    // Initialiser les paramètres
    for (int i = 0; i < graph->node_count; i++) {
        populations[i] = initial_populations[i];
        growth_rates[i] = 0.1; // Par défaut, faible croissance
        carrying_capacities[i] = 100; // Par défaut, capacité de charge de 100
        for (int j = 0; j < graph->node_count; j++) {
            predation[i][j] = (graph->adjacency_matrix[i][j]) ? 0.01 : 0.0;
        }
    }

    // Simulation sur plusieurs étapes
    printf("\n--- Simulation Dynamique ---\n");
    printf("Temps\t");
    for (int i = 0; i < graph->node_count; i++) {
        printf("%s\t", graph->node_names[i]);
    }
    printf("\n");

    for (int t = 0; t < time_steps; t++) {
        printf("%d\t", t);
        for (int i = 0; i < graph->node_count; i++) {
            // Calcul de la prédation
            double predation_loss = 0.0;
            for (int j = 0; j < graph->node_count; j++) {
                if (graph->adjacency_matrix[i][j]) {
                    predation_loss += predation[i][j] * populations[i] * populations[j];
                }
            }

            // Mise à jour de la population
            double growth = growth_rates[i] * populations[i] * (1 - ((double)populations[i] / carrying_capacities[i]));
            populations[i] += (int)(growth - predation_loss);

            // Empêcher des populations négatives
            if (populations[i] < 0) populations[i] = 0;

            printf("%d\t", populations[i]);
        }
        printf("\n");
    }
}


void estimate_species_importance(Graph *graph) {
    for (int i = 0; i < graph->node_count; i++) {
        int degree = get_node_degree(graph, i);
        printf("Importance de %s : %d\n", graph->node_names[i], degree);
    }
}

int get_node_degree(Graph *graph, int node_index) {
// Assuming you have a way to calculate or access the degree of a node
// This is just a placeholder implementation
    int degree = 0;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacency_matrix[node_index][i] != 0) {
            degree++;
        }
    }

    return degree;
}
