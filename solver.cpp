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
        clauseSet clauseset;
    public: 
        solver(int variables,int clauses,clauseSet cs):totalClauses(clauses),totalVariables(variables),clauseset(cs){}
        bool dpll(vector<int>unitClauses,vector<int>countClause,vector<bool>satClause,unordered_set<int>assigned,unordered_set<int>done,int satisfiedClauses){ 
                //unit propogation 
            unordered_set<int>visited;            
            for(int i=0;i<unitClauses.size();i++){
                int unitLiteral=unitClauses[i];
                if(visited.find(unitLiteral)!=visited.end())continue;
                else visited.insert(unitLiteral);
                //cout<<"???"<<unitLiteral<<endl;
                assigned.insert(unitLiteral);
                // for(auto l:assigned)
                //     cout<<l<<" ";
                // cout<<endl;
                done.erase(getvariable(unitLiteral));
                // done.erase(complement(unitLiteral));
                auto it = clauseset.literalMap.equal_range(unitLiteral); 
                for(auto clauseNum=it.first;clauseNum!=it.second;++clauseNum){
                    //cout<<satClause[clauseNum->second]<<" **"<<clauseNum->second<<endl;
                    if(!satClause[clauseNum->second]){
                        satClause[clauseNum->second]=true;
                        // cout<<clauseNum->second<<" true"<<endl;
                        if(clauseNum->second <= totalClauses)
                            satisfiedClauses++;
                        //cout<<satisfiedClauses<<" "<<totalClauses<<" "<<assigned.size()<<"??"<<endl;
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
                    countClause[clauseNum->second]--;
                    if(countClause[clauseNum->second]==1 && satClause[clauseNum->second]==false){
                        for(auto lit: clauseset.clauses[clauseNum->second].literals){
                            if(assigned.find(lit)==assigned.end() && assigned.find(complement(lit))==assigned.end()){
                                unitClauses.push_back(lit);
                                //cout<<"???%%"<<lit<<endl;
                                // cout<<unitClauses.size()<<"$$"<<endl;
                            }
                        }
                    }
                    else if(countClause[clauseNum->second]==0 && satClause[clauseNum->second]==false){
                        //cout<<clauseNum->second<<" "<<"UNSATISFIABLE"<<endl;
                        return false;            
                    }
                }     
            }       
            unitClauses.clear();    
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
            unitClauses.push_back(selectedLiteral);
            if(dpll(unitClauses,countClause,satClause,assigned,done,satisfiedClauses))
                return true;
            unitClauses.pop_back();
            unitClauses.push_back(complement(selectedLiteral));
            return dpll(unitClauses,countClause,satClause,assigned,done,satisfiedClauses);            
        }
    // assignment.reserve(2*(numLiterals+5));
};
int main(){
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
    //cout<<clauses.clauses.size()<<"???"<<endl;
    solver dpllsolver(n,m,clauses);
    cout<<dpllsolver.dpll(clauses.unitClauses,clauses.countClause,clauses.satClause,assigned,done,0)<<endl;
    for(auto k:finalAssignment)
        cout<<k<<" ";
    #ifdef DEBUG
    for(auto k:clauses.unitClauses)
        cout<<k<<" ";
    cout<<endl;
    clauses.print();
    clauses.printLiteral(6);
    #endif
    return 0; 
}
/*
c A SAT instance generated from a 3-CNF formula that had 40 clauses and 6 variables
p cnf 6 30
4 -5 3 0
-2 -3 5 0
-6 3 -5 0
3 -1 -4 0
-1 4 5 0
4 -1 2 0
-5 -4 1 0
6 4 -1 0
3 4 -1 0
6 4 3 0
2 -1 -4 0
-1 -5 4 0
-6 4 -2 0
-4 3 -6 0
-6 5 -3 0
6 3 4 0
-5 -6 -4 0
-5 -3 4 0
6 -1 3 0
-6 -3 -4 0
-2 6 1 0
-5 -6 -4 0
-6 2 4 0
2 -4 6 0
2 -3 -4 0
4 6 -2 0
6 -3 -5 0
-5 2 -3 0
5 -6 -4 0
-4 -1 3 0

*/