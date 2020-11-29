#ifndef __QUERYPARSER_
#define __QUERYPARSER_

#include"document_utils.hpp"
#include"B+Tree/linkedlist.hpp"
#include"IRSystem.hpp"

template<typename IRS>
class QueryParser{
public:
  template<typename I> friend class IRSystem;

  using LinkedList = List<int>;
  using IR = IRSystem<IRS>;

private:
  /** query */
  std::string query;

  /** perform normalization and stemming in the terms appearing in the query */
  void preprocessTokens(std::vector<std::string>& tokens);
  /** split query into tokens */
  void queryToTokens(std::vector<std::string>& tokens);

  /** answer query of the form "NOT term" */
  void notQuery(std::string& term1, LinkedList& result, IR& ir);
  /** answer query of the form "term1 BOOLEAN_OPERATOR term2" */
  void answerQuery(LinkedList& list1, LinkedList& list2, std::string& op, LinkedList& result, IR& ir);
  /** answer a query which has not nested queries */
  void answerSimpleQuery(std::vector<std::string>& tokens, LinkedList& result, IR& ir);

  /** find parenthesis contained in the query */
  //assume well structured
  void findParenthesis(std::vector<std::string>& tokens, std::vector<int>& parenthesis, std::string& p);
  /** identify the structure of the query (i.e. identify all subexpressions) */
  void parseTotalQuery(std::vector<std::string>& tokens, std::vector<std::pair<int,int>>& subexpr);
  /** get posting list of a term which is encoded by some escape character */
  //(spiega bene come funziona tutta la faccenda degli escape characters)
  void getListFromEscapeCharacter(std::string& currentTerm,
                                  std::vector<std::string>& currentQuery,
                                  std::vector<LinkedList>& subexprResults,
                                  std::vector<LinkedList>& cList,
                                  LinkedList& result, IR& ir);
  /** answer to a query that depends on other subqueries */
  void parsePartialQuery(std::vector<std::string>& currentQuery, std::vector<LinkedList>& subexprResults,
                        LinkedList& result, std::vector<LinkedList>& cList, IR& ir);
public:
  /** default constructor */
  QueryParser() = default;

  /** custom constructor */
  QueryParser(std::string& q){
    query = q;
  }

  /** identify the structure of a query and answer to it */
  LinkedList answer(IR& ir);

  /** default destructor */
  ~QueryParser() = default;

};

#include"query_parser.hpp"

#endif
