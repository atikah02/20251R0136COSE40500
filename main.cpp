/** 
  * Assignment 4 for COSE213 Data Structures
  *
  * Won-Ki Jeong (wkjeong@korea.ac.kr)
  *
  * 2025. 5. 26
  *
  */
  
#include "LinkedBinaryTree.h"
#include "SearchTree.h" 
#include "AVLTree.h"

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
 
using namespace std;

// to generate integer keys in skewed (sorted) or random order
void generateKeys(std::vector<int>& keys, int size, bool randomize = false) {
    keys.clear();
    for (int i = 0; i < size; ++i)
        keys.push_back(i + 1);  // generate keys 1..size
    if (randomize) {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(keys.begin(), keys.end(), g);
    }
}

// to generate random float values for keys
void generateValues(std::vector<float>& values, int size) {
    values.clear();
    for (int i = 0; i < size; ++i)
        values.push_back(static_cast<float>(rand()) / RAND_MAX * 20000.0f);
}


int main()
{
	typedef Entry<int, float> EntryType;

	LinkedBinaryTree<EntryType> t;
	
	std::cout << "Size : " << t.size() << std::endl;
	
	t.addRoot();
	
	std::cout << "Size : " << t.size() << std::endl;
	
	SearchTree<EntryType>	st;
	
	std::cout << "Size : " << st.size() << std::endl;
	st.insert(1, 2.5);
	st.insert(3, 4.5);
	st.insert(7, 5.5);
	st.insert(2, 1.5);
	st.insert(3, 8.5);
	std::cout << "Size : " << st.size() << std::endl;

	for(SearchTree<EntryType>::Iterator it = st.begin(); it != st.end(); ++it)
	{
			std::cout << (*it).key() << " , " << (*it).value() << std::endl;
	}
		
	st.erase(3);
	std::cout << "Size : " << st.size() << std::endl;
	for(SearchTree<EntryType>::Iterator it = st.begin(); it != st.end(); ++it)
	{
			std::cout << (*it).key() << " , " << (*it).value() << std::endl;
	}	
	
	std::cout << "------------" << std::endl;

	std::cout << ">>> Starting performance benchmark..." << std::endl;

	//
	//
	//
	AVLTree<EntryType>	avl;
	
	// random test
	// int nElem = 100000; //100000000;
	
	// // int *key = new int[nElem*2];
	// // float *val = new float[nElem*2];
	
	// std::srand(std::time(0)); // use current time as seed for random generator
   
		  
	// // initialize
	// for(int i=0; i<nElem*2; i++)
	// {
	// 	key[i] = std::rand();
	// 	val[i] = (float) std::rand()/RAND_MAX * 20000;
	// }
		

    //
    // AVL tree Insert test
    //	
	// clock_t tm;
    // tm = clock();
	// for(int i=0; i<nElem; i++)
	// {
	// 	avl.insert(key[i], val[i]);
	// }
	// tm = clock() - tm;
	// printf ("It took me %f seconds.\n",((float)tm)/(float)CLOCKS_PER_SEC);
	

    //
    // AVL tree Find test
    //	
    // This example is finding keys (i=nElem~2*nElem-1) different from the 
	// inserted keys (i=0~nElem-1), but the design of the experiment is 
	// your own choice. No need to follow this example.
	//
	// tm = clock();
	// for(int i=nElem; i<nElem*2; i++) 
	// {
	// 	avl.find(key[i]);
	// }
	// tm = clock() - tm;
	// printf ("It took me %f seconds.\n",((float)tm)/(float)CLOCKS_PER_SEC);
	
	//
	// Experimental Insert Timing for Performance Test
	//
	// to measure insertion time for both trees
	//
	std::vector<int> keys;
    std::vector<float> values;
    const int sizes[] = {1000};  // Can add 100000 later
    const int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    for (int i = 0; i < numSizes; ++i) {
        int size = sizes[i];

        for (int isRandom = 0; isRandom <= 1; ++isRandom) {
            bool randomize = isRandom == 1;
            generateKeys(keys, size, randomize);
            generateValues(values, size);

            // --- SearchTree insert test ---
            SearchTree<EntryType> st;
            clock_t start = clock();
            for (int j = 0; j < size; ++j)
                st.insert(keys[j], values[j]);
            clock_t end = clock();
            printf("[SearchTree] Insert (%s, %6d): %f sec\n",
                   randomize ? "random" : "skewed", size,
                   (float)(end - start) / CLOCKS_PER_SEC);
			fflush(stdout);

			start = clock();		// to measure SearchTree find time
			// for (int j = 0; j < size; ++j)
			// 	st.find(keys[j]);  // Find existing key
			for (int r = 0; r < 1000; ++r)
				for (int j = 0; j < size; ++j)
					st.find(keys[j]);
			end = clock();
			printf("[SearchTree] Find   (%s, %6d): %f sec\n",
				randomize ? "random" : "skewed", size,
				(float)(end - start) / CLOCKS_PER_SEC);
			fflush(stdout);


            // --- AVLTree insert test ---
            AVLTree<EntryType> avl;
            start = clock();
            for (int j = 0; j < size; ++j)
                avl.insert(keys[j], values[j]);
            end = clock();
            printf("[AVLTree   ] Insert (%s, %6d): %f sec\n",
                   randomize ? "random" : "skewed", size,
                   (float)(end - start) / CLOCKS_PER_SEC);
			fflush(stdout);

			start = clock();		// to measure AVLTree find time
			// for (int j = 0; j < size; ++j)
			// 	avl.find(keys[j]);  // Find existing key
			for (int r = 0; r < 1000; ++r)
				for (int j = 0; j < size; ++j)
					avl.find(keys[j]);
			end = clock();
			printf("[AVLTree   ] Find   (%s, %6d): %f sec\n",
				randomize ? "random" : "skewed", size,
				(float)(end - start) / CLOCKS_PER_SEC);
			fflush(stdout);
        }

        printf("---------------------------------------------------\n");
    }

	std::cout << ">>> Benchmark complete." << std::endl;

	return 0;
}