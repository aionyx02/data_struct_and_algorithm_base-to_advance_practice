#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#ifndef SPECIALIZED_BUG
#define SPECIALIZED_BUG 0
#endif
namespace specialized_fixture {
#if SPECIALIZED_MODE == 61
int run(){int n,q;std::cin>>n>>q;std::vector<int> initial(n);for(int& x:initial)std::cin>>x;std::vector<std::vector<int>> versions{initial};for(int k=0;k<q;++k){std::string c;std::cin>>c;if(c=="update"){int v,i,x;std::cin>>v>>i>>x;if(SPECIALIZED_BUG==2)v=static_cast<int>(versions.size())-1;auto next=versions[v];next[i]=x;if(SPECIALIZED_BUG==1){for(auto& version:versions)version[i]=x;}versions.push_back(next);std::cout<<versions.size()-1<<'\n';}else if(c=="get"){int v,i;std::cin>>v>>i;std::cout<<versions[v][i]<<'\n';}else if(c=="sum"){int v,l,r;std::cin>>v>>l>>r;long long sum=0;for(int i=l;i<=r;++i)sum+=versions[v][i];std::cout<<sum<<'\n';}else std::cout<<versions.size()<<'\n';}return 0;}
#elif SPECIALIZED_MODE == 62
bool starts(const std::string&a,const std::string&b){return a.size()>=b.size()&&a.compare(0,b.size(),b)==0;}
int run(){int q;std::cin>>q;std::vector<std::vector<std::string>> versions(1);for(int k=0;k<q;++k){std::string c;std::cin>>c;if(c=="insert"){int v;std::string w;std::cin>>v>>w;if(SPECIALIZED_BUG==2)v=static_cast<int>(versions.size())-1;auto next=versions[v];next.push_back(w);if(SPECIALIZED_BUG==1)for(auto& version:versions)version.push_back(w);versions.push_back(next);std::cout<<versions.size()-1<<'\n';}else if(c=="contains"){int v;std::string w;std::cin>>v>>w;std::cout<<(std::find(versions[v].begin(),versions[v].end(),w)!=versions[v].end()?"true":"false")<<'\n';}else if(c=="prefix_count"){int v;std::string p;std::cin>>v>>p;int count=0;for(auto&w:versions[v])if(starts(w,p))++count;std::cout<<count<<'\n';}else std::cout<<versions.size()<<'\n';}return 0;}
#elif SPECIALIZED_MODE == 63
int run(){int q;std::cin>>q;std::vector<std::pair<long long,long long>> lines;for(int k=0;k<q;++k){std::string c;std::cin>>c;if(c=="add_line"){long long m,b;std::cin>>m>>b;lines.push_back({m,b});}else if(c=="query"){long long x;std::cin>>x;if(lines.empty()){std::cout<<"EMPTY\n";continue;}long long answer=SPECIALIZED_BUG==1?-(1LL<<60):(1LL<<60);for(auto [m,b]:lines){long long value=m*x+(SPECIALIZED_BUG==2?0:b);answer=SPECIALIZED_BUG==1?std::max(answer,value):std::min(answer,value);}std::cout<<answer<<'\n';}else lines.clear();}return 0;}
#elif SPECIALIZED_MODE == 64
struct Interval{int id,left,right;};
void print(std::vector<int> ids){std::sort(ids.begin(),ids.end());if(ids.empty()){std::cout<<"EMPTY\n";return;}for(std::size_t i=0;i<ids.size();++i)std::cout<<(i?" ":"")<<ids[i];std::cout<<'\n';}
int run(){int q;std::cin>>q;std::vector<Interval> values;for(int k=0;k<q;++k){std::string c;std::cin>>c;if(c=="add"){int id,l,r;std::cin>>id>>l>>r;auto at=std::find_if(values.begin(),values.end(),[&](auto x){return x.id==id;});if(at!=values.end())std::cout<<"DUPLICATE\n";else values.push_back({id,l,r});}else if(c=="remove"){int id;std::cin>>id;auto at=std::find_if(values.begin(),values.end(),[&](auto x){return x.id==id;});if(at==values.end())std::cout<<"NOT_FOUND\n";else{std::cout<<"REMOVED\n";if(SPECIALIZED_BUG!=2)values.erase(at);}}else if(c=="stab"){int x;std::cin>>x;std::vector<int> ids;for(auto v:values)if(SPECIALIZED_BUG==1?v.left<x&&x<v.right:v.left<=x&&x<=v.right)ids.push_back(v.id);print(ids);}else if(c=="overlap"){int l,r;std::cin>>l>>r;std::vector<int> ids;for(auto v:values)if(SPECIALIZED_BUG==1?v.left<r&&l<v.right:v.left<=r&&l<=v.right)ids.push_back(v.id);print(ids);}else if(c=="count")std::cout<<values.size()<<'\n';else values.clear();}return 0;}
#endif
}int main(){return specialized_fixture::run();}
