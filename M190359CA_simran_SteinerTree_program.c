//
//  main.c
//  Steiner Tree
//
//  Created by Simran Gangwani on 02/04/21.
//

//#include "steiner.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define INF 99999

void printMatrix(int r,int c, int **matrix)
{
    int i,j;
    for(i = 0; i < r; i++)
    {
        for(j = 0; j < c; j++)
        {
            if(matrix[i][j] == INF)
                printf("%6s", "INF");
            else
                printf("%6d",matrix[i][j]);
        }
       
        printf("\n");
    }
}

void dfs(int start,int v, int *visited, int **graph)
{
    visited[start] = 1;
    
    for(int i = 0; i < v; i++)
    {
        if((graph[start][i] != 0 && graph[start][i] != INF) && visited[i] == 0)
            dfs(i,v,visited,graph);
    }
}

int** floydWarshall( int v, int **graph)
{
    int **dist, i, j, k;
    dist = (int **)malloc(v * sizeof(int *));
    for(i = 0; i < v; i++)
        dist[i] = (int *)malloc(v * sizeof(int));
    for(i = 0; i < v; i++)
        for(j = 0; j < v; j++)
            dist[i][j] = graph[i][j];
    
    for (k = 0; k < v; k++)
            for (i = 0; i < v; i++)
                for (j = 0; j < v; j++)
                    if (dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];
                
   
    return dist;
}

void steinerTree(int size,int **dist,int terminal_size,int *terminal)
{
    int vertices_size = size - terminal_size;
    int u,v,x = 0,d,d1;
    
    //---------------Create DP Table----------------------
    int subset_size = pow(2,terminal_size);
    int **dp, i;
    dp = (int **)malloc(subset_size * sizeof(int *));
    for(i = 0; i < subset_size; i++)
        dp[i] = (int *)malloc(vertices_size * sizeof(int));
    //initially fill the table with INF
    for(u = 0; u < subset_size; u++)
        for(v = 0; v < vertices_size; v++)
            dp[u][v] = INF;
            
    
   // base case if |D| = 1, then a steiner tree of minimum weight for D+{v} = {v,D(V)}
   x = terminal_size - 1;
    for(u = 0; u < terminal_size; u++)
    {
        for(v = 0; v < vertices_size; v++)
        {
            //printf("%d %d %d\n",terminal[x],v,dist[terminal[x]][v]) ;
            dp[1<<u][v] = dist[terminal[x]][v];
        }
        x--;
    }
    
   int path;
    
    for(d = 0; d < subset_size; d++)
    {
        for(v = 0; v < vertices_size; v++)
        {
            for(d1 = d; d1 > 0; d1 = (d1 - 1)&d)
            {
                for(u = 0; u < vertices_size; u++)
                {
                    path = dist[v][u];
                    if(dp[d][v] > (dp[d1][u] + dp[d-d1][u] + path))
                        dp[d][v] = dp[d1][u] + dp[d-d1][u] + path;
                }
            }
        }
    }
    
   
    printf("\n-------------------------------DP Table-------------------------------\n");
    printMatrix(subset_size,vertices_size,dp);
    int min_cost = INF;
    for(u = 0; u < vertices_size; u++)
    {
        if(dp[subset_size - 1][u] < min_cost)
        min_cost = dp[subset_size - 1][u];
    }
    printf("Cost of Minimum Steiner Tree = %d\n",(min_cost - terminal_size));
}

int main(int argc, const char * argv[]) {
    int m,n,size,terminal_size;
    printf("Enter number of vertices and edges in the graph : ");
    scanf("%d%d",&m,&n);
    size = m;
    int **graph; //adjacency matrix
    graph = (int **)malloc(size * sizeof(int *));
    for(int i = 0; i < size; i++)
        graph[i] = (int *)malloc(size * sizeof(int));
    
    
    //----------------reprsent graph using adjacency matrix------------------------
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }
    //input graph
    int u,v,w;
    printf("Enter vertices forming edges and their respective weight \n");
    for(int i = 0; i < n; i++)
    {
        scanf("%d%d%d",&u,&v,&w);
        graph[u][v] = w;
        graph[v][u] = w;
    }
    //input terminal vertices
    int **dist;
    printf("Enter number of Terminal verlices : ");
    scanf("%d",&terminal_size);
    int terminal[terminal_size];
    printf("Enter Terminal Vertices : ");
    for(int i = 0; i < terminal_size; i++)
    scanf("%d",&terminal[i]);
    
    
    //Graph reprsentation using Adjacency matrix
    printf("\nOur Graph is created!!!");
    printf("\n----------------------------Adjacency Matrix--------------------------\n");
    printMatrix(m, m, graph);
    
    printf("\nNow we will check for our 3 assupmtions\n");
    //check for assumption 1, k > 1
    if(terminal_size < 1)
    {
        printf("Number of terminal vertices should be > 1\n Assumption 1 failed!!!\n");
        exit(0);
    }
    printf("\nAssumption 1 passed!!!\n i.e k > 1\n");
    
    
    //check for assumption 2, Graph should be connected
    //We will check this using DFS Traversal
    int *visited;
    visited = (int *)malloc(sizeof(int) * m);
    for(int i = 0; i < m; i++)
    visited[i] = 0;
    dfs(0,m,visited,graph);
    
    for(int i = 0; i < m; i++)
    {
        if(visited[i] != 1)
        {
            printf("\nGraph is not connected\nAssumption 2 failed!!!\n");
            exit(0);
        }
    }
    
    printf("\nAssumption 2 passed!!!\n i.e our graph is connected\n");

    //Pre Processing step 3
    graph = (int **)realloc(graph, (m + terminal_size) * sizeof(int *));
    for(int i = m; i < (m + terminal_size); i++)
    graph[i] = NULL;
    for(int i = 0; i < (m + terminal_size); i++)
        graph[i] = (int *)realloc(graph[i], (m + terminal_size) * sizeof(int));
    
    for(int i = 0; i < (m); i++)
    for(int j = m; j < (m + terminal_size); j++)
    graph[i][j] = INF;
    
    for(int i = m ; i < (m + terminal_size); i++)
    {
        for(int j = 0; j < (m + terminal_size); j++)
        {
            if(i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }
    
    for(int i = m, j = 0; j < terminal_size; i++,j++)
    {
        graph[i][terminal[j]] = 1;
        graph[terminal[j]][i] = 1;
    }
    printMatrix(m + terminal_size, m + terminal_size, graph);
    
    printf("\nNow, all the terminal nodes have degree one and are not adjacent\n");
    
    printf("\nThe graph is ready for algorithm :)\n");
    
    size = m + terminal_size;
    //-------------------calculate All Pair Shortest Path------------------
    dist = floydWarshall(size , graph);
    printf("\n---------------------All Pair Shortest Path Matrix---------------------\n");
    printMatrix(size,size,dist);
    
    
    //-------------------calculate Steiner Tree----------------------------
    
    for(int i = 0; i < terminal_size; i++)
    terminal[i] = m++;
    steinerTree(size ,dist,terminal_size,terminal);
    return 0;
}
