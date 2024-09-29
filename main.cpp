/****************************
 * Project 2: Search Engine
 *
 * Course: CS 251, Fall 2023
 * System: WindowsOS using VSCode
 * Program Language: C++
 * Student Author: Zaid Khan Mohammed
 * *** **********************/


/* Instructions for Users:
 *
 * 1. Input a Filename:
 *    When prompted, enter the path to a text file containing the web pages to be indexed. 
 *    The file should contain URLs and their corresponding content.
 *
 * 2. Program Functions:
 *    - The program will index the content of the provided file and allow users to perform 
 *      searches on the indexed data.
 *
 * 3. Query Format:
 *    - Users can enter search queries directly. The program supports basic search 
 *      operations, including:
 *      - Single keyword searches (e.g., "eggs")
 *      - Queries with special characters (e.g., "...milk", "fish...")
 *      - Queries with combinations (e.g., "FISH +EGGS", "fish -red")
 *
 * 4. Testing:
 *    - You can uncomment the testing line in `main()` to run the built-in tests for 
 *      various functions. Ensure the test files are in the correct directory.
 *
 *
 * Note: Ensure that the required text files for testing (e.g., "tiny.txt") are present in 
 * the correct location. The program will output results to the console.
*/


// Including libraries
#include "search.h"
using namespace std;

bool testCleanToken();
bool testGatherTokens();
bool testBuildIndex();
bool testFindQueryMatches();
bool runTests();


int main()
{

    string filename;
    cout<<"Enter file to read from: ";
    getline(cin, filename);
    // if (!runTests()){cout<<"Not all tests passed!"<<endl;} // Testing all functions before implementation
    searchEngine(filename);
    return 0;
}


// Function to test functionality of function cleanToken()
bool testCleanToken()
{
    string ans = "hello";
    int pass = 0, fail = 0;

    // Checking if function return values match expected values
    ans == cleanToken("hello") ? ++pass : ++fail;
    ans == cleanToken("...hello") ? ++pass : ++fail;
    ans == cleanToken("hello...") ? ++pass : ++fail;
    "" == cleanToken(" .,12321,. ") ? ++pass : ++fail;
    "123hello321" == cleanToken("123hello321") ? ++pass : ++fail;
    ans == cleanToken("   HELLO   ") ? ++pass : ++fail;
    ans == cleanToken("!@#hello#@!") ? ++pass : ++fail;
    cout << "Testing cleanToken: " << endl;
    cout << "Pass: " << pass << endl;
    cout << "Fail: " << fail << endl;
    return 0 == fail;
}


// Function to test functionality of function gatherToken()
bool testGatherTokens()
{
    int pass = 0, fail = 0;
    set<string> tokens = gatherTokens("to be or not to be");
    set<string> answers = {"to", "be", "or", "not"};
    tokens.size() == answers.size() ? ++pass : ++fail;
    // Checking if function return values match expected values
    gatherTokens("to be or not to be") == answers ? ++pass : ++fail;
    gatherTokens("...to ...be ...or ...not ...to ...be") == answers ? ++pass : ++fail;
    gatherTokens("to... be... or... not... to... be...") == answers ? ++pass : ++fail;
    gatherTokens("   to   be   or .,~12321~,.  not   to   be") == answers ? ++pass : ++fail;
    gatherTokens("!@#to#@! !@#be#@! !@#or#@! !@#not#@! !@#to#@! !@#be#@!") == answers ? ++pass : ++fail;
    cout << "Testing gatherTokens: " << endl;
    cout << "Pass: " << pass << endl;
    cout << "Fail: " << fail << endl;
    return 0 == fail;
}


// Function to test functionality of function buildIndex()
bool testBuildIndex()
{
    int pass = 0, fail = 0;
    map<string, set<string>> index;
    set<string> matches1 = {"www.shoppinglist.com"};
    set<string> matches2 = {"www.shoppinglist.com", "www.dr.seuss.net"};
    set<string> matches3 = {"www.rainbow.org"};
    set<string> matches4 = {"www.bigbadwolf.com"};
    // Checking if function return value matches expected value
    int numPages = buildIndex("notafile.txt", index);
    numPages == 0 ? ++pass : ++fail;
    numPages = buildIndex("tiny.txt", index);
    numPages == 4 ? ++pass : ++fail;
    // Checking if URL's are correctly indexed
    index["eggs"] == matches1 ? ++pass : ++fail;
    index["milk"] == matches1 ? ++pass : ++fail;
    index["fish"] == matches2 ? ++pass : ++fail;
    index["green"] == matches3 ? ++pass : ++fail;
    index["i'm"] == matches4 ? ++pass : ++fail;
    cout << "Testing buildIndex: " << endl;
    cout << "Pass: " << pass << endl;
    cout << "Fail: " << fail << endl;
    return 0 == fail;
}


// Function to test functionality of function findQueryMatches()
bool testFindQueryMatches()
{
    int pass = 0, fail = 0;
    map<string, set<string>> index;
    set<string> result1 = {"www.shoppinglist.com"};
    set<string> result2 = {"www.shoppinglist.com", "www.dr.seuss.net"};
    set<string> result3 = {};
    set<string> result4 = {"www.bigbadwolf.com"};
    buildIndex("tiny.txt", index);
    // Checking that functoin correctly tokenizes user's query
    findQueryMatches(index, "eggs") == result1 ? ++pass : ++fail;
    findQueryMatches(index, "...milk") == result1 ? ++pass : ++fail;
    findQueryMatches(index, "fish...") == result2 ? ++pass : ++fail;
    findQueryMatches(index, " .,12321,. ") == result3 ? ++pass : ++fail;
    findQueryMatches(index, "123green321") == result3 ? ++pass : ++fail;
    findQueryMatches(index, "   I'M   ") == result4 ? ++pass : ++fail;
    findQueryMatches(index, "FISH +EGGS") == result1 ? ++pass : ++fail;
    findQueryMatches(index, "fish -red") == result1 ? ++pass : ++fail;
    cout << "Testing findQueryMatches: " << endl;
    cout << "Pass: " << pass << endl;
    cout << "Fail: " << fail << endl;
    return 0 == fail;
}


// Calling all function test cases in one function that can be called in main()
bool runTests()
{
    return testCleanToken() && testGatherTokens() && testBuildIndex() && testFindQueryMatches();
}