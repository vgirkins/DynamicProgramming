#include <iostream>
#include <queue>
#include <string>
using namespace std;

// The ith entry of x is the amount of data that will be available
// for processing on day (i + 1). Note zero-indexed array but one-indexed
// days, hence all of the x[j - 1] instead of x[j] later in the code.
int x[] = { 20, 80, 20, 60, 20, 60, 80, 10, 40, 10 };
// The ith entry of s is the maximum amount of data we will be able
// to process i days after a reboot. Note again discrepancy in indexing.
// No data may be processed the day of a reboot.
int s[] = { 100, 90, 50, 45, 40, 35, 20, 15, 10, 5 };
// For convenience, a seperate variable denoting the number of days.
const int n = 10;

int partialDayPoints;

/**
* This function returns the maximum amount of data which can
* be processed on the remaining days if the system was rebooted
* on day k. optDataTable is used to store intermediate results.
* TODO: Have it populate a table with this value and with next reboot.
*/
// To keep indices straight, k is ALWAYS treated as the actual day we are on.
// Adjust other indices as needed.
int OptData(int k, int optDataTable[2][n + 1], int partial) {
    // The dynamic programming step
    if (optDataTable[0][k] != NULL) {
        return optDataTable[0][k];
    }
    if (k == n - 2) {
        // Base case; rebooted on third to last day. We must decide whether or not
        // to reboot tomorrow. We never reboot on the last day.
        int doReboot = min(x[n - 1], s[0]);
        int notReboot = min(x[n - 2], s[0]) + min(x[n - 1], s[1]);
        partial++;
        if (doReboot > notReboot) {
            optDataTable[0][k] = doReboot;
            optDataTable[1][k] = k + 1;     // We decided to reboot tomorrow
            return doReboot;
        }
        else {
            optDataTable[0][k] = notReboot;
            optDataTable[1][k] = NULL;     // We decided not to reboot tomorrow
            return notReboot;
        }
    }
    else {
        int maxData = 0;
        // For every possible day i for a next reboot
        int nextReboot;  // Must be in scope later; this will be the day on which to next reboot
        for (int i = k + 1; i < n - 1; i++) {
            int data = 0;
            // Add up the data we process every day before the reboot
            for (int j = k + 1; j < i; j++) {
                data += min(x[j - 1], s[j - k - 1]);
                partial += k;
            }
            // Add on the data we process after the reboot (optimize)
            data += OptData(i, optDataTable, partial);
            // Check for a new max
            if (data > maxData) {
                partial = 0;
                maxData = data;
                nextReboot = i;
            }
        }
        // We may be better off not rebooting any more. Try this case.
        // TODO could code this in at the beginning somehow with an "if"
        
        int noReboot = 0;
        // The amount of data we'll process is a simple sum of minimums.
        for (int j = k + 1; j <= n; j++) {
            noReboot += min(x[j - 1], s[j - k - 1]);
        }
        if (noReboot > maxData) {
            // It's best not to reboot any more. 
            maxData = noReboot;
            nextReboot = NULL;
        }
        // Add max data processed and next reboot day to table
        optDataTable[0][k] = maxData;
        optDataTable[1][k] = nextReboot;
        return maxData;
    }
}

int main() {

    partialDayPoints = size(x);
    // The columns of this table are days. Day 1 is at index 1. Index 0 included to represent
    // the fact that we reboot our system before beginning.
    // The first row of this table is the most data you can process given you rebooted on day i
    // The second row is the next day on which you should reboot, following an optimal strategy 
    int table[2][n + 1]; 
    for (int i = 0; i < n; i++) {
        table[0][i] = NULL;
    }
    table[0][n] = 0;                        // If reboot on last day, cannot process any more data
    table[1][n] = NULL;                     // Once you're on the last day you can't reboot any more
    table[0][n - 1] = min(x[n - 1], s[0]);  // If reboot on second to last day, this is how much data you can process
    table[1][n - 1] = NULL;                 // Once you're on the second-to-last day you shouldn't reboot any more

    cout << OptData(0, table, partialDayPoints) << endl;

    for (int i = 0; i < n + 1; i++) {
        cout << table[0][i] << " ";
    }
    cout << endl << endl;
    for (int i = 0; i < n + 1; i++) {
        cout << ((table[1][i] == NULL) ? "None" : to_string(table[1][i])) << " ";
    }
    cout << endl;
    return 0;
}