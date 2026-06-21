#pragma once
#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#ifndef STRING_INDEX_BUG
#define STRING_INDEX_BUG 0
#endif
namespace string_index_fixture {
void printStrings(std::vector<std::string> values){std::sort(values.begin(),values.end());values.erase(std::unique(values.begin(),values.end()),values.end());if(values.empty()){std::cout<<"EMPTY\n";return;}for(std::size_t i=0;i<values.size();++i)std::cout<<(i?" ":"")<<values[i];std::cout<<'\n';}
bool startsWith(const std::string& value,const std::string& prefix){return value.size()>=prefix.size()&&value.compare(0,prefix.size(),prefix)==0;}
#if STRING_INDEX_MODE == 55 || STRING_INDEX_MODE == 56
int run(){int q;std::cin>>q;std::vector<std::string> words;for(int query=0;query<q;++query){std::string command,word;std::cin>>command;if(command=="size"){std::cout<<words.size()<<'\n';continue;}if(command=="words"){printStrings(words);continue;}std::cin>>word;if(command=="insert"){auto at=std::lower_bound(words.begin(),words.end(),word);
#if STRING_INDEX_MODE == 56
 if(at!=words.end()&&*at==word)std::cout<<"DUPLICATE\n";else words.insert(at,word);
#else
 words.insert(std::upper_bound(words.begin(),words.end(),word),word);
#endif
}else if(command=="erase"){auto first=std::lower_bound(words.begin(),words.end(),word);if(first==words.end()||*first!=word)std::cout<<"NOT_FOUND\n";else{std::cout<<"REMOVED\n";if(STRING_INDEX_BUG==2)words.erase(first,std::upper_bound(words.begin(),words.end(),word));else words.erase(first);}}else if(command=="contains")std::cout<<(std::binary_search(words.begin(),words.end(),word)?"true":"false")<<'\n';else if(command=="count")std::cout<<(std::upper_bound(words.begin(),words.end(),word)-std::lower_bound(words.begin(),words.end(),word))<<'\n';else if(command=="prefix_count"){int count=0;for(const auto& value:words)if(STRING_INDEX_BUG==1?value==word:startsWith(value,word))++count;
#if STRING_INDEX_MODE == 56
 if(STRING_INDEX_BUG==2&&std::binary_search(words.begin(),words.end(),word))--count;
#endif
 std::cout<<count<<'\n';}else if(command=="autocomplete"){std::vector<std::string> result;for(const auto& value:words)if(startsWith(value,word))result.push_back(value);printStrings(result);}else if(command=="longest_prefix"){std::string best;for(const auto& value:words)if(startsWith(word,value)&&value.size()>best.size())best=value;if(STRING_INDEX_BUG==1&&!std::binary_search(words.begin(),words.end(),word))best.clear();if(best.empty())std::cout<<"NONE\n";else std::cout<<best<<'\n';}}return 0;}
#elif STRING_INDEX_MODE == 57
int run(){int q;std::cin>>q;std::vector<unsigned> values;for(int query=0;query<q;++query){std::string command;std::cin>>command;if(command=="size"){std::cout<<values.size()<<'\n';continue;}unsigned x;std::cin>>x;if(command=="insert")values.push_back(x);else if(command=="erase_one"){auto at=std::find(values.begin(),values.end(),x);if(at==values.end())std::cout<<"NOT_FOUND\n";else{std::cout<<"REMOVED\n";if(STRING_INDEX_BUG==2)values.erase(std::remove(values.begin(),values.end(),x),values.end());else values.erase(at);}}else if(command=="count")std::cout<<std::count(values.begin(),values.end(),x)<<'\n';else{if(values.empty()){std::cout<<"EMPTY\n";continue;}auto better=[&](unsigned a,unsigned b){return command=="max_xor"?(a^x)<(b^x):(a^x)>(b^x);};unsigned chosen=*std::max_element(values.begin(),values.end(),better);std::cout<<(STRING_INDEX_BUG==1&&command=="max_xor"?chosen:(chosen^x))<<'\n';}}return 0;}
#elif STRING_INDEX_MODE == 58
int occurrences(const std::string& text,const std::string& pattern,bool nonOverlap){int count=0;std::size_t position=0;while(position<=text.size()){auto found=text.find(pattern,position);if(found==std::string::npos)break;++count;position=found+(nonOverlap?pattern.size():1);}return count;}
int run(){int p,q;std::cin>>p>>q;std::vector<std::string> patterns(p);for(auto& pattern:patterns)std::cin>>pattern;for(int query=0;query<q;++query){std::string command,text;std::cin>>command>>text;int total=0,distinct=0;for(const auto& pattern:patterns){int count=occurrences(text,pattern,STRING_INDEX_BUG==1);if(STRING_INDEX_BUG==2&&text.find(pattern)!=0)count=0;total+=count;if(count)++distinct;}std::cout<<(command=="count"?total:distinct)<<'\n';}return 0;}
#elif STRING_INDEX_MODE == 59
std::vector<int> suffixArray(const std::string& text){std::vector<int> order(text.size());for(int i=0;i<static_cast<int>(text.size());++i)order[i]=i;std::sort(order.begin(),order.end(),[&](int a,int b){return text.substr(a)<text.substr(b);});return order;}
int lcp(const std::string& text,int a,int b){int count=0;while(a+count<static_cast<int>(text.size())&&b+count<static_cast<int>(text.size())&&text[a+count]==text[b+count])++count;return count;}
void printInts(const std::vector<int>& values){for(std::size_t i=0;i<values.size();++i)std::cout<<(i?" ":"")<<values[i]+(STRING_INDEX_BUG==1?1:0);std::cout<<'\n';}
int run(){std::string text;int q;std::cin>>text>>q;auto sa=suffixArray(text);std::vector<int> rank(text.size());for(int i=0;i<static_cast<int>(sa.size());++i)rank[sa[i]]=i;for(int query=0;query<q;++query){std::string command;std::cin>>command;if(command=="suffix_array")printInts(sa);else if(command=="lcp_array"){std::vector<int> values;for(std::size_t i=1;i<sa.size();++i)values.push_back(lcp(text,sa[i-1],sa[i]));printInts(values);}else if(command=="rank"){int i;std::cin>>i;std::cout<<rank[i]+(STRING_INDEX_BUG==1?1:0)<<'\n';}else if(command=="lcp"){int a,b;std::cin>>a>>b;std::cout<<(STRING_INDEX_BUG==2&&a!=b?lcp(text,sa[std::min(rank[a],rank[b])],sa[std::min(rank[a],rank[b])+1]):lcp(text,a,b))<<'\n';}else{int a,b,c,d;std::cin>>a>>b>>c>>d;std::string left=text.substr(a,b-a+1),right=text.substr(c,d-c+1);std::cout<<(left<right?"<":left>right?">":"=")<<'\n';}}return 0;}
#elif STRING_INDEX_MODE == 60
long long distinctSubstrings(const std::string& text){std::set<std::string> values;for(std::size_t i=0;i<text.size();++i)for(std::size_t length=1;i+length<=text.size();++length)values.insert(text.substr(i,length));return values.size();}
int occurrences(const std::string& text,const std::string& pattern){int count=0;for(std::size_t i=0;i+pattern.size()<=text.size();++i)if(text.compare(i,pattern.size(),pattern)==0)++count;return count;}
int longestRepeated(const std::string& text){int best=0;for(std::size_t i=0;i<text.size();++i)for(std::size_t j=i+1;j<text.size();++j){int length=0;while(j+length<text.size()&&text[i+length]==text[j+length])++length;best=std::max(best,length);}return best;}
int run(){int q;std::cin>>q;std::string text;for(int query=0;query<q;++query){std::string command;std::cin>>command;if(command=="append"){char c;std::cin>>c;text+=c;}else if(command=="distinct_substrings")std::cout<<distinctSubstrings(text)+(STRING_INDEX_BUG==2?1:0)<<'\n';else if(command=="longest_repeated")std::cout<<longestRepeated(text)<<'\n';else{std::string pattern;std::cin>>pattern;if(command=="contains")std::cout<<((STRING_INDEX_BUG==1?text.size()>=pattern.size()&&text.compare(text.size()-pattern.size(),pattern.size(),pattern)==0:text.find(pattern)!=std::string::npos)?"true":"false")<<'\n';else std::cout<<occurrences(text,pattern)<<'\n';}}return 0;}
#endif
}
int main(){return string_index_fixture::run();}
