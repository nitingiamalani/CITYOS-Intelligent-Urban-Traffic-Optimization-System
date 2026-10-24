#include <stdio.h>

#define N 6
#define INF 999999

char *city[N] = {
    "College", "Hospital", "Airport",
    "Railway", "Mall", "Bus Station"
};

int graph[N][N] = {
    {0, 4, 0, 2, 0, 0},
    {4, 0, 5, 1, 0, 0},
    {0, 5, 0, 0, 3, 0},
    {2, 1, 0, 0, 6, 7},
    {0, 0, 3, 6, 0, 2},
    {0, 0, 0, 7, 2, 0}
};

int visited[N];

void BFS(int start)
{
    int q[N], front = 0, rear = 0, i;
    for (i = 0; i < N; i++)
        visited[i] = 0;

    q[rear++] = start;
    visited[start] = 1;

    printf("BFS: ");

    while (front < rear)
    {
        int u = q[front++];
        printf("%s ", city[u]);

        for (i = 0; i < N; i++)
        {
            if (graph[u][i] > 0 && !visited[i])
            {
                visited[i] = 1;
                q[rear++] = i;
            }
        }
    }

    printf("\n");
}

void DFS(int u)
{
    int i;
    visited[u] = 1;
    printf("%s ", city[u]);

    for (i = 0; i < N; i++)
    {
        if (graph[u][i] > 0 && !visited[i])
            DFS(i);
    }
}

void shortestPath(int start, int end)
{
    int dist[N], prev[N], used[N] = {0};
    int i, j, u, path[N], count = 0;

    for (i = 0; i < N; i++)
    {
        dist[i] = INF;
        prev[i] = -1;
    }

    dist[start] = 0;

    for (i = 0; i < N; i++)
    {
        u = -1;

        for (j = 0; j < N; j++)
        {
            if (!used[j] &&
                (u == -1 || dist[j] < dist[u]))
                u = j;
        }

        if (u == -1 || dist[u] == INF)
            break;

        used[u] = 1;

        for (j = 0; j < N; j++)
        {
            if (graph[u][j] > 0 &&
                dist[u] + graph[u][j] < dist[j])
            {
                dist[j] = dist[u] + graph[u][j];
                prev[j] = u;
            }
        }
    }

    if (dist[end] == INF)
    {
        printf("No route found.\n");
        return;
    }

    for (i = end; i != -1; i = prev[i])
        path[count++] = i;

    printf("Shortest route: ");

    for (i = count - 1; i >= 0; i--)
    {
        printf("%s", city[path[i]]);

        if (i > 0)
            printf(" -> ");
    }

    printf("\nTotal distance: %d km\n", dist[end]);
}

int main(void)
{
    int choice, i, start, end;

    do
    {
        printf("\n=== CITY ROUTE FINDER ===\n");
        printf("1. Display locations\n");
        printf("2. BFS traversal\n");
        printf("3. DFS traversal\n");
        printf("4. Find shortest route\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
            return 0;

        switch (choice)
        {
            case 1:
                for (i = 0; i < N; i++)
                    printf("%d. %s\n", i, city[i]);
                break;

            case 2:
            case 3:
            case 4:
                printf("Enter start location (0-5): ");
                scanf("%d", &start);

                if (start < 0 || start >= N)
                {
                    printf("Invalid location.\n");
                    break;
                }

                if (choice == 2)
                    BFS(start);

                else if (choice == 3)
                {
                    for (i = 0; i < N; i++)
                        visited[i] = 0;

                    printf("DFS: ");
                    DFS(start);
                    printf("\n");
                }

                else
                {
                    printf("Enter destination (0-5): ");
                    scanf("%d", &end);

                    if (end < 0 || end >= N)
                        printf("Invalid destination.\n");
                    else
                        shortestPath(start, end);
                }

                break;

            case 5:
                printf("Thank you!\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 5);

    return 0;
}
