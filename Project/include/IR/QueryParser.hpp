/**
 *\file QueryParser.hpp
 *\author Gaia Saveri
 *\brief Class describing a naive query parser, i.e. a machinery to answer boolean queries.
 */

#ifndef __QUERYPARSER_
#define __QUERYPARSER_

#include"../Indexes/document_utils.hpp"
#include"../B+Tree/linkedlist.hpp"
#include"IRSystem.hpp"

/**
 *\tparam IRS Type of the inverted index of the Information Retrieval System.
 */
template<typename IRS>
class QueryParser{
public:
  //need to use inverted indexes' getPostingList method, and ir's generateAllList method.
  template<typename I> friend class IRSystem;

  using LinkedList = List<int>;
  using IR = IRSystem<IRS>;

private:
  /** Query we need to answer to. */
  std::string query;

  /**
   *\brief Perform normalization and stemming in the terms appearing in the query.
   *\param tokens Tokens componing the query (splittin on spaces).
   */
  void preprocessTokens(std::vector<std::string>& tokens);
  /**
   *\brief Function that splits query into tokens.
   *\param tokens Vector in which the function saves the tokens obtained from the query.
   *
   *Inside this function, if the operators "AND NOT"/"OR NOT" are detected, they are
   *modified to be respectively "ANDNOT"/"ORNOT", since they are considered as a
   *single operator (hence occupying a single cell in the vector of tokens) when it
   *comes to actually answer the query.
   */
  void queryToTokens(std::vector<std::string>& tokens);

  /**
   *\brief Function to answer a query of the form "NOT term1".
   *\param term1 Term appearing in the formula "NOT term1".
   *\param result Linked List containing the DocIDs satisfying this query.
   *|param ir Information Retrieval System, to have access to the function "generateAllList".
   */
  void notQuery(std::string& term1, LinkedList& result, IR& ir);
  /**
   *\brief Function to answer a query of the form "term1 BOOLEAN_OPERATOR term2".
   *\param list1 Linked List containing the posting list of term1
   *\param list2 Linked List containing the posting list of term2
   *\param op Boolean operator, one among AND, OR, AND NOT, OR NOT.
   *\param result Linked List containing the DocIDs satisfying this query.
   *\param ir Information Retrieval System, to have access to the function "getPostingList" of the inverted index.
   */
  void answerQuery(LinkedList& list1, LinkedList& list2, std::string& op, LinkedList& result, IR& ir);
  /**
   *\brief Function to answer a query which hasn't any nested queries.
   *\param tokens Tokens obtained by splitting the query.
   *\param result  Linked List containing the DocIDs satisfying this query.
   *\param ir Information Retrieval System, to have access to the function "getPostingList" of the inverted index.
   *
   *This function can be used for queries like "a AND b", "NOT a OR b" ...
   *It is not used for queries like "a AND (b OR c)", "(a OR b) AND NOT (c OR d)".
   */
  void answerSimpleQuery(std::vector<std::string>& tokens, LinkedList& result, IR& ir);

  /**
   *\brief Function used to find parenthesis contained in the query.
   *\param tokens Tokens obtained by splitting the query.
   *\param parentesis Vector containing the indexes at which parenthesis (either open or close) appear.
   *\param p String representing either the open or the close parenthesis, depending on what we are looking for.
   *
   *We assume that query are "well-formed", i.e. the structure of parenthesis is consistent.
   *This function is needed to identify subexpressions inside the query.
   */
  void findParenthesis(std::vector<std::string>& tokens, std::vector<int>& parenthesis, std::string& p);
  /**
   *\brief Function used to identify the structure of the query (i.e. it identifies all subexpressions).
   *\param tokens Tokens obtained by splitting the query.
   *\param subexpr Vector containing pairs of indexes, the pair [i,j] identifies a subexpression
   *starting at i-th token and ending at j-th token.
   *
   *Subexpressions are identified using the parenthesis: a subexpression starts when a parenthesis
   *is opened and ends when the corresponding parenthesis is closed.
   */
  void parseTotalQuery(std::vector<std::string>& tokens, std::vector<std::pair<int,int>>& subexpr);
  /**
   *\brief Function to retrieve the posting list of a term, possibly encoded by some escape characters.
   *\param currentTerm Token currently under analysis.
   *\param currentQuery Subexpression currently under analysis.
   *\param subexprResults Vector containing the results of the subexpressions so far computed.
   *\param cList Vector containing the posting lists associated to the terms present in the query.
   *\param result Linked List containing the posting list of the current term.
   *|param ir Information Retrieval System, to have access to the function "getPostingList" of the inverted index.
   *
   *If the current term is encoded by #x, we need to search inside the vector subexprResult
   *at position x to get its posting list.
   *If the current term is encoded by %x, we need to search inside the vector cList
   *at position x to get its posting list.
   */
  void getListFromEscapeCharacter(std::string& currentTerm,
                                  std::vector<std::string>& currentQuery,
                                  std::vector<LinkedList>& subexprResults,
                                  std::vector<LinkedList>& cList,
                                  LinkedList& result, IR& ir);
  /**
   *\brief Function to answer to a query that depends on other subqueries, i.e. that contains some escape characters.
   *\param currentQuery Subexpression currently under analysis.
   *\param subexprResults Vector containing the results of the subexpressions so far computed.
   *\param result Linked List containing the result of the current query.
   *\param cList Vector containing the posting lists associated to the terms present in the query.
   *\param ir Information Retrieval System, to have access to the function "getPostingList" of the inverted index.
   *
   *Consider the query "a AND (b OR c)", this query contains a subquery which is (b OR c).
   *So first we will answer to the query "b OR c", then to the total query "a AND (b OR c)".
   *When we answer to the "partial" query "b OR c", we store this partial result in the vector subexprResults,
   *and we replace all the terms in the subquery with the escape symbol #0.
   *So that we obtain "a AND (#0 #0 #0)".
   *The symbol # means that we need to search for the linked list we need in the vector subexprResults.
   *0 stands for the fact that, inside that vector, the list we need is at index 0.
   *We then recognize that a is a term, so we retrieve its posting list and store it inside the vector cList.
   *In order to mark this, we replace a with the symbol %0.
   *So now we have that the query is "%0 AND (#0 #0 #0)".
   *The symbol % means that we need to search for the posting list of a inside the vector cList.
   *0 stands for the fact that, inside that vector, the list we need is at index 0.
   */
  void parsePartialQuery(std::vector<std::string>& currentQuery, std::vector<LinkedList>& subexprResults,
                        LinkedList& result, std::vector<LinkedList>& cList, IR& ir);
public:
  /**
   *\brief Default constructor.
   */
  QueryParser() = default;
  /**
   *\brief Custom constructor.
   *\param q Query we need to answer to.
   */
  QueryParser(std::string& q){
    query = q;
  }
  /**
   *\brief Function used to identify the structure of a query and answer to it.
   *\param ir Information Retrieval System from which the querie originated.
   */
  LinkedList answer(IR& ir);
  /**
   *\brief Default destructor.
   */
  ~QueryParser() = default;

};

#include"query_parser.hpp"

#endif
