#ifndef __QUERY_PARSER_
#define __QUERY_PARSER_

#include<regex>

#include"index.hpp"
void findParenthesis(std::vector<std::string>& tokens, std::vector<int>& parenthesis, std::string& p){
  for(int i=0; i<tokens.size(); i++){
    if(tokens.at(i).compare(p)==0) parenthesis.push_back(i);
  }
}

//split query in tokens
void queryToTokens(std::string& query, std::vector<std::string>& tokens){
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
}

void answerSimpleQuery(std::vector<std::string>& tokens, List<int>& result, Index& ii){
  List<int> tmp1{}; //partial result
  int start = 1;
  if(tokens.at(0).compare("NOT")==0) {
    start = 2;
    ii.notQuery(tokens.at(1), result);
  }
  else ii.getPostingList(tokens.at(0), result);
  for(int i=start; i<tokens.size(); i+=2){
    List<int> tmp2{}; //list of the current term
    ii.getPostingList(tokens.at(i+1), tmp2);
    ii.answerQuery(result, tmp2, tokens.at(i), tmp1);
    result = std::move(tmp1);
  }
}

void parseTotalQuery(std::string& query, std::vector<std::string>& tokens, std::vector<std::pair<int,int>>& subexpr){
  //query must be enclosed in () for the whole parsing machinery to work
  std::string o = "(";
  std::string c =")";
  query.insert(0,o);
  query.append(c);
  //fix parenthesis formatting
  query = std::regex_replace(query, std::regex("\\( |\\("), "\( ");
  query = std::regex_replace(query, std::regex(" \\)|\\)"), " )" );
  //from query to set of tokens, i.e. split when find a space
  queryToTokens(query, tokens);
  std::vector<int> open;
  std::vector<int> close;
  //find parentesis' index (open and close parenthesis respectively)
  findParenthesis(tokens, open, o);
  findParenthesis(tokens, close, c);
  //find all subexpression indexes, parsing parenthesis' structure
  //traversing in order the open vector
  for(int i=0; i<close.size(); i++){
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
void getListFromEscapeCharacter(std::string& currentTerm, std::vector<std::string>& currentQuery,
                                std::vector<List<int>>& subexprResults, std::vector<List<int>>& cList,
                                List<int>& result, Index& ii){
  std::string subExprEscape = "#";
  std::string cListEscape = "%";
  if(currentTerm.find(subExprEscape) != std::string::npos){
    //need to search in subexpr result
    bool found = false;
    int i = 0;
    while(!found && i<currentQuery.size()){
      std::string e = "#";
      std::string currentEscape = e.append(std::to_string(i));
      if(currentTerm.compare(currentEscape)==0){
        //forse dovrei lasciarla una
        currentQuery.erase(std::remove_if(currentQuery.begin()+(i+1), currentQuery.end(),
          [currentEscape](std::string s){return (s==currentEscape);}), currentQuery.end());
        found = true;
      }
      if(!found) i++;
    }
    std::cout<<"found "<<i<<std::endl;
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
  else ii.getPostingList(currentTerm, result);
}

//answer a query that depends on other subexpression
void parsePartialQuery(std::vector<std::string>& currentQuery, std::vector<List<int>>& subexprResults,
                       List<int>& result, std::vector<List<int>>& cList, Index& ii){
  //remove parenthesis
  currentQuery.erase(std::remove_if(currentQuery.begin(), currentQuery.end(),
    [](std::string s){return (s=="(" || s == ")");}), currentQuery.end());
  //adjust query
  for(int i=0; i<currentQuery.size(); i++){
    std::string escapeTerm = "%";
    std::string currentEscapeTerm = escapeTerm.append(std::to_string(cList.size()));
  //if currentQuery.at(i) is a term or an operator
  if(currentQuery.at(i).find("#") == std::string::npos){
    //if current query is a term
    if(currentQuery.at(i).compare("AND")!=0 && currentQuery.at(i).compare("OR")!=0 && currentQuery.at(i).compare("NOT")!=0){
      //retrieve its posting list
      List<int> currentList{};
      ii.getPostingList(currentQuery.at(i), currentList);
      cList.push_back(currentList);
      currentQuery.at(i) = currentEscapeTerm;
    }
    //if currentQuery.at(i) is an operator
    else if(currentQuery.at(i).compare("AND")==0 || currentQuery.at(i).compare("OR")==0 || currentQuery.at(i).compare("NOT")==0){
      if(currentQuery.at(i+1).compare("NOT")==0){
        //collapsing: AND NOT = ANDNOT, OR NOT = ORNOT
        currentQuery.at(i).append("NOT");
        currentQuery.erase(currentQuery.begin()+(i+1));
      } } }
  }
  //answer query
  List<int> tmp1{}; //temporary result
  int start = 1;
  if(currentQuery.at(0).compare("NOT")==0) {
    start = 2;
    ii.notQuery(currentQuery.at(1), result);
  }
  //handle escape characters
  else getListFromEscapeCharacter(currentQuery.at(0), currentQuery, subexprResults, cList, result, ii);
  std::cout<<"first "<<result<<std::endl;
  for(int i=start; i<currentQuery.size(); i+=2){
    List<int> tmp2{}; //posting list of the current term
    //handle escape characters
    getListFromEscapeCharacter(currentQuery.at(i+1), currentQuery, subexprResults, cList, tmp2, ii);
    std::cout<<"second: "<<tmp2<<std::endl;
    std::cout<<"operator "<<currentQuery.at(0)<<std::endl;
    ii.answerQuery(result, tmp2, currentQuery.at(i), tmp1);
    result = std::move(tmp1);
  }
}

List<int> answer(std::string& query, Index& ii){
  std::vector<std::string> tokens;
  std::vector<std::pair<int,int>> subexpr;
  parseTotalQuery(query, tokens, subexpr);

  std::vector<List<int>> subexprResults;

  for(auto x : subexpr){
    //escape symbol to detect queries that depends on other subqueries
    std::string escape = "#";
    //update escape symbol
    std::string currentEscape = escape.append(std::to_string(subexprResults.size()));
    //query to answer at current stage
    std::vector<std::string> currentQuery;
    //in subexpr we have [i,j], actual token in tokens are [i+1, j-1]
    std::cout<<"currently processing ["<<x.first+1<<","<<x.second-1<<"]"<<std::endl;
    for(int i=x.first+1; i<x.second; i++){
      //compute terms in current query
      currentQuery.push_back(tokens.at(i));
      tokens.at(i) = currentEscape;
    }
    std::cout<<"Processing query: "<<std::endl;
    for(auto x : currentQuery) std::cout<<x<<" ";
    std::cout<<std::endl;
      //List<int> result{};
      //detect if it is an "independent" query or depends on partial results
      bool dependent = false;
      int i = 0;
      while(!dependent && i<currentQuery.size()){
        if(currentQuery.at(i).find("#") != std::string::npos){
          dependent = true;
          }
        i++;
      }
      List<int> result{};
      if(dependent){
        std::cout<<"answering dependent query"<<std::endl;
        std::vector<List<int>> cList{};
        parsePartialQuery(currentQuery, subexprResults, result, cList, ii);
        std::cout<<"Result is: "<<result<<std::endl;
        std::cout<<std::endl;
        subexprResults.push_back(result);
      }
      else if(!dependent){ //parse and resolve
        std::cout<<"answering simple query"<<std::endl;
        answerSimpleQuery(currentQuery, result, ii);
        std::cout<<"Result is "<<result<<std::endl;
        std::cout<<std::endl;
        subexprResults.push_back(result);
        //std::cout<<"partial result: "<<subexprResults.back()<<std::endl;
      }
    }
    return subexprResults.back();
}
#endif
