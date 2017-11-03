/*
* CSCI406 Algorithms
*
* Project3: Dynamic Programming
*
* Authors:  Victoria (Tori) Girkins
*           Stacia Near
*           Clara Tran
*/

#include <iostream>
#include <fstream>
#include "sstream"
#include <string>
#include <queue>
#include <string>
using namespace std;

// The ith entry of x is the amount of data that will be available
// for processing on day (i + 1). Note zero-indexed array but one-indexed
// days, hence all of the x[j - 1] instead of x[j] later in the code.
//int x[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

// might change from type array to vectors?
// could do dynamic array but I figured vectors would be easier to work with
vector<int> x1;

// The ith entry of s is the maximum amount of data we will be able
// to process i days after a reboot. Note again discrepancy in indexing.
// No data may be processed the day of a reboot.
//int s[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

vector<int> s1;

// Arrays from testing the function originally
int x[] = { 20, 80, 20, 60, 20, 60, 80, 10, 40, 10 };

int s[] = { 100, 90, 50, 45, 40, 35, 20, 15, 10, 5 };

// For convenience, a seperate variable denoting the number of days.
const int n = size(x);

/**
* This function returns the maximum amount of data which can
* be processed on the remaining days if the system was rebooted
* on day k. optDataTable is used to store intermediate results.
* TODO: Have it populate a table with this value and with next reboot.
*/
// To keep indices straight, k is ALWAYS treated as the actual day we are on.
// Adjust other indices as needed.
int OptData(int k, int optDataTable[2][n + 1]) {
    // The dynamic programming step
    if (optDataTable[0][k] != NULL) {
        return optDataTable[0][k];
    }
    if (k == n - 2) {
        // Base case; rebooted on third to last day. We must decide whether or not
        // to reboot tomorrow. We never reboot on the last day.
        int doReboot = min(x[n - 1], s[0]);
        int notReboot = min(x[n - 2], s[0]) + min(x[n - 1], s[1]);
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
            }
            // Add on the data we process after the reboot (optimize)
            data += OptData(i, optDataTable);
            // Check for a new max
            if (data > maxData) {
                maxData = data;
                nextReboot = i;
            }
        }
        // We may be better off not rebooting any more. Try this case.
        
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

void loadDaysInputs(string filename) {
    ifstream daysInput;
    daysInput.open(filename);

    //checking if file exists to open
    if (!daysInput) {
        cerr << "Unable to open file!" << endl
            << "Exiting now" << endl;
        exit(1);
    }

    int tempIndex = 0;
    string line;
    getline(daysInput, line);
    istringstream xInputs(line);

    // ideally format of input file should be only two lines:
    // first line containing the x values, second containing the s values
    // first while loop will read in the x values and store it into the x vector appropriately
    while (!xInputs.eof()) {
        xInputs >> line;
        x1.push_back(stoi(line));
        tempIndex++;
    }

    // here is where the s inputs are being stored into the s vector
    tempIndex = 0;
    getline(daysInput, line);
    istringstream sInputs(line);
    while (!sInputs.eof()) {
        sInputs >> line;
        s1.push_back(stoi(line));
        tempIndex++;
    }

    // I was coding this late so I didn't want to change the array setup
    // I tried to change it to a vector, but an error was thrown so I did 
    // this dumb round-about way to store the values....
    // Don't look at it, it's terrible..!
    x[size(x1)];
    s[size(s1)];
    for (unsigned int i = 0; i < x1.size(); ++i) {
        x[i] = x1[i];
        //cout << x1[i] << " ";
        //cout << x[i] << " ";
    }

    for (unsigned int i = 0; i < s1.size(); ++i) {
        s[i] = s1[i];
        //cout << s1[i] << " ";
        //cout << s1[i] << " ";
    }

    // Important to close input file!
    daysInput.close();
}

void outputResults(int amountProcessed, vector<int> traceback) {
    ofstream tbResults;
    tbResults.open("DP_results.txt");

    // amount of data processed is stored on first line
    tbResults << amountProcessed << endl;

    // amount of data processed each day is saved on the second
    // line with a space in between
    for (int tb : traceback) {
        tbResults << tb << " ";
    }
}

/*
* FINALLY GOT IT TO WORK.. MAYBE
* Testing with other values
* Will double check with group members
*/
vector<int> traceback(int indexRow[2][n + 1]) {
    vector<int> rebootDays;
    bool hasRebooted = false;
    int sIndex = 0;
    int tableIndex = 0;

    for (int i = 0; i < size(x); ++i) {
        // i + 1 is taking into account that days start on day 1
        if ((i + 1) != indexRow[1][tableIndex]) {
            if (x[i] < s[sIndex]) {
                rebootDays.push_back(x[i]);
                sIndex++;
            }
            else if (s[sIndex] < x[i]) {
                rebootDays.push_back(s[sIndex]);
                sIndex++;
            }
            // condition when x and s values are equal
            else {
                rebootDays.push_back(x[i]);
            }
        }
        else {
            // resets sIndex to start back to "day 1" for s
            sIndex = 0;
            tableIndex++;
            rebootDays.push_back(0);
        }
   }

    return rebootDays;
}

/*
* TO DO: figure out what to do for n. If we don't want n to be "hardcoded"
* and based on the size of the x from the input file, we have to find another
* way to define the constant int n. I did this late after other hw so my brain
* doesn't have enough energy for this at the moment... so n is left as is for now
*/

int main() {
    // opens the text file containing x and s values to read in
    loadDaysInputs("daysInput.txt");

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

    int dataProcessed = OptData(0, table);
    cout << dataProcessed << endl;
    /*
    for (int i = 0; i < n + 1; ++i) {
        cout << table[0][i] << " ";
    }
    cout << endl << endl;
    for (int i = 0; i < n + 1; i++) {
        cout << ((table[1][i] == NULL) ? "None" : to_string(table[1][i])) << " ";
    }
    cout << endl;
    */
    vector<int> rebootAnswers = traceback(table);
    // printing answers to make sure output function stores correct values
    for (int i = 0; i < rebootAnswers.size(); ++i) {
        cout << rebootAnswers.at(i) << " ";
    }
    cout << endl;

    // calls function to output the results into a text file as part of requirements
    outputResults(dataProcessed, rebootAnswers);

    return 0;
}