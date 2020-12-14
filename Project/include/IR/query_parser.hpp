#include<regex>

#include"../Indexes/document_utils.hpp"

//----------------------------- PREPROCESS QUERY -----------------------------//

//perform normalization and stemming in the terms appearing in the query
template<typename IRS>
void QueryParser<IRS>::preprocessTokens(std::vector<std::string>& tokens){
  std::vector<std::string> terms;
  std::vector<int> ind;
  //all symbols in the tokenized query that are not terms
  std::vector<std::string> special;
  //parenthesis
  special.push_back("(");
  special.push_back(")");
  //boolean operators
  special.push_back("AND");
  special.push_back("OR");
  special.push_back("NOT");
  special.push_back("ANDNOT");
  special.push_back("ORNOT");
  for(size_t i=0; i<tokens.size(); i++){
    if(std::find(special.begin(), special.end(), tokens.at(i)) == special.end()){
      //current token is a term
      terms.push_back(tokens.at(i));
      ind.push_back(i);
    }
  }
  //normalize terms
  normalize(terms);
  //perform stemming on terms
  stemming(terms);
  //back to tokenized query, in the right positions given by ind
  for(size_t i=0; i<terms.size(); i++){
    tokens.at(ind.at(i)) = terms.at(i);
  }
}

//split query in tokens and preprocess tokens
template<typename IRS>
void QueryParser<IRS>::queryToTokens(std::vector<std::string>& tokens){
  std::string token;
  size_t pos = 0;
  std::string delimiter = " ";
  //fix AND NOT and OR NOT
  query = std::regex_replace(query, std::regex("AND NOT"), "ANDNOT");
  query = std::regex_replace(query, std::regex("OR NOT"), "ORNOT");
  //split query in tokens based on spaces
  while ((pos = query.find(delimiter)) != std::string::npos) {
    token = query.substr(0, pos);
    tokens.push_back(token);
    query.erase(0, pos + delimiter.length());
  }
  //push what remains
  tokens.push_back(query);
  preprocessTokens(tokens);
}

//------------------------- ANSWER INDEPENDENT QUERY -------------------------//

template<typename IRS>
void QueryParser<IRS>::notQuery(std::string& term1, typename QueryParser<IRS>::LinkedList& result, typename QueryParser<IRS>::IR& ir){
  LinkedList list1{};
  //list of posintgs associated to term1
  ir.invertedIndex.getPostingList(term1, list1);
  list1.complement(result, 0, ir.getMaxDocID());
}

template<typename IRS>
void QueryParser<IRS>::answerQuery(typename QueryParser<IRS>::LinkedList& list1, typename QueryParser<IRS>::LinkedList& list2, std::string& op,
                                   typename QueryParser<IRS>::LinkedList& result, typename QueryParser<IRS>::IR& ir){
  if(op.compare("AND") == 0){
    list1.intersection(list2, result);
  }
  else if(op.compare("OR") == 0){
    list1.union_list(list2, result);
  }
  else if(op.compare("ANDNOT") == 0) {
    list1.andnot(list2, result);
  }
  else{ //OR NOT
    LinkedList diff{};
    list2.complement(diff, 0, ir.getMaxDocID());
    list1.union_list(diff, result);
  }
}

template<typename IRS>
void QueryParser<IRS>::answerSimpleQuery(std::vector<std::string>& tokens, typename QueryParser<IRS>::LinkedList& result, typename QueryParser<IRS>::IR& ir){
  LinkedList tmp1{}; //partial result
  int start = 1;
  if(tokens.at(0).compare("NOT")==0) {
    start = 2;
    notQuery(tokens.at(1), result, ir);
  }
  else ir.invertedIndex.getPostingList(tokens.at(0), result);
  for(size_t i=start; i<tokens.size(); i+=2){
    LinkedList tmp2{}; //list of the current term
    ir.invertedIndex.getPostingList(tokens.at(i+1), tmp2);
    answerQuery(result, tmp2, tokens.at(i), tmp1, ir);
    result = std::move(tmp1);
  }
}

//-------------------------- PARSE NESTED QUERIES ----------------------------//

template<typename IRS>
void QueryParser<IRS>::findParenthesis(std::vector<std::string>& tokens, std::vector<int>& parenthesis, std::string& p){
  for(size_t i=0; i<tokens.size(); i++){
    if(tokens.at(i).compare(p)==0) parenthesis.push_back(i);
  }
}

template<typename IRS>
void QueryParser<IRS>::parseTotalQuery(std::vector<std::string>& tokens, std::vector<std::pair<int,int>>& subexpr){
  //query must be enclosed in () for the whole parsing machinery to work
  std::string o = "(";
  std::string c =")";
  query.insert(0,o);
  query.append(c);
  //fix parenthesis formatting
  query = std::regex_replace(query, std::regex("\\( |\\("), "\( ");
  query = std::regex_replace(query, std::regex(" \\)|\\)"), " )" );
  //from query to set of tokens, i.e. split when find a space
  queryToTokens(tokens);
  std::vector<int> open;
  std::vector<int> close;
  //find parentesis' index (open and close parenthesis respectively)
  findParenthesis(tokens, open, o);
  findParenthesis(tokens, close, c);
  //find all subexpression indexes, parsing parenthesis' structure
  //traversing in order the open vector
  for(size_t i=0; i<close.size(); i++){
    //treversing in reverse order the close vector
    for(int j=open.size()-1; j>=0; j--){
      if(close.at(i)>open.at(j)){
        //adding the indexes of the subexpression just detected
        //excluding parentheses
        subexpr.push_back(std::make_pair(open.at(j), close.at(i)));
        //delete the "consumed" open parenthesis
        open.erase(open.begin()+j);
        break;
      }  }  }
}

//get posting list of terms corresponding to escape characters
template<typename IRS>
void QueryParser<IRS>::getListFromEscapeCharacter(std::string& currentTerm, std::vector<std::string>& currentQuery,
                                std::vector<typename QueryParser<IRS>::LinkedList>& subexprResults, std::vector<typename QueryParser<IRS>::LinkedList>& cList,
                                typename QueryParser<IRS>::LinkedList& result, typename QueryParser<IRS>::IR& ir){
  std::string subExprEscape = "#";
  std::string cListEscape = "%";
  if(currentTerm.find(subExprEscape) != std::string::npos){
    //need to search in subexpr result
    bool found = false;
    size_t i = 0;
    while(!found && i<currentQuery.size()){
      std::string e = "#";
      std::string currentEscape = e.append(std::to_string(i));
      if(currentTerm.compare(currentEscape)==0){
        auto itr=std::find(currentQuery.begin(), currentQuery.end(), currentEscape);
        auto index=std::distance(currentQuery.begin(), itr);
        currentQuery.erase(std::remove_if(currentQuery.begin()+(index+1), currentQuery.end(),
          [currentEscape](std::string s){return (s==currentEscape);}), currentQuery.end());
        found = true;
      }
      if(!found) i++;
    }
    result = subexprResults.at(i);
  }
  else if(currentTerm.find(cListEscape) != std::string::npos){
    //need to search in cList
    bool found = false;
    int i = 0;
    while(!found){
      std::string c = "%";
      std::string currentEscape = c.append(std::to_string(i));
      if(currentTerm.compare(currentEscape)==0){
        found = true;
      }
      if(!found) i++;
    }
    result = cList.at(i);
  }
  else ir.invertedIndex.getPostingList(currentTerm, result);
}

//answer a query that depends on other subexpression
template<typename IRS>
void QueryParser<IRS>::parsePartialQuery(std::vector<std::string>& currentQuery, std::vector<typename QueryParser<IRS>::LinkedList>& subexprResults,
                       typename QueryParser<IRS>::LinkedList& result, std::vector<typename QueryParser<IRS>::LinkedList>& cList, typename QueryParser<IRS>::IR& ir){
  //remove parenthesis
  currentQuery.erase(std::remove_if(currentQuery.begin(), currentQuery.end(),
    [](std::string s){return (s=="(" || s == ")");}), currentQuery.end());
  //adjust query
  for(size_t i=0; i<currentQuery.size(); i++){
    std::string escapeTerm = "%";
    std::string currentEscapeTerm = escapeTerm.append(std::to_string(cList.size()));
  //if currentQuery.at(i) is an escape character
  if(currentQuery.at(i).find("#") == std::string::npos && currentQuery.at(0).compare("NOT")!=0){
    //if current query is a term
    if(currentQuery.at(i).compare("AND")!=0 && currentQuery.at(i).compare("OR")!=0 && currentQuery.at(i).compare("NOT")!=0 && currentQuery.at(i).compare("ORNOT")!=0 && currentQuery.at(i).compare("ANDNOT")!=0){
      //retrieve its posting list
      LinkedList currentList{};
      ir.invertedIndex.getPostingList(currentQuery.at(i), currentList);
      cList.push_back(currentList);
      currentQuery.at(i) = currentEscapeTerm;
    }
    }
  }
  //answer query
  LinkedList tmp1{}; //temporary result
  int start = 1;
  if(currentQuery.at(0).compare("NOT")==0) {
    start = 2;
    notQuery(currentQuery.at(1), result, ir);
  }
  //handle escape characters
  else getListFromEscapeCharacter(currentQuery.at(0), currentQuery, subexprResults, cList, result, ir);
  for(size_t i=start; i<currentQuery.size(); i+=2){
    LinkedList tmp2{}; //posting list of the current term
    //handle escape characters
    getListFromEscapeCharacter(currentQuery.at(i+1), currentQuery, subexprResults, cList, tmp2, ir);
    answerQuery(result, tmp2, currentQuery.at(i), tmp1, ir);
    result = std::move(tmp1);
  }
}

//--------------------------- ANSWER GENERIC QUERY ---------------------------//

template<typename IRS>
typename QueryParser<IRS>::LinkedList QueryParser<IRS>::answer(typename QueryParser<IRS>::IR& ir){
  std::vector<std::string> tokens;
  std::vector<std::pair<int,int>> subexpr;
  parseTotalQuery(tokens, subexpr);
  std::vector<LinkedList> subexprResults;
  for(auto x : subexpr){
    //escape symbol to detect queries that depends on other subqueries
    std::string escape = "#";
    //update escape symbol
    std::string currentEscape = escape.append(std::to_string(subexprResults.size()));
    //query to answer at current stage
    std::vector<std::string> currentQuery;
    //in subexpr we have [i,j], actual token in tokens are [i+1, j-1]
    for(int i=x.first+1; i<x.second; i++){
      //compute terms in current query
      currentQuery.push_back(tokens.at(i));
      tokens.at(i) = currentEscape;
    }
      //detect if it is an "independent" query or depends on partial results
      bool dependent = false;
      size_t i = 0;
      while(!dependent && i<currentQuery.size()){
        if(currentQuery.at(i).find("#") != std::string::npos){
          dependent = true;
          }
        i++;
      }
      LinkedList result{};
      if(dependent){
        std::vector<LinkedList> cList{};
        parsePartialQuery(currentQuery, subexprResults, result, cList, ir);
        subexprResults.push_back(result);
      }
      else if(!dependent){ //parse and resolve
        answerSimpleQuery(currentQuery, result, ir);
        subexprResults.push_back(result);
      }
    }
    return subexprResults.back();
}
