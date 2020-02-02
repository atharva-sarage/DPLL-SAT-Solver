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
        vector<vector<int>*> literalMap2; 
        vector <int> countClause;
        vector <bool> satClause;
        multimap<int,int> literalMap;       
        vector <int> unitClauses;

        clauseSet(int numliteral,int numclause):numClauses(numclause),numLiterals(numliteral){      
            countClause.reserve(numClauses+5);
            visited.reserve(2*(numLiterals)+5);
            literalMap2.reserve(2*(numLiterals)+5);
            for(int i=0;i<2*numLiterals+5;i++)
                literalMap2[i]= new vector<int>();
            countClause.push_back(0);
            satClause.push_back(false);
            clauses.push_back(clause(0));
            satClause.reserve(numClauses+5);          
        }
        void addClause(clause cs){
            if(cs.tautology)
                return;
            else{
                clauses.push_back(cs);
                satClause.push_back(false);
                countClause.push_back(cs.totalLiterals);
                if(cs.literals.size() == 1)
                    unitClauses.push_back(*(cs.literals.begin()));
                for(auto lit:cs.literals){
                    literalMap.insert(make_pair(lit,countClause.size()-1));             
                    literalMap2[lit]->push_back(countClause.size()-1);
                }
            }
        }
        void pureLiteralElim(){
            for(int i=1;i<2*numLiterals;i+=2){
                if(literalMap2[i]->size() && literalMap2[i+1]->size()==0){
                     unitClauses.push_back(i);
                }
                else if(literalMap2[i+1]->size() && literalMap2[i]->size()==0){
                    unitClauses.push_back(i+1);
                }
            }
        }
        void printLiteral(int lit){
            for(auto k:(*literalMap2[lit]))
                cout<<k<<" ";
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
        clauseSet* clauseset;
    public: 
        solver(int variables,int clauses,clauseSet* cs):totalClauses(clauses),totalVariables(variables),clauseset(cs){}
        bool dpll(vector<int>unitClauses,vector<int>countClause,vector<bool>satClause,unordered_set<int>assigned,unordered_set<int>done,int satisfiedClauses){ 
                //unit propogation 
            bool visited[2*totalVariables+5];
            memset(visited,false,sizeof(visited));
            for(int i=0;i<unitClauses.size();i++){
                int unitLiteral=unitClauses[i];
                if(visited[unitLiteral])
                    continue;
                visited[unitLiteral]=true;
                assigned.insert(unitLiteral);              
                done.erase(getvariable(unitLiteral));
                //clauseset->printLiteral(unitLiteral);
                //auto it = clauseset->literalMap.equal_range(unitLiteral); 
               //cout<<clauseset->literalMap2[unitLiteral].size()<<"$$$$"<<endl;
                if(clauseset->literalMap2[unitLiteral]->size()>0){
                    for(auto clauseNum:*(clauseset->literalMap2[unitLiteral])){
                        //cout<<clauseNum<<endl;
                        if(!satClause[clauseNum]){
                            satClause[clauseNum]=true;
                            if(clauseNum <= totalClauses)
                                satisfiedClauses++;
                            if(satisfiedClauses==totalClauses){
                                finalAssignment=assigned;
                                return true;
                            }
                        }
                    }   
                }           
                int compUnitLiteral=complement(unitLiteral);         
                if(clauseset->literalMap2[compUnitLiteral]->size()>0){
                    for(auto clauseNum:*(clauseset->literalMap2[compUnitLiteral])){
                        countClause[clauseNum]--;
                        if(countClause[clauseNum]==1 && satClause[clauseNum]==false){
                            for(auto lit: clauseset->clauses[clauseNum].literals){
                                if(assigned.find(lit)==assigned.end() && assigned.find(complement(lit))==assigned.end()){
                                    unitClauses.push_back(lit);
                                }
                            }
                        }
                        else if(countClause[clauseNum]==0 && satClause[clauseNum]==false){
                            return false;                                
                        }
                    }   
                }  
            }       
            unitClauses.clear();    
            int pivot = *(done.begin());
            int selectedLiteral = 2*pivot;
            unitClauses.push_back(selectedLiteral);
            if(dpll(unitClauses,countClause,satClause,assigned,done,satisfiedClauses))
                return true;
            unitClauses.pop_back();
            unitClauses.push_back(complement(selectedLiteral));
            return dpll(unitClauses,countClause,satClause,assigned,done,satisfiedClauses);            
        }
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
            clauses.addClause(cl);
            input.clear();
        }else{
            input.push_back(inp);
        }    
    }
    //clauses.printLiteral(2);
    // cout<<"?????????"<<endl;
    // vector <pair<int,int>> vec;
    // for(int i=1;i<=2*n;i++)
    //     vec.push_back({clauses.literalMap.count(i),i});    
    // sort(vec.begin(),vec.end(),greater<pair<int,int>>());
    // for(auto k:vec)
    //     done.insert(k.second);
    clauses.pureLiteralElim();
    solver dpllsolver(n,m,&clauses);
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