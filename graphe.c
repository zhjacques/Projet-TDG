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
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
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

    for (int i = 0; i < graph->node_count; i++) {
        fscanf(file, "%f %f %f %f %f %f",
               &graph->node_properties[i].biomasse_kg,
               &graph->node_properties[i].flux_W,
               &graph->node_properties[i].co2_emis,
               &graph->node_properties[i].co2_absorbe,
               &graph->node_properties[i].capacite_portage,
               &graph->node_properties[i].degradation_habitat);
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

    // Initialize parameters
    for (int i = 0; i < graph->node_count; i++) {
        populations[i] = initial_populations[i];
        growth_rates[i] = 0.1; // Default growth rate
        carrying_capacities[i] = graph->node_properties[i].capacite_portage;
        for (int j = 0; j < graph->node_count; j++) {
            predation[i][j] = 0.0; // Initialize predation matrix
        }
    }

    // Simulation over multiple time steps
    printf("\n--- Simulation Dynamique ---\n");
    printf("Temps\t");
    for (int i = 0; i < graph->node_count; i++) {
        printf("%s\t", graph->node_names[i]);
    }
    printf("\n");

    for (int t = 0; t < time_steps; t++) {
        printf("%d\t", t);
        for (int i = 0; i < graph->node_count; i++) {
            // Calculate predation loss
            double predation_loss = 0.0;
            for (int j = 0; j < graph->node_count; j++) {
                if (graph->adjacency_matrix[j][i]) {
                    predation_loss += predation[j][i] * populations[j];
                }
            }

            // Update population with growth and predation
            double growth = growth_rates[i] * populations[i] * (1 - ((double)populations[i] / carrying_capacities[i]));
            double habitat_impact = graph->node_properties[i].degradation_habitat;
            populations[i] += (int)(growth - predation_loss - habitat_impact);

            // Prevent negative populations
            if (populations[i] < 0) populations[i] = 0;

            printf("%d\t", populations[i]);
        }
        printf("\n");
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
void estimate_species_importance(Graph *graph) {
    for (int i = 0; i < graph->node_count; i++) {
        int degree = get_node_degree(graph, i);
        printf("Importance de %s : %d\n", graph->node_names[i], degree);
    }
}

// Fonction récursive pour imprimer les ancêtres d'un nœud
void print_ancestors(const Graph *graph, int node, int visited[], int level) {
    visited[node] = 1;
    for (int i = 0; i < graph->node_count; i++) {
        if (graph->adjacency_matrix[i][node] && !visited[i]) {
            for (int j = 0; j < level; j++) {
                printf("  ");
            }
            printf("%s\n", graph->node_names[i]);
            print_ancestors(graph, i, visited, level + 1);
        }
    }
    visited[node] = 0;
}

// Fonction pour afficher les chaînes alimentaires dont une espèce dépend
void display_food_chains(const Graph *graph, const char *species_name) {
    int node = -1;
    for (int i = 0; i < graph->node_count; i++) {
        if (strcmp(graph->node_names[i], species_name) == 0) {
            node = i;
            break;
        }
    }

    if (node == -1) {
        printf("Espèce %s introuvable dans le graphe.\n", species_name);
        return;
    }

    printf("\n--- Chaînes alimentaires pour %s ---\n", species_name);
    int visited[MAX_NODES] = {0};
    print_ancestors(graph, node, visited, 0);
}




void appliquer_degradation(Graph *graph, float facteur_degradation)
{
    for (int i = 0; i < graph->node_count; i++)
    {
        graph->node_properties[i].biomasse_kg *= (1 - facteur_degradation);
        graph->node_properties[i].flux_W *= (1 - facteur_degradation);
        graph->node_properties[i].co2_emis *= (1 - facteur_degradation);
        graph->node_properties[i].co2_absorbe *= (1 - facteur_degradation);
        graph->node_properties[i].capacite_portage *= (1 - facteur_degradation);

        // Affichage des propriétés mises à jour
        printf("Node %d (%s):\n", i, graph->node_names[i]);
        printf("  Biomasse (kg): %.2f\n", graph->node_properties[i].biomasse_kg);
        printf("  Flux (W): %.2f\n", graph->node_properties[i].flux_W);
        printf("  CO2 emis: %.2f\n", graph->node_properties[i].co2_emis);
        printf("  CO2 absorbe: %.2f\n", graph->node_properties[i].co2_absorbe);
        printf("  Capacite de portage: %.2f\n\n", graph->node_properties[i].capacite_portage);
    }
}



void predation_consommation(Graph *graph, float facteur_predation)
{
    for (int i = 0; i < graph->node_count; i++)  // Pour chaque sommet
    {
        if (graph->node_properties[i].flux_W > 0)  // Si le sommet i est un consommateur (ayant un flux positif)
        {
            for (int j = 0; j < graph->node_count; j++)  // Parcourir tous les voisins
            {
                if (graph->adjacency_matrix[i][j] != 0)  // Si i et j sont connectés
                {
                    float reduction_biomasse = graph->node_properties[i].flux_W * facteur_predation;  // Calcul de la réduction
                    graph->node_properties[j].biomasse_kg -= reduction_biomasse;  // Réduire la biomasse du voisin

                    // Éviter des valeurs négatives pour la biomasse
                    if (graph->node_properties[j].biomasse_kg < 0)
                    {
                        graph->node_properties[j].biomasse_kg = 0;
                    }
                }
            }
        }
    }
}



