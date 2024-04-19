#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
	int isWord;
	int frequency; // If isWord, the number of occurences of the word
	struct Trie* next[26];
};

struct Trie *createTrie();

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
	struct Trie *cur = pTrie;
	int wordLen = strlen(word);
	for (int i = 0; i < wordLen; i++) {
		int nextIndex = word[i] - 'a';
		if (cur->next[nextIndex] == NULL) {
			cur->next[nextIndex] = createTrie();
			cur = cur->next[nextIndex];
		} 
		else 
			cur = cur->next[nextIndex];
	}
	if (cur->isWord)
		cur->frequency++;
	else {
		cur->isWord = 1;
		cur->frequency = 1;
	}
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
	struct Trie *cur = pTrie;
	int wordLen = strlen(word);
	for (int i = 0; i < wordLen; i++) {
		int nextIndex = word[i] - 'a';
		if (cur->next[nextIndex] == NULL)
			return 0;
		cur = cur->next[nextIndex];
	}
	if (cur->isWord)
		return cur->frequency;
	else
		return 0;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
	for (int i = 0; i < 26; i++)
		if (pTrie->next[i] != NULL) {
			deallocateTrie(pTrie->next[i]);
			pTrie->next[i] = NULL;
		}
	free(pTrie);
	return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
	struct Trie *myTrie = malloc(sizeof(struct Trie));
	myTrie->isWord = 0;

	for (int i = 0; i<26; i++)
		myTrie->next[i] = NULL;
	
	return myTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
	FILE *file;
	file = fopen(filename, "r");
	int numWords;
	fscanf(file, "%d", &numWords);
	char buffer[100];
	for (int i = 0; i < numWords; i++) {
		if (fscanf(file, "%s", buffer) != EOF) {
			pInWords[i] = strdup(buffer);
		}
	}
	fclose(file);
	return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}