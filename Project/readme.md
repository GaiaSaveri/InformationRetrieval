# Low level details project

Information Retrieval exam 2020-2021.

**Author**: Gaia Saveri

## Structure of the repository

This repository contains the following folders:

* `data`, which contains the dataset used in this project (subfolder `documents`). It is a publicly available version of the Reuters-21578 corpus. It is a collection of 10788 documents from the Reuters financial newswire service. This dataset comes with a set of stop-words (file `stopwords`), used during linguistic preprocessing.
The other files in this folder are files I produced for benchmark purposes:

  * `words.txt` contains the un-normalised words present in the corpus (without duplicates), it is used inside the file `python/Queries.ipynb` to built two datasets of queries. These produced datasets are `simple_queries.txt` (containing queries of the form *term1 BOOL_OP term2*, which run in linear time) and `complex_queries.txt` (containing more complex queries).

  * `nwords.txt` contains the number of words in the vocabulary for different sizes of the corpus. It is used in the file `python/Laws.ipynb`, to plot the Heap's law for this dataset.   

* `files`, which contains the files resulted after building the inverted index. `dictionary.txt` and `dict_offsets.txt` represent respectively the vocabulary and its offsets in bytes (i.e. the i-th line of this file contains the number of bytes after the beginning of the vocabulary file necessary to reach the i-th word of the vocabulary). `posting_lists.txt` and `post_offsets.txt` represent respectively the file containing the posting list of each term in the vocabulary and its offsets in bytes (where offsets are defined in analogous way as for the previous vocabulary file). `compressed_dictionary.txt` and `compressed_dict_offsets` represent respectively the compressed version of the vocabulary (compression is made via blocked storage plus front coding) and its offsets. `compressed_posting_list.txt` and `compressed_post_offsets` represent respectively the compressed version of the posting lists of each term in the vocabulary (compression is made via variable byte codes) and its offsets in bytes.

* `include`, which is the core of this project, it contains subfolders:

  * `B+Tree`: contains the data structures used for the non-compressed version of the dictionary and the posting lists. In `BPTree.hpp` there is the interface for the B+Tree used to lookup in the dictionary, in `BPTree_methods.hpp` there is the implementation of the methods of the B+Tree, in `node.hpp` there is the implementation of nodes of the B+Tree.  In `linkedlist.hpp` there is the implementation of a singly linked list used to represent the posting lists. This is actually a modified (hopefully improved!) version of the linked list developed during the course of Advanced Programming, held by prof. Sartori (here the original implementation [linked list](https://github.com/asartori86/advanced_programming_2019-20/blob/master/lectures/c%2B%2B/08_STL/as_linked_list.cc)).

  * `Dictionary`: this folder contains two representations for the dictionary on disk (i.e. in both cases the dictionary is stored in a file and terms are read in memory when necessary). `Dictionary.hpp` is the interface for the non-compressed, naive, representation: terms here a searched by binary searching a sorted file containing the vocabulary. `d_methods.hpp` contains the implementation of the methods for this class. `CompressedDictionary` is the interface for the compressed representation of the dictionary: this class serves both to compress the dictionary and to search terms on it. Terms are searched by first locating the block in which they are (by binary searching the set of blocks), then the term is found by linear scanning the block containing it. `cd_methods.hpp` contains the implementation of the methods for this class.

  * `PostingLists`: this folder contains two representations for the posting lists on disk. `Postings.hpp` is the interface for the non-compressed version: all the posting lists are stored in a file (in such a way that to the i-th line corresponds the posting line of the i-th term of the lexicographically sorted vocabulary) and read inside a linked list when necessary. `p_methods` contains the implementation of the methods for this class. `CompressedPostings.hpp` is the interface for the compressed version of the posting lists: this class serves both to compress all the posting lists and to read them, one at a time, when necessary. `cpl_methods.hpp` contains the implementation of the methods for this class.

  * `Indexes`: this folder contains two possible representation for the inverted index: `TreeIndex.hpp`, which is used when the search structure for the dictionary is a B+Tree (hence the whole dictionary is in memory), and `FileIndex.hpp` used when both the dictionary and the posting lists resides on files on disk. The class `Document.hpp` contains an interface for a generic document, intended as a single file, having as attributes a unique DocID and a bag of words of terms. The files `document_methods.hpp` and `document_utils.hpp` contain the implementation of its methods: essentially functions to perform the linguistic preprocessing pipeline (for stemming purposes, the Porter2 stemmer is used, its c++ version used in this project can be found at [this link](https://bitbucket.org/smassung/porter2_stemmer/wiki/Home), and also in the `stemming` folder). Finally, the file `file_utils.hpp` contains a set of functions for managing files used all across the project.

  * `IR`: this folder contains the actual Information Retrieval System. It is located in the header `IRsystem.hpp`, which is a templated class used to build a Boolean IR, using one among the inverted indexes described above (the index we mean to use has to be specified as template parameter). The file `QueryParser.hpp` contains the interface for a naive system able to parse and answer (well-formed) boolean queries of theoretically arbitrary complexity. It is naive because it does not apply any optimisation technique. The file `query_parser.hpp` contains the implementation of its methods.

* `src`: this folder contains the source code used to test the correctness and the performance of the IR System described above. The file `benchmark.cc` tests the time needed to retrieve both terms and posting lists using different types of inverted indexes. The file `query.cc` tests the time took by differently built IR Systems for answering query, and also checks that all systems agree on the result of each query. The file `main.cc` lets the user specify which kind of inverted index he/she wants the Boolean IR System to use in order to answer his/her query.

* `test`: this folder contains the files written when running the `benchmark.cc` code, these are used to evaluate the performance of the system under different conditions and to make comparisons between the compressed and the non-compressed representation of the dictionary and the posting lists in terms of speed and size.  

* `Doxygen` containing the file `doxy.in`, used to produce the Doxygen style documentation.

The `Makefile` is used to compile the codes inside the `src` folder and to generate the documentation. By typing `make` in the terminal the executables `main.o`, `queries.o` and `benchmark.o` are produced. By typing `make documentation` instead, the Doxygen style documentation is produced, and two folders `latex` and `html` are created.
