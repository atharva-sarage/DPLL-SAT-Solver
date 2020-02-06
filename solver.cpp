#include<bits/stdc++.h>
//#define DEBUG
# define IOS ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
using namespace std;
vector<bool>finalAssignment;
int totalClauses,totalVariables;
class clause{
    private:
        bool tautology=false;
        int totalLiterals=0;
    public:
        vector<int>literals;
        clause(){}
        clause(int unitLiteral){
            literals.emplace_back(unitLiteral);
        }
        inline void addLiteral(int x){
            totalLiterals++;
            if(x<0){
                literals.emplace_back(-2*x-1);
                tautology|=(find(literals.begin(),literals.end(),2*x)!=literals.end());
            }
            else{
                literals.emplace_back(2*x);
                tautology|=(find(literals.begin(),literals.end(),-2*x-1)!=literals.end());
            }
        }
        int getCountLiteralsInClause(){
            return totalLiterals;
        }
        bool isTautology(){
            return tautology;
        }
        friend ostream& operator<<(ostream& os, const clause& cs){
            for(auto lit:cs.literals)
                os<<lit<<" "; 
            return os;
        }
};
class clauseCurrentState{
    public:
        vector <int> countClause;
        vector <bool> isSatisfied;
        vector <int> unitClauses;
    clauseCurrentState(){
      init();
    }
    clauseCurrentState(const clauseCurrentState& state){
        init();
        countClause=state.countClause;
        isSatisfied=state.isSatisfied;
        unitClauses=state.unitClauses;
    }
    void init(){
        countClause.reserve(totalClauses+5);
        isSatisfied.reserve(totalClauses+5);          
        isSatisfied.emplace_back(false);
        countClause.emplace_back(0);
    }
    clauseCurrentState* clone(){
        return new clauseCurrentState(*this);
    }
};
class clauseSet{
    public:
        vector <clause> clauses;
        vector<vector<int>*> literalClauseMap;         
        clauseCurrentState state;
        clauseSet(){                 
            literalClauseMap.reserve(2* totalVariables+5);
            for(int i=0;i<2*totalVariables+5;i++)
                literalClauseMap[i]= new vector<int>();           
            clauses.emplace_back(clause(0));            
        }
        void addClause(clause cs){
            if(cs.isTautology())
                return;
            else{
                clauses.emplace_back(cs);
                state.isSatisfied.emplace_back(false);
                state.countClause.emplace_back(cs.getCountLiteralsInClause());
                if(cs.literals.size() == 1)
                    state.unitClauses.emplace_back(cs.literals.front());
                for(auto lit:cs.literals){
                    literalClauseMap[lit]->emplace_back(state.countClause.size()-1);
                }
            }
        }
        void pureLiteralElim(){
            for(int i=1;i<2*totalVariables;i+=2){
                if(literalClauseMap[i]->size() && literalClauseMap[i+1]->size()==0){
                    state.unitClauses.emplace_back(i);
                }
                else if(literalClauseMap[i+1]->size() && literalClauseMap[i]->size()==0){
                    state.unitClauses.emplace_back(i+1);
                }
            }
        }
        void printLiteral(int lit){
            for(auto k:(*literalClauseMap[lit]))
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
class solver{
    private:        
        clauseSet* clauseset;
    public: 
        solver(clauseSet* cs):clauseset(cs){}
        bool dpll(clauseCurrentState& state,vector<bool>&assigned,int satisfiedClauses){ 
                //unit propogation 
            bool visited[2*totalVariables+5]={false};  // memory opt here
            for(int i=0;i<state.unitClauses.size();i++){
                int unitLiteral=state.unitClauses[i];
                if(visited[unitLiteral])
                    continue;
                visited[unitLiteral]=true;
                assigned[unitLiteral]=true;              
                if(clauseset->literalClauseMap[unitLiteral]->size()>0){

                    for(auto clauseNum:*(clauseset->literalClauseMap[unitLiteral])){
                        if(!state.isSatisfied[clauseNum]){
                            state.isSatisfied[clauseNum]=true;
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
                if(clauseset->literalClauseMap[compUnitLiteral]->size()>0){                         
                    for(auto clauseNum:*(clauseset->literalClauseMap[compUnitLiteral])){
                        state.countClause[clauseNum]--;
                        if(state.countClause[clauseNum]==1 && state.isSatisfied[clauseNum]==false){
                            for(auto lit: clauseset->clauses[clauseNum].literals){
                                if(!assigned[lit] && !assigned[complement(lit)]){
                                    state.unitClauses.emplace_back(lit);
                                }
                            }
                        }
                        else if(state.countClause[clauseNum]==0 && state.isSatisfied[clauseNum]==false){
                            return false;                                
                        }
                    }   
                }  
            }       
            state.unitClauses.clear();    
            int temp=0;
            int selectedLiteral;
          
            //DLCS Heuristic
            pair<int,int>selectedVariable={0,0};
            for(int i=1;i<=2*totalVariables;i+=2){
                if(!assigned[i] && !assigned[i+1]){
                    selectedVariable=max(selectedVariable,{clauseset->literalClauseMap[i]->size()+clauseset->literalClauseMap[i+1]->size(),i});
                }
            }
            selectedLiteral=selectedVariable.second;
            int random=rand()%2;
            if(random%2==0)
                selectedLiteral=complement(selectedLiteral);
            state.unitClauses.emplace_back(selectedLiteral);
            clauseCurrentState prototypeState=clauseCurrentState(state);
            vector<bool>assigned2=assigned;
            if(dpll(prototypeState,assigned2,satisfiedClauses))
                return true;
            state.unitClauses.pop_back();
            state.unitClauses.emplace_back(complement(selectedLiteral));
            return dpll(state,assigned,satisfiedClauses);            
        }
};
int main(){
    IOS;
    srand(time(NULL));
    string strOneLine,str;
    int inp,clauseId=0;
    char ch; 
    cin>>ch;
    while (ch=='c'){
        getline(cin,strOneLine);
        cin>>ch;
    }
    cin>>str>>totalVariables>>totalClauses;
    clauseSet clauses;
    vector<bool>assigned(2*totalVariables+5);
    vector<int>input;
    while(cin>>inp){
        if(inp==0){
            clause cl;
            for(auto literal:input)
                cl.addLiteral(literal);
            clauses.addClause(cl);
            input.clear();
        }else{
            input.emplace_back(inp);
        }    
    }
    clauses.pureLiteralElim();
    solver dpllsolver(&clauses);
    int ret=dpllsolver.dpll(clauses.state,assigned,0);
    cout<<ret<<endl;
    // if(!ret)
    //     cout<<"UNSAT\n";
    // else{
    //     cout<<"SAT\n";
    //     for(int i=1;i<=totalVariables*2;i+=2){
    //         if(finalAssignment[i])
    //             cout<<-1*getvariable(i)<<" ";              
    //         else
    //             cout<<getvariable(i)<<" ";     
    //     }
    //     cout<<"\n";        
    // }
    return 0; 
}