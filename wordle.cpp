#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <queue>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(
    int index, //current index function is working on
    std::string& possibleWord, //current word function is working on
    std::string& floating, //queue of all floating letters (so we can pop easier)
    std::set<std::string>& wordSet,
    const std::set<std::string>& dict); //set of strings to add to once word is valid

void wordleFloatHelper(
    int index, //current index function is working on
    std::string& possibleWord, //current word function is working on
    std::string floating, //queue of all floating letters (so we can pop easier)
    std::set<std::string>& wordSet,
    const std::set<std::string>& dict); //set of strings to add to once word is valid);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::string possibleWord = in;
    std::string floatingCopy = floating;
    std::set<std::string> wordSet;

    int blankCount = 0;
    for (size_t i = 0; i < in.size(); i++) {
        if (in[i] == '-') {
            blankCount++;
        }
    }
    if (blankCount == floating.size()) { //just need to permute the yellow words through the blank spaces, needs second helper??
        wordleFloatHelper(0, possibleWord, floatingCopy, wordSet, dict);
    } else {
        wordleHelper(0, possibleWord, floatingCopy, wordSet, dict);
    }

    return wordSet;
}

// Define any helper functions here
void wordleHelper(int index, std::string& possibleWord, std::string& floating, std::set<std::string>& wordSet, const std::set<std::string>& dict) {
    if (index >= possibleWord.size()) {//we've made a full word
        if (floating.size() == 0) {
            if (dict.find(possibleWord) != dict.end() && wordSet.find(possibleWord) == wordSet.end()) { //word is real and no floating letters and not already found
                std::string wordCopy = possibleWord; //to avoid having every string in the set reference one string
                wordSet.insert(wordCopy);
                //std::cout << possibleWord << std::endl;
            }
        }
        return;
    }

    if (possibleWord[index] != '-') { //if we come across a set letter
        wordleHelper(index + 1, possibleWord, floating, wordSet, dict); //go to the next position
        return; //once we're done with all following possibilities, return
    }

    for (int i = 0; i < 26; i++) {
        char letter = (char)(i + 'a');
        possibleWord[index] = letter;

        if (floating.find(letter) != std::string::npos) { //if the letter we will try is in floating, take it out so we can say we used it
            floating.erase(floating.find(letter), 1); //erasing it

            wordleHelper(index + 1, possibleWord, floating, wordSet, dict);
            floating.push_back(letter);
        } else {
            wordleHelper(index + 1, possibleWord, floating, wordSet, dict);
        }
    } //all letters have been tried for this spot
    possibleWord[index] = '-';
}

void wordleFloatHelper(int index, std::string& possibleWord, std::string floating, std::set<std::string>& wordSet, const std::set<std::string>& dict) { //pretty much same as wordle helper but only permutes through each letter in floating
    if (index >= possibleWord.size()) {//we've made a full word
        if (floating.size() == 0) {
            if (dict.find(possibleWord) != dict.end() && wordSet.find(possibleWord) == wordSet.end()) { //word is real and no floating letters and not already found
                std::string wordCopy = possibleWord; //to avoid having every string in the set reference one string
                wordSet.insert(wordCopy);
                // std::cout << possibleWord << std::endl;
            }
        }
        return;
    }

    if (possibleWord[index] != '-') { //if we come across a set letter
        wordleFloatHelper(index + 1, possibleWord, floating, wordSet, dict); //go to the next position
        return; //once we're done with all following possibilities, return
    }


    std::string floatingCopy = floating;
    for (size_t i = 0; i < floating.size(); i++) {
        possibleWord[index] = floating[i];
        floatingCopy.erase(i, 1);
        // std::cout << "Recursion Level: " << index << std::endl;
        // std::cout << "Current Letter: " << floating[i] << std::endl;
        // std::cout << "Current i-value: " << i << std::endl;
        // std::cout << "FloatingCopy After Erase: " << floatingCopy << std::endl;
        // if (i == 0) {
        //     std::cout << "I IS ZEROOOOOOOOOOOOOO" << std::endl << std::endl << std::endl << std::endl;
        //     //break;
        // }
        
       wordleFloatHelper(index+1, possibleWord, floatingCopy, wordSet, dict);
       floatingCopy = floating;
       //std::cout << "FloatingCopy After Reset: " << floatingCopy << std::endl;
    }
    possibleWord[index] = '-';
}