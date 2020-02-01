#include<bits/stdc++.h>
//#define DEBUG
using namespace std;
void here(){
    cout<<"here"<<endl;
}
class clause{
    public:
        bool sat=false,tautology=false;
        int totalLiterals=0;
        unordered_set<int>literals;  
        clause(){}
        clause(int unitLiteral){
            literals.insert(unitLiteral);
        }
        inline void addLiteral(int x){
            totalLiterals++;
            if(x<0){
                literals.insert(-2*x-1);
                tautology|=(literals.find(-2*x)!=literals.end());
            }
            else{
                literals.insert(2*x);
                tautology|=(literals.find(2*x-1)!=literals.end());
            }
        }
        friend ostream& operator<<(ostream& os, const clause& cs){
            for(auto lit:cs.literals)
                os<<lit<<" "; 
            return os;
        }
};
class clauseSet{
    private:
        int numClauses,numLiterals;    
        vector <bool> visited;
    public:
        vector <clause> clauses;
        vector <int> countClause;
        vector <bool> satClause;
        multimap<int,int> literalMap;
        vector <int> unitClauses;

        clauseSet(int numliteral,int numclause):numClauses(numclause),numLiterals(numliteral){      
            countClause.reserve(numClauses+5);
            visited.reserve(2*(numLiterals)+5);
            countClause.push_back(0);
            satClause.push_back(false);
            clauses.push_back(clause(0));
            satClause.reserve(numClauses+5);          
        }
        void addClause(clause cs,bool set){
            if(cs.tautology)
                return;
            else{
                clauses.push_back(cs);
                satClause.push_back(false);
                countClause.push_back(cs.totalLiterals);
                if(cs.literals.size() == 1){
                    unitClauses.push_back(*(cs.literals.begin()));
                }
                if(set){
                    for(auto lit:cs.literals){
                        literalMap.insert(make_pair(lit,countClause.size()-1));
                    }
                }
            }
        }
        void pureLiteralElim(){
            for(int i=1;i<2*numLiterals;i+=2){
                if(literalMap.count(i) && literalMap.count(i+1)==0){
                     unitClauses.push_back(i);
                }
                else if(literalMap.count(i+1) && literalMap.count(i)==0){
                    unitClauses.push_back(i+1);
                }
            }
        }
        void printLiteral(int lit){
            auto it = literalMap.equal_range(lit); 
            for(auto itr=it.first;itr!=it.second;++itr)
                cout<<itr->first<<" "<<itr->second<<endl;;
            cout<<endl;
        }
};
inline int complement(int i){
    if(i&1)
        return i+1;
    else
        return i-1;
}
inline int getvariable(int i){
    if(i&1)
        return (i+1)/2;
    else
        return i/2;
}
unordered_set<int>finalAssignment;
class solver{
    private:
        int totalClauses,totalVariables;
    public: 
        solver(int variables,int clauses):totalClauses(clauses),totalVariables(variables){}
        bool dpll(clauseSet clauseset,unordered_set<int>assigned,unordered_set<int>done,int satisfiedClauses){ 
                //unit propogation      
           // cout<<clauseset.unitClauses.size()<<"##"<<endl;
            for(int i=0;i<clauseset.unitClauses.size();i++){
                int unitLiteral=clauseset.unitClauses[i];
                //cout<<"???"<<unitLiteral<<endl;
                assigned.insert(unitLiteral);
                done.erase(getvariable(unitLiteral));
                // done.erase(complement(unitLiteral));
                auto it = clauseset.literalMap.equal_range(unitLiteral); 
                for(auto clauseNum=it.first;clauseNum!=it.second;++clauseNum){
                    //cout<<clauseset.satClause[clauseNum->second]<<" **"<<clauseNum->second<<endl;
                    if(!clauseset.satClause[clauseNum->second]){
                        clauseset.satClause[clauseNum->second]=true;
                        // cout<<clauseNum->second<<" true"<<endl;
                        if(clauseNum->second <= totalClauses)
                            satisfiedClauses++;
                        // for(auto l:assigned)
                        //     cout<<l<<" ";
                        // cout<<endl;
                        // cout<<satisfiedClauses<<" "<<totalClauses<<" "<<assigned.size()<<"??"<<endl;
                        if(satisfiedClauses==totalClauses){
                            finalAssignment=assigned;
                            return true;
                        }
                    }
                }              
                int compUnitLiteral=complement(unitLiteral);
                //cout<<compUnitLiteral<<endl;
                it = clauseset.literalMap.equal_range(compUnitLiteral); 
                for(auto clauseNum=it.first;clauseNum!=it.second;++clauseNum){
                    // clauseset.clauses[clauseNum->second].literals.erase(compUnitLiteral);
                    clauseset.countClause[clauseNum->second]--;
                    if(clauseset.countClause[clauseNum->second]==1 && clauseset.satClause[clauseNum->second]==false){
                        for(auto lit: clauseset.clauses[clauseNum->second].literals){
                            if(assigned.find(lit)==assigned.end() && assigned.find(complement(lit))==assigned.end())
                                 clauseset.unitClauses.push_back(lit);
                        }
                    }
                    else if(clauseset.countClause[clauseNum->second]==0){
                        //cout<<"UNSATISFIABLE"<<endl;
                        return false;            
                    }
                }     
            }       
            clauseset.unitClauses.clear();    
            //clauseset.print();
            // chose pivot element implement heuristic here
            int pivot = *(done.begin());
            
            // pair<int,int> optLit;
            // for(auto lit:done){
            //     optLit=max(optLit,{clauseset.literalMap.count(lit),lit});
            // }
            // int selectedLiteral=optLit.second;
            int selectedLiteral = 2*pivot;
            //cout<<selectedLiteral<<"??"<<endl;
            clauseset.unitClauses.push_back(selectedLiteral);
            if(dpll(clauseset,assigned,done,satisfiedClauses))
                return true;
            clauseset.unitClauses.pop_back();
            clauseset.unitClauses.push_back(complement(selectedLiteral));
            return dpll(clauseset,assigned,done,satisfiedClauses);            
        }
    // assignment.reserve(2*(numLiterals+5));
};
int main(){
    auto start = chrono::high_resolution_clock::now(); 
    string strOneLine,str;
    int n,m,inp,clauseId=0;
    char ch; 
    cin>>ch;
    while (ch=='c'){
        getline(cin,strOneLine);
        cin>>ch;
    }
    cin>>str>>n>>m;
    unordered_set<int>done;
    unordered_set<int>assigned;
    vector <int> vec;
    for(int i=1;i<=n;i++)
        vec.push_back(i);
    //random_shuffle(vec.begin(),vec.end());
    for(auto k:vec)
        done.insert(k);
    clauseSet clauses(n,m);
    vector<int>input;
    while(cin>>inp){
        if(inp==0){
            clause cl;
            for(auto literal:input)
                cl.addLiteral(literal);
            clauses.addClause(cl,true);
            input.clear();
        }else{
            input.push_back(inp);
        }    
    }
    // clauses.printLiteral(8);
    // cout<<"?????????"<<endl;
    // vector <pair<int,int>> vec;
    // for(int i=1;i<=2*n;i++)
    //     vec.push_back({clauses.literalMap.count(i),i});    
    // sort(vec.begin(),vec.end(),greater<pair<int,int>>());
    // for(auto k:vec)
    //     done.insert(k.second);
    clauses.pureLiteralElim();
    solver dpllsolver(n,m);
    cout<<dpllsolver.dpll(clauses,assigned,done,0)<<endl;
    for(auto k:finalAssignment)
        cout<<k<<" ";
    cout<<endl;
    #ifdef DEBUG
    for(auto k:clauses.unitClauses)
        cout<<k<<" ";
    cout<<endl;
    clauses.print();
    clauses.printLiteral(6);
    #endif
 auto end = chrono::high_resolution_clock::now(); 
    double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count(); 
  
    time_taken *= 1e-9; 
  
    cout << "Time taken by program is : " << fixed  
         << time_taken << setprecision(9); 
    cout << " sec" << endl; 
    return 0; 
}