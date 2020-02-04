#include<bits/stdc++.h>
//#define DEBUG
# define IOS ios::sync_with_stdio(0);cin.tie(0);cout.tie(0);
using namespace std;
vector<bool>finalAssignment;
int totalClauses,totalVariables;
class clause{
    public:
        bool sat=false,tautology=false;
        int totalLiterals=0;
        vector<int>literals2;
        clause(){}
        clause(int unitLiteral){
            literals2.emplace_back(unitLiteral);
        }
        inline void addLiteral(int x){
            totalLiterals++;
            if(x<0){
                literals2.emplace_back(-2*x-1);
                tautology|=(find(literals2.begin(),literals2.end(),2*x)!=literals2.end());
            }
            else{
                literals2.emplace_back(2*x);
                tautology|=(find(literals2.begin(),literals2.end(),-2*x-1)!=literals2.end());
            }
        }
        friend ostream& operator<<(ostream& os, const clause& cs){
            for(auto lit:cs.literals2)
                os<<lit<<" "; 
            return os;
        }
};
class clauseSet{
    public:
        vector <clause> clauses;
        vector<vector<int>*> literalMap2; 
        vector <int> countClause;
        vector <bool> satClause;
        vector <int> unitClauses;

        clauseSet(){      
            countClause.reserve(totalClauses+5);
            literalMap2.reserve(2* totalVariables+5);
            for(int i=0;i<2*totalVariables+5;i++)
                literalMap2[i]= new vector<int>();
            countClause.emplace_back(0);
            satClause.emplace_back(false);
            clauses.emplace_back(clause(0));
            satClause.reserve(totalClauses+5);          
        }
        void addClause(clause cs){
            if(cs.tautology)
                return;
            else{
                clauses.emplace_back(cs);
                satClause.emplace_back(false);
                countClause.emplace_back(cs.totalLiterals);
                if(cs.literals2.size() == 1)
                    unitClauses.emplace_back(cs.literals2.front());
                for(auto lit:cs.literals2){
                    literalMap2[lit]->emplace_back(countClause.size()-1);
                }
            }
        }
        void pureLiteralElim(){
            for(int i=1;i<2*totalVariables;i+=2){
                if(literalMap2[i]->size() && literalMap2[i+1]->size()==0){
                     unitClauses.emplace_back(i);
                }
                else if(literalMap2[i+1]->size() && literalMap2[i]->size()==0){
                    unitClauses.emplace_back(i+1);
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
class solver{
    private:        
        clauseSet* clauseset;
    public: 
        solver(clauseSet* cs):clauseset(cs){}
        bool dpll(vector<int>&unitClauses,vector<int>&countClause,vector<bool>&satClause,vector<bool>&assigned,int satisfiedClauses){ 
                //unit propogation 
            bool visited[2*totalVariables+5]={false};  // memory opt here
            for(int i=0;i<unitClauses.size();i++){
                int unitLiteral=unitClauses[i];
                if(visited[unitLiteral])
                    continue;
                visited[unitLiteral]=true;
                assigned[unitLiteral]=true;;              
                if(clauseset->literalMap2[unitLiteral]->size()>0){
                    for(auto clauseNum:*(clauseset->literalMap2[unitLiteral])){
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
                            for(auto lit: clauseset->clauses[clauseNum].literals2){
                                if(!assigned[lit] && !assigned[complement(lit)]){
                                    unitClauses.emplace_back(lit);
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
            int temp=0;
            int selectedLiteral;

            // heuristic selection is giving high performance improvements
            
            //smallestClauseHeuristic
            // pair<int,int>selectedClause={1e6,0};
            // for(int i=1;i<=totalClauses;i++){
            //     if(satClause[i])
            //         continue;
            //     else
            //         selectedClause=min(selectedClause,{countClause[i],i});                
            // }
            // for(auto lit: clauseset->clauses[selectedClause.second].literals2){
            //     if(!assigned[lit] && !assigned[complement(lit)])
            //         {selectedLiteral=lit;break;}
            // }

            //DLCS Heuristic
            pair<int,int>selectedVariable={0,0};
            for(int i=1;i<=2*totalVariables;i+=2){
                if(!assigned[i] && !assigned[i+1]){
                    selectedVariable=max(selectedVariable,{clauseset->literalMap2[i]->size()+clauseset->literalMap2[i+1]->size(),i});
                    //selectedVariable=max(selectedVariable,{max(clauseset->literalMap2[i]->size(),clauseset->literalMap2[i+1]->size()),i});                    
                }
            }
            selectedLiteral=selectedVariable.second;
            int random=rand()%2;
            if(random%2==0)
                selectedLiteral=complement(selectedLiteral);
            unitClauses.emplace_back(selectedLiteral);
            vector<int>unitClauses2=unitClauses;
            vector<int>countClause2=countClause;
            vector<bool>satClause2=satClause;
            vector<bool>assigned2=assigned;
            if(dpll(unitClauses2,countClause2,satClause2,assigned2,satisfiedClauses))
                return true;
            unitClauses.pop_back();
            unitClauses.emplace_back(complement(selectedLiteral));
            return dpll(unitClauses,countClause,satClause,assigned,satisfiedClauses);            
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
    vector<bool>assigned(2*totalVariables+5);
    clauseSet clauses;
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
    int ret=dpllsolver.dpll(clauses.unitClauses,clauses.countClause,clauses.satClause,assigned,0);
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