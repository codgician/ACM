#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm>
#include <cmath>
#include <climits>
#include <iomanip>
#define SIZE 101
#define PI 3.14159
using namespace std;

typedef struct _Node {
    int x;
    int y;
} node;

node arr[SIZE];

int getCrossedProduct(node a, node b)
{
    return a.x * b.y - b.x * a.y;
}

double getDistance(node a, node b)
{
    return sqrt((double)((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

node nodeMinus(node a, node b)
{
    node ans;
    ans.x = a.x - b.x;
    ans.y = a.y - b.y;
    return ans;
}

bool isLeft(node a, node b, node c)
{
    return getCrossedProduct(nodeMinus(b, a), nodeMinus(c, a)) > 0;
}

bool cmpRule(node a, node b)
{
    int crossedProduct = getCrossedProduct(nodeMinus(a, arr[0]), nodeMinus(b, arr[0]));
    if (crossedProduct < 0)
        return false;
    else if (crossedProduct == 0 && getDistance(a, arr[0]) < getDistance(b, arr[0]))
        return false;
    else
        return true;
}

// Maintain a stack.
node graham[SIZE];
int arrPt = 0;

void push(node n)
{
    graham[arrPt] = n;
    arrPt++;
}

void pop()
{
    arrPt--;
}

int main()
{
    ios::sync_with_stdio(false);
    int caseNum;
    scanf("%d", &caseNum);
    for (int t = 0; t < caseNum; t++)
    {
        int nodeNum, R;
        scanf("%d%d", &nodeNum, &R);
        double ans = 0;
        if (nodeNum == 1)
        {
            int x, y;
            scanf("%d%d", &x, &y);
            ans = 0;
        }
        else if (nodeNum == 2)
        {
            node a, b;
            scanf("%d%d%d%d", &a.x, &a.y, &b.x, &b.y);
            ans = getDistance(a, b);
        }
        else
        {
            // Find the point with the minimum y coordinate.
            int lowestNodeIndex = 0;
            node lowestNode;
            lowestNode.y = INT_MAX;
            for (int i = 0; i < nodeNum; i++)
            {
                scanf("%d%d", &arr[i].x, &arr[i].y);
                if (arr[i].y < lowestNode.y || (arr[i].y == lowestNode.y && arr[i].x < lowestNode.x))
                {
                    lowestNodeIndex = i;
                    lowestNode = arr[i];
                }
            }
            swap(arr[0], arr[lowestNodeIndex]);

            // Sort it!
            sort(arr + 1, arr + nodeNum, cmpRule);

            arrPt = 0;
            push(arr[0]);
            push(arr[1]);
            push(arr[2]);

            for (int i = 3; i < nodeNum; i++)
            {
                while (!isLeft(graham[arrPt - 2], graham[arrPt - 1], arr[i]) && arrPt > 2)
                {
                    pop();
                }
                push(arr[i]);
            }

            for (int i = 0; i < arrPt - 1; i++)
            {
                ans += getDistance(graham[i], graham[i + 1]);
            }
            ans += getDistance(graham[0], graham[arrPt - 1]);
        }
        ans += PI * R * 2;
        printf("%.0f\n", ans);
        if (t < caseNum - 1)
            printf("\n");
    }
    return 0;
}
