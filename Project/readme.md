# Low level details project
---

Information Retrieval exam 2020-2021.

**Author**: Gaia Saveri

## Structure of the repository
---

This repository contains the following folders:

* `data`, which contains the dataset used in this project (subfolder `documents`). It is a publicly available version of the Reuters-$21578$ corpus. It is a collection of $10788$ documents from the Reuters financial newswire service. This dataset comes with a set of stop-words (file `stopwords`), used during linguistic preprocessing.
The other files in this folder are files I produced for benchmark purposes:

  * `words.txt` contains the un-normalised words present in the corpus (without duplicates), it is used inside the file `python/Queries.ipynb` to built two dataset of queries. These produced datasets are `simple_queries.txt` (containing query of the form $term_1 \ OP \ term_2$, which run in linear time) and `complex_queries.txt` (containing more complex queries).

  * `nwords.txt` contains the number of words in the vocabulary for different sizes of the corpus. It is used in the file `python/Laws.ipynb`, to plot the Heap's law for this dataset.   

* `files`, which contains the files resulted after building the inverted index. `dictionary.txt` and `dict_offsets.txt` represent respectively the vocabulary and its offsets in bytes (i.e. the $i$-th line of this file contains the number of bytes after the beginning of the vocabulary file necessary to reach the $i$-th word of the vocabulary). `posting_lists.txt` and `post_offsets.txt` represent respectively the file containing the posting list of each term in the vocabulary and its offsets in bytes (where offsets are define in analogous way as for the previous vocabulary file). `compressed_dictionary.txt` and `compressed_dict_offsets` represent respectively the compressed version of the vocabulary (compression is made via blocked storage plus front coding) and its offsets. `compressed_posting_list.txt` and `compressed_post_offsets` represent respectively the compressed version of the posting lists of each term in the vocabulary (compression is made via variable byte codes) and its offsets in bytes.

* `include`, which is the core of this project, it contains subfolders:

  * `B+Tree`: contains the data structures used for the non-compressed version of the dictionary and the posting lists. In `BPTree.hpp` there is the interface for the B+Tree used to lookup in the dictionary, in `BPTree_methods.hpp` there is the implementation of the methods of the B+Tree, in `node.hpp` there is the implementation of nodes of the B+Tree.  In `linkedlist.hpp` there is the implementation of a singly linked list used to represent the posting list. This is actually a modified (hopefully improved!) version of the linked list developed during the course of Advanced Programming, held by prof. Sartori (here the original implementation [linked list](https://github.com/asartori86/advanced_programming_2019-20/blob/master/lectures/c%2B%2B/08_STL/as_linked_list.cc)).

  * `Dictionary`: this folder contains two representations for the dictionary on disk (i.e. in both cases the dictionary is stored in a file and terms are read in memory when necessary). `Dictionary.hpp` is the interface for the non-compressed, naive, representation: terms here a searched by binary searching a sorted file containing the vocabulary. `d_methods.hpp` contains the implementation of the methods for this class. `CompressedDictionary` is the interface for the compressed representation of the dictionary: this class serves both to compress the dictionary and to search terms on it, terms are searched by first locating the block in which they are by binary searching the set of blocks, then the term is found by linear scanning the block containing it. `cd_methods.hpp` contains the implementation of the methods for this class.

  * `PostingLists`: this folder contains two representations for the posting lists on disk. `Postings.hpp` is the interface for the non-compressed version: all the posting lists are stored in a file (in such a way that to the $i$-th line corresponds the posting line of the $i$-th term of the lexicographically sorted vocabulary) and read inside a linked list when necessary. `p_methods` contains the implementation of the methods for this class. `CompressedPostings.hpp` is the interface for the compressed version of the posting lists: this class serves both to compress all the posting lists and to read them, one at a time, when necessary. `cpl_methods.hpp` contains the implementation of the methods for this class.

  * `Indexes`:
