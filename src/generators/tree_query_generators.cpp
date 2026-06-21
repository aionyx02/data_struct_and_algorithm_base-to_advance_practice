#include "generator_registry.hpp"
#include "generator_support.hpp"
#include "judge/case_generator.hpp"

#include <algorithm>
#include <limits>
#include <queue>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace judge::generators {
namespace {

constexpr int kNodeCount = 9;
constexpr int kEdges[][2] = {
    {1,3},{1,2},{2,5},{2,4},{3,6},{6,8},{6,7},{7,9}
};

struct TreeOracle {
    int n = kNodeCount;
    int log = 4;
    std::vector<std::vector<int>> adjacency;
    std::vector<int> parent,depth,tin,tout,order,subtree,heavy,head,position,inverse;
    std::vector<std::vector<int>> up;
    TreeOracle():adjacency(n+1),parent(n+1),depth(n+1),tin(n+1),tout(n+1),subtree(n+1),heavy(n+1,-1),head(n+1),position(n+1),inverse(n),up(log,std::vector<int>(n+1)){
        for(auto edge:kEdges){adjacency[edge[0]].push_back(edge[1]);adjacency[edge[1]].push_back(edge[0]);}
        for(auto& neighbors:adjacency)std::sort(neighbors.begin(),neighbors.end());
        dfs(1,0);up[0]=parent;for(int level=1;level<log;++level)for(int node=1;node<=n;++node)up[level][node]=up[level-1][up[level-1][node]];int next=0;decompose(1,1,next);
    }
    void dfs(int node,int from){parent[node]=from;depth[node]=from?depth[from]+1:0;tin[node]=static_cast<int>(order.size());order.push_back(node);subtree[node]=1;int best=0;for(int next:adjacency[node])if(next!=from){dfs(next,node);subtree[node]+=subtree[next];if(subtree[next]>best||(subtree[next]==best&&(heavy[node]==-1||next<heavy[node]))){best=subtree[next];heavy[node]=next;}}tout[node]=static_cast<int>(order.size())-1;}
    void decompose(int node,int chainHead,int& next){head[node]=chainHead;position[node]=next;inverse[next++]=node;if(heavy[node]!=-1)decompose(heavy[node],chainHead,next);for(int child:adjacency[node])if(parent[child]==node&&child!=heavy[node])decompose(child,child,next);}
    bool isAncestor(int a,int b)const{return tin[a]<=tin[b]&&tout[b]<=tout[a];}
    int ancestor(int node,int steps)const{for(int level=0;level<log&&node;++level)if(steps&(1<<level))node=up[level][node];return node;}
    int lca(int a,int b)const{if(isAncestor(a,b))return a;if(isAncestor(b,a))return b;for(int level=log-1;level>=0;--level)if(up[level][a]&&!isAncestor(up[level][a],b))a=up[level][a];return parent[a];}
    int distance(int a,int b)const{int common=lca(a,b);return depth[a]+depth[b]-2*depth[common];}
    std::vector<int> path(int a,int b)const{int common=lca(a,b);std::vector<int> left,right;while(a!=common){left.push_back(a);a=parent[a];}left.push_back(common);while(b!=common){right.push_back(b);b=parent[b];}std::reverse(right.begin(),right.end());left.insert(left.end(),right.begin(),right.end());return left;}
    std::vector<int> subtreeNodes(int node)const{return std::vector<int>(order.begin()+tin[node],order.begin()+tout[node]+1);}
};

void writeTree(std::ostringstream& input,int operations){input<<kNodeCount<<' '<<operations<<'\n';for(auto edge:kEdges)input<<edge[0]<<' '<<edge[1]<<'\n';}
void appendBoolean(std::ostringstream& output,bool value){appendLine(output,value?"true":"false");}

GeneratedCase generateEulerTour(std::mt19937_64& random,int operations){
    if (operations < 6) throw std::runtime_error("A45-euler-tour-representation operation_limit is too small");
    TreeOracle tree; std::ostringstream input,output; writeTree(input,operations);
    input<<"euler\n";appendIntList(output,tree.order);input<<"tout 6\n";appendLine(output,tree.tout[6]);input<<"subtree 6\n";appendIntList(output,tree.subtreeNodes(6));input<<"tin 5\n";appendLine(output,tree.tin[5]);input<<"is_ancestor 6 9\n";appendBoolean(output,true);input<<"is_ancestor 2 9\n";appendBoolean(output,false);
    for(int emitted=6;emitted<operations;++emitted){int kind=randomInt(random,0,4),v=randomInt(random,1,tree.n);if(kind==0){input<<"tin "<<v<<'\n';appendLine(output,tree.tin[v]);}else if(kind==1){input<<"tout "<<v<<'\n';appendLine(output,tree.tout[v]);}else if(kind==2){input<<"depth "<<v<<'\n';appendLine(output,tree.depth[v]);}else if(kind==3){int b=randomInt(random,1,tree.n);input<<"is_ancestor "<<v<<' '<<b<<'\n';appendBoolean(output,tree.isAncestor(v,b));}else{input<<"subtree "<<v<<'\n';appendIntList(output,tree.subtreeNodes(v));}}
    return{input.str(),output.str()};
}

GeneratedCase generateBinaryLifting(std::mt19937_64& random,int operations){
    if (operations < 5) throw std::runtime_error("A46-binary-lifting-table operation_limit is too small");
    TreeOracle tree; std::ostringstream input,output; writeTree(input,operations);
    input<<"ancestor 9 0\n";appendLine(output,9);input<<"ancestor 9 4\n";appendLine(output,1);input<<"ancestor 9 5\n";appendLine(output,"NONE");input<<"table 9\n";appendLine(output,"7 6 1 0");input<<"table 8\n";appendLine(output,"6 3 0 0");
    for(int emitted=5;emitted<operations;++emitted){int v=randomInt(random,1,tree.n),kind=randomInt(random,0,2);if(kind==0){int k=randomInt(random,0,7),answer=tree.ancestor(v,k);input<<"ancestor "<<v<<' '<<k<<'\n';if(answer)appendLine(output,answer);else appendLine(output,"NONE");}else if(kind==1){input<<"depth "<<v<<'\n';appendLine(output,tree.depth[v]);}else{input<<"table "<<v<<'\n';std::vector<int> row;for(int level=0;level<tree.log;++level)row.push_back(tree.up[level][v]);appendIntList(output,row);}}
    return{input.str(),output.str()};
}

GeneratedCase generateLca(std::mt19937_64& random,int operations){
    if (operations < 6) throw std::runtime_error("A47-lca-query-structure operation_limit is too small");
    TreeOracle tree; std::ostringstream input,output; writeTree(input,operations);
    input<<"lca 4 9\n";appendLine(output,1);input<<"lca 8 9\n";appendLine(output,6);input<<"distance 4 9\n";appendLine(output,6);input<<"distance 7 8\n";appendLine(output,2);input<<"kth_on_path 4 9 4\n";appendLine(output,6);input<<"is_ancestor 6 9\n";appendBoolean(output,true);
    for(int emitted=6;emitted<operations;++emitted){int a=randomInt(random,1,tree.n),b=randomInt(random,1,tree.n),kind=randomInt(random,0,3);if(kind==0){input<<"lca "<<a<<' '<<b<<'\n';appendLine(output,tree.lca(a,b));}else if(kind==1){input<<"distance "<<a<<' '<<b<<'\n';appendLine(output,tree.distance(a,b));}else if(kind==2){auto path=tree.path(a,b);int k=randomInt(random,0,static_cast<int>(path.size()));input<<"kth_on_path "<<a<<' '<<b<<' '<<k<<'\n';if(k>=static_cast<int>(path.size()))appendLine(output,"OUT_OF_RANGE");else appendLine(output,path[k]);}else{input<<"is_ancestor "<<a<<' '<<b<<'\n';appendBoolean(output,tree.isAncestor(a,b));}}
    return{input.str(),output.str()};
}

GeneratedCase generateHld(std::mt19937_64& random,int operations){
    if (operations < 6) throw std::runtime_error("A48-heavy-light-decomposition operation_limit is too small");
    TreeOracle tree; std::ostringstream input,output; writeTree(input,operations);
    input<<"order\n";appendIntList(output,tree.inverse);input<<"heavy 1\n";appendLine(output,3);input<<"heavy 6\n";appendLine(output,7);input<<"head 9\n";appendLine(output,1);input<<"head 8\n";appendLine(output,8);input<<"position 2\n";appendLine(output,6);
    for(int emitted=6;emitted<operations;++emitted){int v=randomInt(random,1,tree.n),kind=randomInt(random,0,3);if(kind==0){input<<"head "<<v<<'\n';appendLine(output,tree.head[v]);}else if(kind==1){input<<"position "<<v<<'\n';appendLine(output,tree.position[v]);}else if(kind==2){input<<"heavy "<<v<<'\n';if(tree.heavy[v]==-1)appendLine(output,"NONE");else appendLine(output,tree.heavy[v]);}else{input<<"chain "<<v<<'\n';std::vector<int> chain;for(int node=tree.head[v];;node=tree.heavy[node]){chain.push_back(node);if(node==v)break;}appendIntList(output,chain);}}
    return{input.str(),output.str()};
}

GeneratedCase generateHldSegmentTree(std::mt19937_64& random,int operations){
    if (operations < 6) throw std::runtime_error("A49-hld-segment-tree operation_limit is too small");
    TreeOracle tree;std::vector<long long> values(tree.n+1);for(int node=1;node<=tree.n;++node)values[node]=node;std::ostringstream input,output;input<<tree.n<<' '<<operations<<'\n';for(int node=1;node<=tree.n;++node)input<<(node==1?"":" ")<<values[node];input<<'\n';for(auto edge:kEdges)input<<edge[0]<<' '<<edge[1]<<'\n';
    auto pathSum=[&](int a,int b){long long sum=0;for(int node:tree.path(a,b))sum+=values[node];return sum;};auto subtreeSum=[&](int v){long long sum=0;for(int node:tree.subtreeNodes(v))sum+=values[node];return sum;};auto pathAdd=[&](int a,int b,int delta){for(int node:tree.path(a,b))values[node]+=delta;};
    input<<"path_sum 4 9\n";appendLine(output,static_cast<int>(pathSum(4,9)));input<<"subtree_sum 6\n";appendLine(output,static_cast<int>(subtreeSum(6)));input<<"path_add 4 9 10\n";pathAdd(4,9,10);input<<"path_sum 4 9\n";appendLine(output,static_cast<int>(pathSum(4,9)));input<<"subtree_sum 6\n";appendLine(output,static_cast<int>(subtreeSum(6)));input<<"value 4\n";appendLine(output,static_cast<int>(values[4]));
    for(int emitted=6;emitted<operations;++emitted){int kind=randomInt(random,0,4),a=randomInt(random,1,tree.n);if(kind==0){int b=randomInt(random,1,tree.n);input<<"path_sum "<<a<<' '<<b<<'\n';appendLine(output,static_cast<int>(pathSum(a,b)));}else if(kind==1){int b=randomInt(random,1,tree.n),delta=randomInt(random,-5,5);input<<"path_add "<<a<<' '<<b<<' '<<delta<<'\n';pathAdd(a,b,delta);}else if(kind==2){input<<"subtree_sum "<<a<<'\n';appendLine(output,static_cast<int>(subtreeSum(a)));}else if(kind==3){int value=randomInt(random,-20,20);input<<"point_set "<<a<<' '<<value<<'\n';values[a]=value;}else{input<<"value "<<a<<'\n';appendLine(output,static_cast<int>(values[a]));}}
    return{input.str(),output.str()};
}

const std::vector<int> kColors={0,1,2,1,3,2,3,3,2,1};
std::vector<int> subtreeColors(const TreeOracle& tree,int v){std::vector<int> colors;for(int node:tree.subtreeNodes(v))colors.push_back(kColors[node]);std::sort(colors.begin(),colors.end());return colors;}
int distinctCount(std::vector<int> colors){colors.erase(std::unique(colors.begin(),colors.end()),colors.end());return static_cast<int>(colors.size());}
std::pair<int,long long> frequencySummary(const std::vector<int>& colors){int best=0;long long sum=0;for(std::size_t i=0;i<colors.size();){std::size_t j=i;while(j<colors.size()&&colors[j]==colors[i])++j;int count=static_cast<int>(j-i);if(count>best){best=count;sum=colors[i];}else if(count==best)sum+=colors[i];i=j;}return{best,sum};}
int modeColor(const std::vector<int>& colors){int best=-1,mode=0;for(std::size_t i=0;i<colors.size();){std::size_t j=i;while(j<colors.size()&&colors[j]==colors[i])++j;int count=static_cast<int>(j-i);if(count>best){best=count;mode=colors[i];}i=j;}return mode;}
void writeColoredTree(std::ostringstream& input,int operations){input<<kNodeCount<<' '<<operations<<'\n';for(int node=1;node<=kNodeCount;++node)input<<(node==1?"":" ")<<kColors[node];input<<'\n';for(auto edge:kEdges)input<<edge[0]<<' '<<edge[1]<<'\n';}

GeneratedCase generateSmallToLarge(std::mt19937_64& random,int operations){
    if (operations < 5) throw std::runtime_error("A50-small-to-large-merging operation_limit is too small");
    TreeOracle tree; std::ostringstream input,output; writeColoredTree(input,operations);
    input<<"distinct 2\n";appendLine(output,2);input<<"mode 4\n";appendLine(output,3);input<<"distinct 1\n";appendLine(output,3);input<<"count 1 2\n";appendLine(output,3);input<<"mode 1\n";appendLine(output,1);
    for(int emitted=5;emitted<operations;++emitted){int v=randomInt(random,1,tree.n),kind=randomInt(random,0,2);auto colors=subtreeColors(tree,v);if(kind==0){input<<"distinct "<<v<<'\n';appendLine(output,distinctCount(colors));}else if(kind==1){int color=randomInt(random,1,4);input<<"count "<<v<<' '<<color<<'\n';appendLine(output,static_cast<int>(std::count(colors.begin(),colors.end(),color)));}else{input<<"mode "<<v<<'\n';appendLine(output,modeColor(colors));}}
    return{input.str(),output.str()};
}

GeneratedCase generateDsuOnTree(std::mt19937_64& random,int operations){
    if (operations < 5) throw std::runtime_error("A51-dsu-on-tree operation_limit is too small");
    TreeOracle tree; std::ostringstream input,output; writeColoredTree(input,operations);
    input<<"dominant_sum 1\n";appendLine(output,6);input<<"max_frequency 1\n";appendLine(output,3);input<<"dominant_sum 4\n";appendLine(output,3);input<<"distinct 2\n";appendLine(output,2);input<<"dominant_sum 3\n";appendLine(output,4);
    for(int emitted=5;emitted<operations;++emitted){int v=randomInt(random,1,tree.n),kind=randomInt(random,0,2);auto colors=subtreeColors(tree,v);auto summary=frequencySummary(colors);if(kind==0){input<<"distinct "<<v<<'\n';appendLine(output,distinctCount(colors));}else if(kind==1){input<<"max_frequency "<<v<<'\n';appendLine(output,summary.first);}else{input<<"dominant_sum "<<v<<'\n';appendLine(output,static_cast<int>(summary.second));}}
    return{input.str(),output.str()};
}

struct CentroidOracle{const TreeOracle& tree;std::vector<int> blocked,size,parent,level;explicit CentroidOracle(const TreeOracle& value):tree(value),blocked(tree.n+1),size(tree.n+1),parent(tree.n+1),level(tree.n+1){build(1,0,0);}int calculate(int node,int from){size[node]=1;for(int next:tree.adjacency[node])if(next!=from&&!blocked[next])size[node]+=calculate(next,node);return size[node];}int find(int node,int from,int total){for(int next:tree.adjacency[node])if(next!=from&&!blocked[next]&&size[next]>total/2)return find(next,node,total);return node;}void build(int start,int from,int depth){int total=calculate(start,0),center=find(start,0,total);parent[center]=from;level[center]=depth;blocked[center]=1;for(int next:tree.adjacency[center])if(!blocked[next])build(next,center,depth+1);}};

GeneratedCase generateCentroid(std::mt19937_64& random,int operations){
    if (operations < 8) throw std::runtime_error("A52-centroid-decomposition operation_limit is too small");
    TreeOracle tree;CentroidOracle centroid(tree);std::vector<int> active(tree.n+1);std::ostringstream input,output;writeTree(input,operations);
    input<<"centroid_parent 3\n";appendLine(output,"NONE");input<<"centroid_parent 2\n";appendLine(output,3);input<<"centroid_level 9\n";appendLine(output,3);input<<"activate 4\n";active[4]=1;input<<"activate 8\n";active[8]=1;input<<"nearest 9\n";appendLine(output,3);input<<"nearest 5\n";appendLine(output,2);input<<"centroid_parent 1\n";appendLine(output,2);
    for(int emitted=8;emitted<operations;++emitted){int v=randomInt(random,1,tree.n),kind=randomInt(random,0,4);if(kind==0){input<<"centroid_parent "<<v<<'\n';if(centroid.parent[v])appendLine(output,centroid.parent[v]);else appendLine(output,"NONE");}else if(kind==1){input<<"centroid_level "<<v<<'\n';appendLine(output,centroid.level[v]);}else if(kind==2){input<<"activate "<<v<<'\n';active[v]=1;}else if(kind==3){input<<"deactivate "<<v<<'\n';active[v]=0;}else{input<<"nearest "<<v<<'\n';int best=std::numeric_limits<int>::max();for(int node=1;node<=tree.n;++node)if(active[node])best=std::min(best,tree.distance(v,node));if(best==std::numeric_limits<int>::max())appendLine(output,"NONE");else appendLine(output,best);}}
    return{input.str(),output.str()};
}

struct VirtualResult{std::vector<int> nodes;std::vector<std::pair<int,int>> edges;};
VirtualResult virtualTree(const TreeOracle& tree,std::vector<int> requested){std::sort(requested.begin(),requested.end());requested.erase(std::unique(requested.begin(),requested.end()),requested.end());std::vector<int> nodes=requested;for(std::size_t i=1;i<requested.size();++i)nodes.push_back(tree.lca(requested[i-1],requested[i]));std::sort(nodes.begin(),nodes.end());nodes.erase(std::unique(nodes.begin(),nodes.end()),nodes.end());std::vector<std::pair<int,int>> edges;for(int node:nodes){int best=0;for(int candidate:nodes)if(candidate!=node&&tree.isAncestor(candidate,node)&&(best==0||tree.depth[candidate]>tree.depth[best]))best=candidate;if(best)edges.push_back({best,node});}std::sort(edges.begin(),edges.end(),[](auto a,auto b){return a.second<b.second;});return{nodes,edges};}
void appendVirtualEdges(std::ostringstream& output,const std::vector<std::pair<int,int>>& edges){if(edges.empty()){appendLine(output,"EMPTY");return;}for(std::size_t i=0;i<edges.size();++i)output<<(i?" ":"")<<edges[i].first<<'-'<<edges[i].second;output<<'\n';}

GeneratedCase generateVirtualTree(std::mt19937_64& random,int operations){
    if (operations < 5) throw std::runtime_error("A53-virtual-tree operation_limit is too small");
    TreeOracle tree; std::ostringstream input,output; writeTree(input,operations);
    auto emit=[&](const std::string& command,const std::vector<int>& requested){input<<command<<' '<<requested.size();for(int node:requested)input<<' '<<node;input<<'\n';auto result=virtualTree(tree,requested);if(command=="virtual_size")appendLine(output,static_cast<int>(result.nodes.size()));else if(command=="steiner_edges"){int total=0;for(auto edge:result.edges)total+=tree.distance(edge.first,edge.second);appendLine(output,total);}else{appendIntList(output,result.nodes);appendVirtualEdges(output,result.edges);}};
    emit("build",{4,5});emit("virtual_size",{4,4,5});emit("steiner_edges",{4,5,8,9});emit("build",{8,9});emit("virtual_size",{9});
    for(int emitted=5;emitted<operations;++emitted){int count=randomInt(random,1,5);std::vector<int> nodes;for(int i=0;i<count;++i)nodes.push_back(randomInt(random,1,tree.n));int kind=randomInt(random,0,2);emit(kind==0?"build":kind==1?"virtual_size":"steiner_edges",nodes);}
    return{input.str(),output.str()};
}

bool findForestPath(const std::vector<std::vector<int>>& adjacency,int source,int target,std::vector<int>& path){std::vector<int> parent(adjacency.size(),-1);std::queue<int> queue;queue.push(source);parent[source]=0;while(!queue.empty()){int node=queue.front();queue.pop();for(int next:adjacency[node])if(parent[next]==-1){parent[next]=node;queue.push(next);}}if(parent[target]==-1)return false;for(int node=target;;node=parent[node]){path.push_back(node);if(node==source)break;}std::reverse(path.begin(),path.end());return true;}

GeneratedCase generateLinkCut(std::mt19937_64& random,int operations){
    if (operations < 12) throw std::runtime_error("A54-link-cut-tree operation_limit is too small");
    const int n=8;std::vector<long long> value(n+1);for(int node=1;node<=n;++node)value[node]=node;std::vector<std::vector<int>> adjacency(n+1);std::ostringstream input,output;input<<n<<' '<<operations<<'\n';for(int node=1;node<=n;++node)input<<(node==1?"":" ")<<value[node];input<<'\n';
    auto link=[&](int a,int b){input<<"link "<<a<<' '<<b<<'\n';std::vector<int> path;if(findForestPath(adjacency,a,b,path))appendLine(output,"CYCLE");else{adjacency[a].push_back(b);adjacency[b].push_back(a);appendLine(output,"LINKED");}};auto cut=[&](int a,int b){input<<"cut "<<a<<' '<<b<<'\n';auto at=std::find(adjacency[a].begin(),adjacency[a].end(),b);if(at==adjacency[a].end())appendLine(output,"NO_EDGE");else{adjacency[a].erase(at);adjacency[b].erase(std::find(adjacency[b].begin(),adjacency[b].end(),a));appendLine(output,"CUT");}};auto pathSum=[&](int a,int b){input<<"path_sum "<<a<<' '<<b<<'\n';std::vector<int> path;if(!findForestPath(adjacency,a,b,path))appendLine(output,"DISCONNECTED");else{long long sum=0;for(int node:path)sum+=value[node];appendLine(output,static_cast<int>(sum));}};
    link(1,2);link(2,3);link(3,4);pathSum(1,4);input<<"path_add 1 4 5\n";for(int node:{1,2,3,4})value[node]+=5;input<<"value 1\n";appendLine(output,6);cut(1,3);cut(2,3);input<<"connected 1 4\n";appendBoolean(output,false);link(4,5);pathSum(1,5);input<<"value 4\n";appendLine(output,9);
    int emitted=12;while(emitted<operations){int kind=randomInt(random,0,6),a=randomInt(random,1,n),b=randomInt(random,1,n);if(kind==0)link(a,b);else if(kind==1)cut(a,b);else if(kind==2){input<<"connected "<<a<<' '<<b<<'\n';std::vector<int> path;appendBoolean(output,findForestPath(adjacency,a,b,path));}else if(kind==3)pathSum(a,b);else if(kind==4){int delta=randomInt(random,-5,5);input<<"path_add "<<a<<' '<<b<<' '<<delta<<'\n';std::vector<int> path;if(!findForestPath(adjacency,a,b,path))appendLine(output,"DISCONNECTED");else for(int node:path)value[node]+=delta;}else if(kind==5){int x=randomInt(random,-20,20);input<<"set "<<a<<' '<<x<<'\n';value[a]=x;}else{input<<"value "<<a<<'\n';appendLine(output,static_cast<int>(value[a]));}++emitted;}
    return{input.str(),output.str()};
}

}  // namespace

void registerTreeQueryGenerators(CaseGeneratorRegistry& registry){
    registry.emplace("A45-euler-tour-representation",generateEulerTour);
    registry.emplace("A46-binary-lifting-table",generateBinaryLifting);
    registry.emplace("A47-lca-query-structure",generateLca);
    registry.emplace("A48-heavy-light-decomposition",generateHld);
    registry.emplace("A49-hld-segment-tree",generateHldSegmentTree);
    registry.emplace("A50-small-to-large-merging",generateSmallToLarge);
    registry.emplace("A51-dsu-on-tree",generateDsuOnTree);
    registry.emplace("A52-centroid-decomposition",generateCentroid);
    registry.emplace("A53-virtual-tree",generateVirtualTree);
    registry.emplace("A54-link-cut-tree",generateLinkCut);
}

}  // namespace judge::generators
