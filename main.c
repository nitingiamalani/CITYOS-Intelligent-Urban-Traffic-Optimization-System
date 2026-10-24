#include <stdio.h>
#include <limits.h>
#include <string.h>

#define V 8
#define INF 1000000000

const char *places[V] = {
    "College", "Hospital", "Airport", "Railway Station",
    "Shopping Mall", "Bus Station", "Tech Park", "City Center"
};

/*
 * CITYOS: Intelligent Urban Traffic Optimization System
 * Data structures: adjacency matrix, arrays, DFS recursion
 * Algorithms: Dijkstra, Floyd-Warshall, Tarjan articulation points
 *
 * Road weights represent travel cost in minutes.
 * 0 means no direct road. INF is used internally for unreachable paths.
 */
int base[V][V] = {
    {0, 5, 0, 4, 0, 0, 6, 0},
    {5, 0, 7, 0, 3, 0, 0, 4},
    {0, 7, 0, 6, 0, 5, 0, 0},
    {4, 0, 6, 0, 2, 3, 0, 0},
    {0, 3, 0, 2, 0, 4, 5, 3},
    {0, 0, 5, 3, 4, 0, 2, 6},
    {6, 0, 0, 0, 5, 2, 0, 4},
    {0, 4, 0, 0, 3, 6, 4, 0}
};

int road[V][V];
int closed[V][V];
int traffic[V][V]; /* Extra cost added to the base travel time. */

void line(void)
{
    printf("------------------------------------------------------------\n");
}

void showPlaces(void)
{
    int i;
    line();
    printf("CITYOS CITY INTERSECTIONS\n");
    line();
    for (i = 0; i < V; i++)
        printf("%d. %s\n", i + 1, places[i]);
    line();
}

void showRoads(void)
{
    int i, j, found = 0;
    line();
    printf("%-20s %-20s %-12s %-12s\n", "FROM", "TO", "BASE (min)", "CURRENT (min)");
    line();

    for (i = 0; i < V; i++)
    {
        for (j = i + 1; j < V; j++)
        {
            if (base[i][j] > 0)
            {
                found = 1;
                printf("%-20s %-20s %-12d ", places[i], places[j], base[i][j]);
                if (closed[i][j])
                    printf("%-12s\n", "CLOSED");
                else
                    printf("%-12d\n", road[i][j] + traffic[i][j]);
            }
        }
    }
    if (!found)
        printf("No roads are configured.\n");
    line();
}

int readInt(const char *prompt, int low, int high)
{
    int value;
    char extra;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d%c", &value, &extra) == 2 && extra == '\n'
            && value >= low && value <= high)
            return value;

        printf("Invalid input. Enter a number from %d to %d.\n", low, high);
        while (getchar() != '\n')
            ;
    }
}

void resetTraffic(void)
{
    int i, j;
    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            traffic[i][j] = 0;
}

void dijkstra(int source, int target, int printPath)
{
    int dist[V], visited[V] = {0}, parent[V];
    int i, count, u = -1, v, min, path[V], length = 0;

    for (i = 0; i < V; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[source] = 0;

    for (count = 0; count < V; count++)
    {
        min = INF;
        u = -1;

        for (i = 0; i < V; i++)
        {
            if (!visited[i] && dist[i] < min)
            {
                min = dist[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;
        if (u == target)
            break;

        for (v = 0; v < V; v++)
        {
            if (u != v && base[u][v] > 0 && !closed[u][v])
            {
                int weight = road[u][v] + traffic[u][v];
                if (!visited[v] && dist[u] + weight < dist[v])
                {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    line();
    if (dist[target] == INF)
    {
        printf("NO ROUTE AVAILABLE between %s and %s.\n",
               places[source], places[target]);
        printf("Try reopening a road or selecting another destination.\n");
        line();
        return;
    }

    printf("SHORTEST ROUTE: %s -> %s\n", places[source], places[target]);
    printf("Estimated travel time: %d minutes\n", dist[target]);

    if (printPath)
    {
        v = target;
        while (v != -1 && length < V)
        {
            path[length++] = v;
            v = parent[v];
        }

        printf("Route: ");
        for (i = length - 1; i >= 0; i--)
        {
            printf("%s", places[path[i]]);
            if (i > 0)
                printf(" -> ");
        }
        printf("\n");
    }
    line();
}

void shortestRouteMenu(void)
{
    int s, d;
    showPlaces();
    s = readInt("Enter source number: ", 1, V) - 1;
    d = readInt("Enter destination number: ", 1, V) - 1;

    if (s == d)
    {
        printf("You are already at %s. Travel time: 0 minutes.\n", places[s]);
        return;
    }
    dijkstra(s, d, 1);
}

void setTraffic(void)
{
    int a, b, extra;
    showPlaces();
    a = readInt("Enter first intersection number: ", 1, V) - 1;
    b = readInt("Enter second intersection number: ", 1, V) - 1;

    if (a == b || base[a][b] == 0)
    {
        printf("No direct road exists between those intersections.\n");
        return;
    }
    if (closed[a][b])
    {
        printf("That road is closed. Reopen it before changing traffic.\n");
        return;
    }

    extra = readInt("Extra traffic delay in minutes (0-120): ", 0, 120);
    traffic[a][b] = traffic[b][a] = extra;
    road[a][b] = road[b][a] = base[a][b];

    printf("Traffic updated: %s <-> %s now takes %d minutes.\n",
           places[a], places[b], base[a][b] + extra);
    printf("Shortest routes will use the updated travel time.\n");
}

void closeRoad(void)
{
    int a, b;
    showPlaces();
    a = readInt("Enter first intersection number: ", 1, V) - 1;
    b = readInt("Enter second intersection number: ", 1, V) - 1;

    if (a == b || base[a][b] == 0)
    {
        printf("No direct road exists between those intersections.\n");
        return;
    }

    if (closed[a][b])
    {
        printf("That road is already closed.\n");
        return;
    }

    closed[a][b] = closed[b][a] = 1;
    printf("ROAD CLOSED: %s <-> %s\n", places[a], places[b]);
    printf("The route planner will avoid this road automatically.\n");
}

void reopenRoad(void)
{
    int a, b;
    showPlaces();
    a = readInt("Enter first intersection number: ", 1, V) - 1;
    b = readInt("Enter second intersection number: ", 1, V) - 1;

    if (a == b || base[a][b] == 0)
    {
        printf("No direct road exists between those intersections.\n");
        return;
    }

    if (!closed[a][b])
    {
        printf("That road is not currently closed.\n");
        return;
    }

    closed[a][b] = closed[b][a] = 0;
    printf("ROAD REOPENED: %s <-> %s\n", places[a], places[b]);
    printf("Current traffic delay on the road: %d minutes.\n", traffic[a][b]);
}

void allPairsShortestPaths(void)
{
    int dist[V][V];
    int i, j, k;

    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
        {
            if (i == j)
                dist[i][j] = 0;
            else if (base[i][j] > 0 && !closed[i][j])
                dist[i][j] = road[i][j] + traffic[i][j];
            else
                dist[i][j] = INF;
        }
    }

    for (k = 0; k < V; k++)
        for (i = 0; i < V; i++)
            for (j = 0; j < V; j++)
                if (dist[i][k] < INF && dist[k][j] < INF
                    && dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];

    line();
    printf("ALL-PAIRS SHORTEST TRAVEL TIMES (minutes)\n");
    printf("%-18s", "");
    for (j = 0; j < V; j++)
        printf("%-9d", j + 1);
    printf("\n");

    for (i = 0; i < V; i++)
    {
        printf("%-18s", places[i]);
        for (j = 0; j < V; j++)
        {
            if (dist[i][j] >= INF)
                printf("%-9s", "INF");
            else
                printf("%-9d", dist[i][j]);
        }
        printf("\n");
    }
    printf("Columns are intersection numbers shown in the list above.\n");
    line();
}

/* Tarjan articulation-point algorithm on the currently open road network. */
int disc[V], low[V], parent[V], articulation[V], timer;

void tarjanDFS(int u)
{
    int children = 0, v;
    disc[u] = low[u] = ++timer;

    for (v = 0; v < V; v++)
    {
        if (u == v || base[u][v] == 0 || closed[u][v])
            continue;

        if (disc[v] == 0)
        {
            children++;
            parent[v] = u;
            tarjanDFS(v);

            if (low[v] < low[u])
                low[u] = low[v];

            if (parent[u] == -1 && children > 1)
                articulation[u] = 1;

            if (parent[u] != -1 && low[v] >= disc[u])
                articulation[u] = 1;
        }
        else if (v != parent[u] && disc[v] < low[u])
        {
            low[u] = disc[v];
        }
    }
}

void criticalIntersections(void)
{
    int i, found = 0;

    for (i = 0; i < V; i++)
    {
        disc[i] = 0;
        low[i] = 0;
        parent[i] = -1;
        articulation[i] = 0;
    }
    timer = 0;

    for (i = 0; i < V; i++)
        if (disc[i] == 0)
            tarjanDFS(i);

    line();
    printf("CRITICAL INTERSECTIONS ANALYSIS\n");
    printf("These intersections can disconnect parts of the road network\n");
    printf("if the intersection or its connected roads become unavailable.\n\n");

    for (i = 0; i < V; i++)
    {
        if (articulation[i])
        {
            printf("- %s\n", places[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No articulation points found in the current open-road network.\n");

    line();
}

void resetNetwork(void)
{
    int i, j;
    for (i = 0; i < V; i++)
    {
        for (j = 0; j < V; j++)
        {
            road[i][j] = base[i][j];
            closed[i][j] = 0;
            traffic[i][j] = 0;
        }
    }
    printf("Network reset. All roads reopened and traffic delays cleared.\n");
}

void menu(void)
{
    line();
    printf(" CITYOS - INTELLIGENT URBAN TRAFFIC OPTIMIZATION SYSTEM\n");
    line();
    printf("1. Display city intersections\n");
    printf("2. Display road network and current travel times\n");
    printf("3. Find shortest route (Dijkstra)\n");
    printf("4. Simulate traffic congestion\n");
    printf("5. Close a road\n");
    printf("6. Reopen a road\n");
    printf("7. All-pairs shortest paths (Floyd-Warshall)\n");
    printf("8. Find critical intersections (Tarjan)\n");
    printf("9. Reset traffic and road closures\n");
    printf("0. Exit\n");
    line();
}

int main(void)
{
    int choice, i, j;

    for (i = 0; i < V; i++)
        for (j = 0; j < V; j++)
            road[i][j] = base[i][j];

    printf("\nWelcome to CITYOS!\n");
    printf("Urban road-network analysis and route optimization.\n");

    do
    {
        menu();
        choice = readInt("Choose an option: ", 0, 9);

        switch (choice)
        {
            case 1:
                showPlaces();
                break;
            case 2:
                showRoads();
                break;
            case 3:
                shortestRouteMenu();
                break;
            case 4:
                setTraffic();
                break;
            case 5:
                closeRoad();
                break;
            case 6:
                reopenRoad();
                break;
            case 7:
                allPairsShortestPaths();
                break;
            case 8:
                criticalIntersections();
                break;
            case 9:
                resetNetwork();
                break;
            case 0:
                printf("CITYOS shutting down. Travel safely!\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (choice != 0);

    return 0;
}
