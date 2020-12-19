#include<bits/stdc++.h>
using namespace std;

vector<string>web_pages;              
map<string,int>Imp;                  
map<string,int>click;
map<string,double>PR;
pair<string, string>key;             
vector<pair<string, string> > V;               

struct Edge {
        string src, dest;
};
vector<Edge>E;

class Graph {
	    vector<vector<string>> adjList;
	    vector<vector<string>> adjList2;
        map<string,int>M;
        map<string,double>out;
        int V;
public:
	  
     Graph(vector<Edge> edge, int N);
     void page_rank();
}; 

Graph::Graph(vector<Edge> edge, int N){ 	
     V=N;
for(int i=0;i<V;i++)M[web_pages[i]]=i;
 
adjList.resize(200); 
      
for (int i=0;i<edge.size();i++){
      out[edge[i].src]++;
      adjList[M[edge[i].dest]].push_back(edge[i].src);  }
	
}
  
void Graph::page_rank(){ 
     
   double A[V][100]={0};
   for(int i=0;i<V;i++)A[i][0]=1/(double)V;                         
          for(int j=1;j<100;j++)
    	     for(int i=0;i<V;i++){
    	     	adjList2=adjList;
    	     	while(adjList2[M[web_pages[i]]].size()!=0){
    	     		string x = adjList2[M[web_pages[i]]].back();	 
					adjList2[M[web_pages[i]]].pop_back();
    	     		A[i][j]+=A[M[x]][j-1]/out[x];}
			 }
         for (int i=0;i<V;i++)PR[web_pages[i]]=A[i][99]; 
}

void initialize(){
	int ctr,clck;
	string link="", line="", str ="";
    fstream WebGraph;
    fstream Keywords;
    fstream Nm_imp;
    fstream clicks;
	WebGraph.open("web_graph.csv", ios::out | ios::in);
	Keywords.open("key_words.csv", ios::out | ios::in);
   	Nm_imp.open  ("Impressions.csv", ios::out | ios::in);
   	clicks.open  ("clicks.csv",ios::out | ios::in);
   
   	while(getline(Nm_imp,line)) { 
      stringstream iss(line); 
      getline(iss, link, ','); 
      getline(iss,str,',');
      if (str.size()==0)break;
      ctr= stoi (str);
      Imp[link]=ctr;
      web_pages.push_back(link);
    }
    
    while(getline(clicks,line)) { 
      stringstream iss(line); 
      getline(iss, link, ','); 
      getline(iss,str,',');
      if (str.size()==0)break;
      clck=stoi(str);
      click[link]=clck;
    }
    
    while(getline(WebGraph,line)) { 
     stringstream iss(line); 
      getline(iss, link, ','); 
      getline(iss,str,',');
      Edge e;
      e.src=link; e.dest=str;
      E.push_back(e);
    }
    
    while(getline(Keywords,line)) { 
     stringstream iss(line);   
     getline(iss, link, ','); 
      for(int i=0;i<1000;i++){
      	string s=str;
        getline(iss,str,','); 
        if (str==s)break;
        key.first=str;
        key.second=link;
        V.push_back(key);
      }
    }
    WebGraph.close(); 
    Keywords.close(); 
    Nm_imp.close();
    clicks.close();
}

void Update_Impression(string link1){
	fstream Nm_imp;
	Nm_imp.open  ("Impressions.csv", ios::in);
	vector<string>temp;
	string link="", line="",str="";
	Imp[link1]++;

	while(getline(Nm_imp,line)) { 
      stringstream iss(line); 
      getline(iss, link, ','); 
      getline(iss,str,',');
      if(link==link1)str=to_string(Imp[link1]);
      temp.push_back(link+","+ str);
    }
    Nm_imp.close();

    Nm_imp.open("Impressions.csv", ios::out | ios::trunc);
    for(const auto& i : temp) {
        Nm_imp << i << std::endl;
    }
    temp.clear();
    Nm_imp.close();
}   
void Update_clicks(string link1){
	fstream clicks;
	clicks.open  ("clicks.csv", ios::in);
	vector<string>temp;
	string link="", line="",str="";
	click[link1]++;
	
    while(getline(clicks,line)) { 
      stringstream iss(line); 
      getline(iss, link, ','); 
      getline(iss,str,',');
      if(link==link1)str=to_string(click[link1]);
      temp.push_back(link+","+ str);
    }
    clicks.close();

    clicks.open("clicks.csv", ios::out | ios::trunc);
    for(const auto& i : temp) {
        clicks << i << std::endl;
    }
    temp.clear();
    clicks.close();
}   

priority_queue<pair<double,string>> score (vector<string>init) {
	
    priority_queue<pair<double,string>>score_q;

	for (int i=0;i<init.size();i++){
		double v1=0.4*PR[init[i]];
		double v2= PR [init[i]]* (1-(0.1*(double)Imp[init[i]])/(1+0.1*(double)Imp[init[i]]));
		double CTR=(double)click[init[i]]/(double)Imp[init[i]];
		double v3= (0.1*(double)Imp[init[i]])/(1+0.1*(double)Imp[init[i]])*CTR;
		double sum1=(v2+v3)*0.6;
		double sum2=v1+sum1;
		pair<double,string>R;
		R.first=sum2;   R.second=init[i];
		score_q.push(R);
	}
	return score_q;
}

priority_queue<pair<double,string>> search_Quo(string key){
	 key.erase (key.begin());
	 key.erase (key.end()-1);
	 vector<string>init;
	 for(int i=0;i<V.size();i++)
		if(V[i].first==key)init.push_back(V[i].second);
	return score(init);
}

priority_queue<pair<double,string>> search_or(string key){
	
    set<string>init;
	string word; 
    stringstream iss(key); 
    while (iss >> word) {
        if (word=="OR")continue;
        else
        for(int i=0;i<V.size();i++) if(V[i].first==word)init.insert(V[i].second);
    }
    vector<string> v(init.size());
    copy(init.begin(), init.end(), v.begin());
    return score(v);
}

priority_queue<pair<double,string>> search_and(string key){
	
    vector<vector<string>>get_common;
	vector<string>init;
	string word; 
    stringstream iss(key); 
    while (iss >> word) {
        if (word=="AND")continue;
        else 
        for(int i=0;i<V.size();i++) if(V[i].first==word)init.push_back(V[i].second);
        if (init.size()==0) exit(0);
        else {get_common.push_back(init);init.clear();}
    }
    vector<string>v1= get_common[0];
    vector<string>v2= get_common[1];
	sort(v1.begin(), v1.end()); 
    sort(v2.begin(), v2.end());
    init.clear();
    vector<string> v(v1.size() + v2.size()); 
    vector<string>::iterator it, st; 
    it = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), v.begin()); 
    for (st = v.begin(); st != it; ++st) init.push_back(*st);
    return score(init);
}

priority_queue<pair<double,string>> search(string key){
		if (key[0]==char(34)) return search_Quo(key);
	    else if (key.find(" AND ")!= string::npos)return search_and(key);
		else return search_or(key); 
}

int main(){
	int n;
	string new_search;
	priority_queue<pair<double,string>> search_results;
	map<int,string>results;
	
	initialize();
	Graph graph(E, web_pages.size());
	graph.page_rank();
	sort(V.begin(),V.end());

while(true){
	cout<<"Welcome \nWhat would you like to do? \n1-New Search  2-Exit"<<endl;
	cin>>n;
	if (n==2)exit(0);
	else{
		cout<<"Search"<<endl;
        std::getline(std::cin >> std::ws, new_search);
		search_results= search(new_search);
		int i=1;
		if (search_results.size()==0){cout<<"############### No Related Results Found #############"<<endl;continue;}
	
		while(search_results.size()!=0){
		   pair<double, string> top = search_results.top();
		   search_results.pop();
		   results[i]=top.second;
		   cout<<i << "- "<<top.second<<endl;
		   Update_Impression(top.second);
		   i++;
		}
		cout<<"1- You want a webpage to open  2-New Search     3-Exit\n";
		cin>>n;
		if (n==3)exit(0);
		else if(n==2)continue;
		else {
			while(true){		
			cout<<"Choose a webpage to open"<<endl;
			cin>>n;
			cout<<"You are viewing "<< results[n]<<endl;
		    Update_clicks(results[n]);
			cout<<"Would you like to\n"<<"1-Back to search results\n2- New search\n3- Exit"<<endl;
			cin>>n;
			if(n==1)continue;
			else if (n==2)break;
			else exit(0);
}}}}}
