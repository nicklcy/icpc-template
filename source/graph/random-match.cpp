// #79. skyline 97
using namespace std;
int n,m,a[505],p[505],ans,u[505],U;
vector<int> v[505];
bool dfs(int x){
    u[x]=U;
    random_shuffle(v[x].begin(),v[x].end());
    for(int i=0;i<v[x].size();++i){
        int y=v[x][i];
        if(!a[y]){
            a[x]=y,a[y]=x;
            return 1;
        }
    }
    for(int i=0;i<v[x].size();++i){
        int y=v[x][i],z=a[y];
        if(u[z]==U) continue;
        a[x]=y,a[y]=x,a[z]=0;
        if(dfs(z)) return 1;
        a[x]=0,a[y]=z,a[z]=y;
    }
    return 0;
}
int main(){
    scanf("%d%d",&n,&m);
    for(int i=1;i<=m;++i){
        int x,y;
        scanf("%d%d",&x,&y);
        v[x].pb(y),v[y].pb(x);
    }
    for(int i=1;i<=n;++i)p[i]=i;
    for(int _=5;_;--_){
        random_shuffle(p+1,p+n+1);
        for(int i=1;i<=n;++i){
            if(a[p[i]]) continue;
            ++U;
            dfs(p[i]);
        }
    }
    for(int i=1;i<=n;++i)if(a[i]>i)++ans;
    printf("%d\n",ans);
    for(int i=1;i<n;++i)printf("%d ",a[i]);
    printf("%d\n",a[n]);
    //system("pause");
    return 0;
}
