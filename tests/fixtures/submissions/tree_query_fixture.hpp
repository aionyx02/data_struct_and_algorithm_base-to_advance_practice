#pragma once

#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <string>
#include <vector>

#ifndef TREE_QUERY_BUG
#define TREE_QUERY_BUG 0
#endif

namespace tree_query_fixture {

void printList(const std::vector<int>& values) {
    if (values.empty()) { std::cout << "EMPTY\n"; return; }
    for (std::size_t i=0;i<values.size();++i) std::cout << (i?" ":"") << values[i];
    std::cout << '\n';
}

struct StaticTree {
    int n;
    int log;
    std::vector<std::vector<int>> adjacency;
    std::vector<int> parent,depth,tin,tout,order,subtree,heavy,head,position,inverse;
    std::vector<std::vector<int>> up;

    explicit StaticTree(int nodeCount):n(nodeCount),log(1),adjacency(n+1),parent(n+1),depth(n+1),tin(n+1),tout(n+1),subtree(n+1),heavy(n+1,-1),head(n+1),position(n+1),inverse(n) {
        while ((1<<log)<=n) ++log;
        up.assign(log,std::vector<int>(n+1));
    }
    void addEdge(int a,int b){adjacency[a].push_back(b);adjacency[b].push_back(a);}
    void prepare(bool reverseOrder=false,bool wrongHeavy=false,bool noHeavyChains=false){
        for(auto& neighbors:adjacency){std::sort(neighbors.begin(),neighbors.end());if(reverseOrder)std::reverse(neighbors.begin(),neighbors.end());}
        order.clear();dfs(1,0);
        up[0]=parent;for(int level=1;level<log;++level)for(int node=1;node<=n;++node)up[level][node]=up[level-1][up[level-1][node]];
        if(wrongHeavy){for(int node=1;node<=n;++node){heavy[node]=-1;for(int child:adjacency[node])if(parent[child]==node){heavy[node]=child;break;}}}
        int next=0;decompose(1,1,next,noHeavyChains);
    }
    void dfs(int node,int from){
        parent[node]=from;depth[node]=from?depth[from]+1:0;tin[node]=static_cast<int>(order.size());order.push_back(node);subtree[node]=1;int bestSize=0;
        for(int next:adjacency[node])if(next!=from){dfs(next,node);subtree[node]+=subtree[next];if(subtree[next]>bestSize||(subtree[next]==bestSize&&(heavy[node]==-1||next<heavy[node]))){bestSize=subtree[next];heavy[node]=next;}}
        tout[node]=static_cast<int>(order.size())-1;
    }
    void decompose(int node,int chainHead,int& nextPosition,bool noHeavyChains){
        head[node]=chainHead;position[node]=nextPosition;inverse[nextPosition++]=node;
        if(heavy[node]!=-1)decompose(heavy[node],noHeavyChains?heavy[node]:chainHead,nextPosition,noHeavyChains);
        for(int child:adjacency[node])if(parent[child]==node&&child!=heavy[node])decompose(child,child,nextPosition,noHeavyChains);
    }
    bool isAncestor(int a,int b)const{return tin[a]<=tin[b]&&tout[b]<=tout[a];}
    int ancestor(int node,int steps)const{for(int level=0;level<log&&node;++level)if(steps&(1<<level))node=up[level][node];return node;}
    int lca(int a,int b)const{if(isAncestor(a,b))return a;if(isAncestor(b,a))return b;for(int level=log-1;level>=0;--level)if(up[level][a]&&!isAncestor(up[level][a],b))a=up[level][a];return parent[a];}
    int distance(int a,int b)const{int common=lca(a,b);return depth[a]+depth[b]-2*depth[common];}
    std::vector<int> path(int a,int b)const{int common=lca(a,b);std::vector<int> left,right;while(a!=common){left.push_back(a);a=parent[a];}left.push_back(common);while(b!=common){right.push_back(b);b=parent[b];}std::reverse(right.begin(),right.end());left.insert(left.end(),right.begin(),right.end());return left;}
    std::vector<int> subtreeNodes(int node,bool dropLast=false)const{int right=tout[node]-(dropLast?1:0);std::vector<int> result;for(int index=tin[node];index<=right;++index)result.push_back(order[index]);return result;}
};

StaticTree readTree(int& queryCount,bool reverseOrder=false,bool wrongHeavy=false,bool noHeavyChains=false){int n;std::cin>>n>>queryCount;StaticTree tree(n);for(int edge=1;edge<n;++edge){int a,b;std::cin>>a>>b;tree.addEdge(a,b);}tree.prepare(reverseOrder,wrongHeavy,noHeavyChains);return tree;}

#if TREE_QUERY_MODE == 45
int run(){int q;StaticTree tree=readTree(q,TREE_QUERY_BUG==2);for(int query=0;query<q;++query){std::string command;std::cin>>command;
    if(command=="tin"){int v;std::cin>>v;std::cout<<tree.tin[v]<<'\n';}
    else if(command=="tout"){int v;std::cin>>v;std::cout<<tree.tout[v]+(TREE_QUERY_BUG==1?1:0)<<'\n';}
    else if(command=="depth"){int v;std::cin>>v;std::cout<<tree.depth[v]<<'\n';}
    else if(command=="is_ancestor"){int a,b;std::cin>>a>>b;std::cout<<(tree.isAncestor(a,b)?"true":"false")<<'\n';}
    else if(command=="subtree"){int v;std::cin>>v;printList(tree.subtreeNodes(v));}
    else if(command=="euler")printList(tree.order);
}return 0;}
#elif TREE_QUERY_MODE == 46
int run(){int q;StaticTree tree=readTree(q);for(int query=0;query<q;++query){std::string command;std::cin>>command;
    if(command=="ancestor"){int v,k;std::cin>>v>>k;int result=tree.ancestor(v,k+(TREE_QUERY_BUG==1?1:0));if(result)std::cout<<result<<'\n';else std::cout<<"NONE\n";}
    else if(command=="depth"){int v;std::cin>>v;std::cout<<tree.depth[v]<<'\n';}
    else if(command=="table"){int v;std::cin>>v;std::vector<int> values;for(int level=0;level<tree.log;++level){int value=TREE_QUERY_BUG==2?tree.ancestor(v,level+1):tree.up[level][v];values.push_back(value);}printList(values);}
}return 0;}
#elif TREE_QUERY_MODE == 47
int brokenLca(const StaticTree& tree,int a,int b){while(tree.depth[a]>tree.depth[b])a=tree.parent[a];while(tree.depth[b]>tree.depth[a])b=tree.parent[b];return a==b?a:tree.parent[a];}
int run(){int q;StaticTree tree=readTree(q);for(int query=0;query<q;++query){std::string command;std::cin>>command;
    if(command=="lca"){int a,b;std::cin>>a>>b;std::cout<<(TREE_QUERY_BUG==1?brokenLca(tree,a,b):tree.lca(a,b))<<'\n';}
    else if(command=="distance"){int a,b;std::cin>>a>>b;std::cout<<tree.distance(a,b)+(TREE_QUERY_BUG==2?1:0)<<'\n';}
    else if(command=="kth_on_path"){int a,b,k;std::cin>>a>>b>>k;auto path=tree.path(a,b);if(k<0||k>=static_cast<int>(path.size()))std::cout<<"OUT_OF_RANGE\n";else std::cout<<path[k]<<'\n';}
    else if(command=="is_ancestor"){int a,b;std::cin>>a>>b;std::cout<<(tree.isAncestor(a,b)?"true":"false")<<'\n';}
}return 0;}
#elif TREE_QUERY_MODE == 48
int run(){int q;StaticTree tree=readTree(q,false,TREE_QUERY_BUG==1,TREE_QUERY_BUG==2);for(int query=0;query<q;++query){std::string command;std::cin>>command;
    if(command=="head"){int v;std::cin>>v;std::cout<<tree.head[v]<<'\n';}
    else if(command=="position"){int v;std::cin>>v;std::cout<<tree.position[v]<<'\n';}
    else if(command=="heavy"){int v;std::cin>>v;if(tree.heavy[v]==-1)std::cout<<"NONE\n";else std::cout<<tree.heavy[v]<<'\n';}
    else if(command=="chain"){int v;std::cin>>v;std::vector<int> values;for(int node=tree.head[v];;node=tree.heavy[node]){values.push_back(node);if(node==v)break;}printList(values);}
    else if(command=="order")printList(tree.inverse);
}return 0;}
#elif TREE_QUERY_MODE == 49
int run(){int n,q;std::cin>>n>>q;std::vector<long long> values(n+1);for(int i=1;i<=n;++i)std::cin>>values[i];StaticTree tree(n);for(int e=1;e<n;++e){int a,b;std::cin>>a>>b;tree.addEdge(a,b);}tree.prepare();for(int query=0;query<q;++query){std::string command;std::cin>>command;
    if(command=="path_sum"){int a,b;std::cin>>a>>b;auto nodes=tree.path(a,b);long long sum=0;for(std::size_t i=TREE_QUERY_BUG==1?1:0;i<nodes.size();++i)sum+=values[nodes[i]];std::cout<<sum<<'\n';}
    else if(command=="path_add"){int a,b;long long delta;std::cin>>a>>b>>delta;auto nodes=tree.path(a,b);for(std::size_t i=TREE_QUERY_BUG==1?1:0;i<nodes.size();++i)values[nodes[i]]+=delta;}
    else if(command=="subtree_sum"){int v;std::cin>>v;long long sum=0;for(int node:tree.subtreeNodes(v,TREE_QUERY_BUG==2))sum+=values[node];std::cout<<sum<<'\n';}
    else if(command=="point_set"){int v;long long value;std::cin>>v>>value;values[v]=value;}
    else if(command=="value"){int v;std::cin>>v;std::cout<<values[v]<<'\n';}
}return 0;}
#elif TREE_QUERY_MODE == 50 || TREE_QUERY_MODE == 51
int run(){int n,q;std::cin>>n>>q;std::vector<int> color(n+1);for(int i=1;i<=n;++i)std::cin>>color[i];StaticTree tree(n);for(int e=1;e<n;++e){int a,b;std::cin>>a>>b;tree.addEdge(a,b);}tree.prepare();for(int query=0;query<q;++query){std::string command;std::cin>>command;int v;std::cin>>v;auto nodes=tree.subtreeNodes(v);if(TREE_QUERY_BUG==2&&!nodes.empty())nodes.erase(nodes.begin());std::vector<int> colors;for(int node:nodes)colors.push_back(color[node]);std::sort(colors.begin(),colors.end());
#if TREE_QUERY_MODE == 50
    if(command=="distinct"){if(TREE_QUERY_BUG==1){int best=0;for(int child:tree.adjacency[v])if(tree.parent[child]==v){auto childNodes=tree.subtreeNodes(child);std::set<int> childColors;for(int node:childNodes)childColors.insert(color[node]);best=std::max(best,static_cast<int>(childColors.size()));}std::cout<<best<<'\n';}else{colors.erase(std::unique(colors.begin(),colors.end()),colors.end());std::cout<<colors.size()<<'\n';}}
    else if(command=="count"){int target;std::cin>>target;std::cout<<std::count(colors.begin(),colors.end(),target)<<'\n';}
    else if(command=="mode"){int bestColor=0,bestCount=-1;for(std::size_t i=0;i<colors.size();){std::size_t j=i;while(j<colors.size()&&colors[j]==colors[i])++j;int count=static_cast<int>(j-i);if(count>bestCount||(count==bestCount&&((TREE_QUERY_BUG==2&&colors[i]>bestColor)||(TREE_QUERY_BUG!=2&&colors[i]<bestColor)))){bestCount=count;bestColor=colors[i];}i=j;}std::cout<<bestColor<<'\n';}
#else
    if(command=="distinct"){colors.erase(std::unique(colors.begin(),colors.end()),colors.end());std::cout<<colors.size()<<'\n';}
    else{int best=0;long long dominantSum=0;for(std::size_t i=0;i<colors.size();){std::size_t j=i;while(j<colors.size()&&colors[j]==colors[i])++j;int count=static_cast<int>(j-i);if(count>best){best=count;dominantSum=colors[i];}else if(count==best)dominantSum+=colors[i];i=j;}if(command=="max_frequency")std::cout<<best<<'\n';else std::cout<<(TREE_QUERY_BUG==1&&best?*std::lower_bound(colors.begin(),colors.end(),colors.front()):dominantSum)<<'\n';}
#endif
}return 0;}
#elif TREE_QUERY_MODE == 52
struct CentroidData{StaticTree& tree;std::vector<int> blocked,size,parent,level;CentroidData(StaticTree& t):tree(t),blocked(t.n+1),size(t.n+1),parent(t.n+1),level(t.n+1){}
 int calculate(int node,int from){size[node]=1;for(int next:tree.adjacency[node])if(next!=from&&!blocked[next])size[node]+=calculate(next,node);return size[node];}
 int find(int node,int from,int total){if(TREE_QUERY_BUG==1)return node;for(int next:tree.adjacency[node])if(next!=from&&!blocked[next]&&size[next]>total/2)return find(next,node,total);return node;}
 void build(int start,int from,int depth){int total=calculate(start,0);int center=find(start,0,total);parent[center]=from;level[center]=depth;blocked[center]=1;for(int next:tree.adjacency[center])if(!blocked[next])build(next,center,depth+1);}
};
int run(){int q;StaticTree tree=readTree(q);CentroidData data(tree);data.build(1,0,0);std::vector<int> active(tree.n+1);for(int query=0;query<q;++query){std::string command;int v;std::cin>>command>>v;if(command=="centroid_parent"){if(data.parent[v])std::cout<<data.parent[v]<<'\n';else std::cout<<"NONE\n";}else if(command=="centroid_level")std::cout<<data.level[v]<<'\n';else if(command=="activate")active[v]=1;else if(command=="deactivate")active[v]=0;else{int best=std::numeric_limits<int>::max();if(TREE_QUERY_BUG==2){for(int node=1;node<=tree.n;++node)if(active[node]){best=tree.distance(v,node);break;}}else for(int node=1;node<=tree.n;++node)if(active[node])best=std::min(best,tree.distance(v,node));if(best==std::numeric_limits<int>::max())std::cout<<"NONE\n";else std::cout<<best<<'\n';}}return 0;}
#elif TREE_QUERY_MODE == 53
int run(){int q;StaticTree tree=readTree(q);for(int query=0;query<q;++query){std::string command;int k;std::cin>>command>>k;std::vector<int> requested(k);for(int& node:requested)std::cin>>node;std::sort(requested.begin(),requested.end());if(TREE_QUERY_BUG!=2)requested.erase(std::unique(requested.begin(),requested.end()),requested.end());std::vector<int> nodes=requested;if(TREE_QUERY_BUG!=1){for(std::size_t i=1;i<requested.size();++i)nodes.push_back(tree.lca(requested[i-1],requested[i]));std::sort(nodes.begin(),nodes.end());if(TREE_QUERY_BUG!=2)nodes.erase(std::unique(nodes.begin(),nodes.end()),nodes.end());}std::vector<std::pair<int,int>> edges;for(int node:nodes){int best=0;for(int candidate:nodes)if(candidate!=node&&tree.isAncestor(candidate,node)&&(best==0||tree.depth[candidate]>tree.depth[best]))best=candidate;if(best)edges.push_back({best,node});}if(command=="virtual_size")std::cout<<nodes.size()<<'\n';else if(command=="steiner_edges"){int total=0;for(auto [a,b]:edges)total+=tree.distance(a,b);std::cout<<total<<'\n';}else{printList(nodes);if(edges.empty())std::cout<<"EMPTY\n";else{std::sort(edges.begin(),edges.end(),[](auto a,auto b){return a.second<b.second;});for(std::size_t i=0;i<edges.size();++i)std::cout<<(i?" ":"")<<edges[i].first<<'-'<<edges[i].second;std::cout<<'\n';}}}return 0;}
#elif TREE_QUERY_MODE == 54
bool findPath(const std::vector<std::vector<int>>& adjacency,int source,int target,std::vector<int>& path){std::vector<int> parent(adjacency.size(),-1);std::queue<int> queue;queue.push(source);parent[source]=0;while(!queue.empty()){int node=queue.front();queue.pop();for(int next:adjacency[node])if(parent[next]==-1){parent[next]=node;queue.push(next);}}if(parent[target]==-1)return false;for(int node=target;node;node=parent[node]){path.push_back(node);if(node==source)break;}std::reverse(path.begin(),path.end());return true;}
int run(){int n,q;std::cin>>n>>q;std::vector<long long> value(n+1);for(int i=1;i<=n;++i)std::cin>>value[i];std::vector<std::vector<int>> adjacency(n+1);for(int query=0;query<q;++query){std::string command;std::cin>>command;if(command=="link"){int a,b;std::cin>>a>>b;std::vector<int> path;if(findPath(adjacency,a,b,path))std::cout<<"CYCLE\n";else{adjacency[a].push_back(b);adjacency[b].push_back(a);std::cout<<"LINKED\n";}}else if(command=="cut"){int a,b;std::cin>>a>>b;auto at=std::find(adjacency[a].begin(),adjacency[a].end(),b);if(at==adjacency[a].end()&&TREE_QUERY_BUG==2){std::vector<int> path;if(findPath(adjacency,a,b,path)&&path.size()>1){b=path[1];at=std::find(adjacency[a].begin(),adjacency[a].end(),b);}}if(at==adjacency[a].end())std::cout<<"NO_EDGE\n";else{adjacency[a].erase(at);adjacency[b].erase(std::find(adjacency[b].begin(),adjacency[b].end(),a));std::cout<<"CUT\n";}}else if(command=="connected"){int a,b;std::cin>>a>>b;std::vector<int> path;std::cout<<(findPath(adjacency,a,b,path)?"true":"false")<<'\n';}else if(command=="path_sum"){int a,b;std::cin>>a>>b;std::vector<int> path;if(!findPath(adjacency,a,b,path))std::cout<<"DISCONNECTED\n";else{long long sum=0;for(std::size_t i=TREE_QUERY_BUG==1?1:0;i<path.size();++i)sum+=value[path[i]];std::cout<<sum<<'\n';}}else if(command=="path_add"){int a,b;long long delta;std::cin>>a>>b>>delta;std::vector<int> path;if(!findPath(adjacency,a,b,path))std::cout<<"DISCONNECTED\n";else for(std::size_t i=TREE_QUERY_BUG==1?1:0;i<path.size();++i)value[path[i]]+=delta;}else if(command=="set"){int v;long long x;std::cin>>v>>x;value[v]=x;}else{int v;std::cin>>v;std::cout<<value[v]<<'\n';}}return 0;}
#endif

}  // namespace tree_query_fixture
int main(){return tree_query_fixture::run();}
