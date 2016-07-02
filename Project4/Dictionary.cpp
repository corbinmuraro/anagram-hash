// Dictionary.cpp

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <algorithm>  // for swap
#include <vector>
using namespace std;


const int MAX_BUCKETS = 49999;
const int primes[26] = {2, 41, 37, 47, 3, 67, 71, 23, 5, 101, 61, 17, 19, 13, 31, 43, 97, 29, 11, 7, 73, 83, 79, 89, 59, 53};

unsigned int hashParam(const string word);
int sumCharacters(const string word);
void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);


struct Node
{
    Node();
    string value;
    Node* next;
    vector<string> permutations;
};

Node::Node()
{
    value = "";
    next = nullptr;
}


class DictionaryImpl
{
  public:
    DictionaryImpl();
    ~DictionaryImpl() {}
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
  private:
    Node* hashTable[MAX_BUCKETS];
};

DictionaryImpl::DictionaryImpl()
{
    for (int i = 0; i < MAX_BUCKETS; i++)
        hashTable[i] = nullptr;
}


void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
    
    string regularFormat = word;
    
    sort(word.begin(), word.end());         // sorts word to arrange characters in alphabetic order
    
    unsigned int index = hashParam(word) % (unsigned int)MAX_BUCKETS;        // generates index for insertion using the hash function
    

    if (hashTable[index] == nullptr)                // checks for empty list
    {
        hashTable[index] = new Node();
        hashTable[index]->value = word;
        hashTable[index]->permutations.push_back(regularFormat); // there are no other anagrams of the given characters found thus far.

    }
    
    else if (hashTable[index]->value == word)       // checks for anagrams, if found, should add it to the Node's permutations array.
    {
        hashTable[index]->permutations.push_back(regularFormat);
    }
    
    else
    {
        Node* originalHead = hashTable[index];      // keeps track of the original node at the beginning of the list
        hashTable[index] = new Node();              // creates a new node and places it at the beginning of the list
        hashTable[index]->value = word;             // assigns the value of the new node
        hashTable[index]->next = originalHead;      // sets the new node's next to the temp (prior first node)
        
        hashTable[index]->permutations.push_back(regularFormat); // there are no other anagrams of the given characters found thus far.

    }
    
}


void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    removeNonLetters(letters);
    
    sort(letters.begin(), letters.end());         // sorts word to arrange characters in alphabetic order
    
    unsigned int index;
    index = hashParam(letters) % (unsigned int)MAX_BUCKETS;
    
    for (Node* i = hashTable[index]; i != nullptr; i = i->next)     // look at each Node in the bucket
    {
        if (i->value == letters)                                    // if the node's value matches the alphabetically ordered parameter
        {
            for (int j = 0; j < i->permutations.size(); j++)
                callback(i->permutations[j]);                       // call callback for each permutation
        }
    }
}


int sumCharacters(const string word)
{
    int sum = 0;
    
    for (int i = 0; i < word.size(); i++)
    {
        sum += word[i];
    }
    
    return sum;
}


unsigned int hashParam(const string word)
{
    unsigned int result = 1;
    int primesIndex;
    
    for (int i = 0; i < word.size(); i++)
    {
        primesIndex = word[i] - 'a';        // converts each character to an array index from 0 to 25
        result *= primes[primesIndex];
    }
    
    return result;
}


void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop off everything from "to" to end.
} 




//******************** Dictionary functions **********************************

// These functions simply delegate to DictionaryImpl's functions.
// You probably don't want to change any of this code.

Dictionary::Dictionary()
{
    m_impl = new DictionaryImpl;
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}
