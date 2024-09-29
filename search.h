/****************************
 * Project 2: Search Engine
 *
 * Course: CS 251, Fall 2023
 * System: WindowsOS using VSCode
 * Program Language: C++
 * Student Author: Zaid Khan Mohammed
 * *** **********************/

#pragma once
// Including libraries
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <sstream>
using namespace std;

// Function that takes a string and returns a "cleaned" token (without any leading and trailing punctuation, lowercase)
string cleanToken(string s)
{
    int start = -1;    // Stores index of first non-whitespace, non-punctuation character
    int end = -1;      // Stores index of last non-whitespace, non punctuation character
    bool hasAlpha = 0; // Flag to know if string contains at least one alphabet
    for (int i = 0; i < s.size(); ++i)
    {
        s[i] = tolower(s[i]); // Converting entire string to lowercase
        if (isalpha(s[i]))
        {
            hasAlpha = 1; // Updating flag if needed
        }
    }
    if (!hasAlpha)
    {
        return ""; // Return empty string if string has no alphabets (indicating the token is to be discarded)
    }
    for (int i = 0; i < s.size(); ++i)
    {
        if (!isspace(s[i]) && !ispunct(s[i]))
        {
            start = i; // Finding the starting index of "cleaned" token
            break;
        }
    }
    for (int i = 0; i < s.size(); ++i)
    {
        if (!isspace(s[i]) && !ispunct(s[i]))
        {
            end = i; // Finding the ending index of "cleaned" token
        }
    }
    s = s.substr(start, end - start + 1);
    if (start == -1 && end == -1)
    {
        return ""; // Returning empty string of the "cleaned" token contains no characters
    }
    return s;
}

// Function to create and return a set of "cleaned" tokens in a string
set<string> gatherTokens(string text)
{
    set<string> tokens;
    string token;
    stringstream strstream;
    strstream << text; // Using stringstream to obtain separate words
    while (strstream >> token)
    {
        token = cleanToken(token); // Passing all these words through cleanToken()
        if (token.size() > 0)
        {
            tokens.insert(token); // Inserting token into set unless empty string was returned by cleanToken (token is to be discarded)
        }
    }
    return tokens;
}

// Function to build the entire inverted index from a data file and returns the number of pages read in the process
int buildIndex(string filename, map<string, set<string>> &index)
{
    int numPages = 0;
    string URL;
    string line;
    set<string> tokens;
    ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open())
    { // Checking if file was open correctly
        cout << "Invalid filename." << endl;
        return 0;
    }
    while (!inFS.eof())
    {
        getline(inFS, URL); // Reading URLs from file and breaking out of loop if URL is an empty string (end of file is reached)
        if (URL == "")
        {
            break;
        }
        getline(inFS, line);         // Reading next line from the file (URL's content)
        tokens = gatherTokens(line); // Creating a set of all tokens read from that page
        for (auto &e : tokens)
        {
            index[e].insert(URL); // Filling map with information (inverted index)
        }
        ++numPages;
    }
    return numPages;
}

// Function to tokenize a query, find appropriate matches, and return a set of mathing URLs
set<string> findQueryMatches(map<string, set<string>> &index, string sentence)
{
    set<string> result;
    stringstream strstream;
    string term;
    strstream << sentence; // Using stringstream to split user's query into words
    while (strstream >> term)
    {
        char modifier = term[0]; // Recognising the modifier (if any) for each word
        term = cleanToken(term);
        if (modifier != '+' && modifier != '-')
        {
            for (auto &e : index[term])
            {
                result.insert(e); // Simply inserting matching URLs if no modifier is present
            }
        }
        if (modifier == '+')
        {
            // If "+"" modifier is present, we need to remove URLs from result that are NOT present in the term's index
            set<string> newResult; // Creating a new result set that will copy URLs from result that contain the term
            for (auto &e : result)
            {
                if (index[term].count(e))
                {
                    newResult.insert(e); // Adding URL only if it is indexed with term
                }
            }
            result = newResult;
        }
        if (modifier == '-')
        {
            // If "-" modifier is present, we need to remove URLs from result that ARE present in the term's index
            set<string> newResult; // Creating a new result set that will copy URLs from result that DON'T contain the term
            for (auto &e : result)
            {
                if (!index[term].count(e))
                {
                    newResult.insert(e); // Adding URL only if it is not indexed with term
                }
            }
            result = newResult;
        }
    }
    return result;
}

// Function that brings together all previous functions to work as a search enging. Doesn't return any value
void searchEngine(string filename)
{
    string line;
    string userQuery = " ";
    ifstream inFS;
    map<string, set<string>> index;
    set<string> result;
    inFS.open(filename);
    if (!inFS.is_open())
    { // Checking if file was opened correctly
        cout << "Invalid filename." << endl;
        return;
    }
    int pages = buildIndex(filename, index); // Building inverted index and storing the number of pages read
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << pages << " pages containing " << index.size() << " unique terms" << endl;
    cout << endl;
    while (userQuery != "")
    { // Running loop till user enters <ENTER>
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, userQuery);
        if (userQuery == "")
        {
            break;
        }
        result = findQueryMatches(index, userQuery); // Process user's query and return a set of URLs
        cout << "Found " << result.size() << " matching pages" << endl;
        for (auto &e : result)
        {
            cout << e << endl; // Printing all matches
        }
        cout << endl;
    }
    cout << "Thank you for searching!"; // Exiting program
}
