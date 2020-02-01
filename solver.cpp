#include<bits/stdc++.h>
//#define DEBUG
using namespace std;
void here(){
    cout<<"here"<<endl;
}
class clause{
    public:
        bool sat=false,tautology=false;
        unordered_set<int>literals;  
        clause(){}
        clause(int unitLiteral){
            literals.insert(unitLiteral);
        }
        inline void addLiteral(int x){
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
        multimap<int,int> literalMap;
        vector <int> unitClauses;

        clauseSet(int numliteral,int numclause):numClauses(numclause),numLiterals(numliteral){      
            clauses.reserve(numClauses+5);
            visited.reserve(2*(numLiterals)+5);
            clauses.push_back(clause(0));
        }
        void addClause(clause cs,bool set=true){
            if(cs.tautology)
                return;
            else{
                clauses.push_back(cs); // 1 based indexing for literal map
                if(cs.literals.size() == 1){
                    unitClauses.push_back(clauses.size()-1);
                }
                if(set){
                    for(auto lit:cs.literals){
                        literalMap.insert(make_pair(lit,clauses.size()-1));
                    }
                }
            }
        }
        void removeLastAddedUnitClause(){
            clause remclause=clauses.back();
            // int literal=*(remclause.literals.begin());
            // auto iterpair = literalMap.equal_range(literal);
            // auto it = iterpair.first;
            // for (; it != iterpair.second; ++it) {
            //    if (it->second == literal) { 
            //         literalMap.erase(it);
            //         break;
            //     }
            // }
            unitClauses.pop_back();
            clauses.pop_back();
        }
        void pureLiteralElim(){
            for(int i=1;i<2*numLiterals;i+=2){
                if(literalMap.count(i) && literalMap.count(i+1)==0){
                    addClause(clause(i));
                }
                else if(literalMap.count(i+1) && literalMap.count(i)==0){
                    addClause(clause(i+1));
                }
            }
        }
        void print(){
            for(auto k:clauses){
                cout<<k<<endl;
            }
        }
        void printLiteral(int lit){
            auto it = literalMap.equal_range(lit); 
            for(auto itr=it.first;itr!=it.second;++itr)
                cout<<itr->first<<" "<<itr->second<<endl;;
            cout<<endl;
        }
};
int complement(int i){
    if(i&1)
        return i+1;
    else
        return i-1;
}
int getvariable(int i){
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
                //clauseset.print();    
            for(int i=0;i<clauseset.unitClauses.size();i++){
                auto unitClause=clauseset.unitClauses[i];
                int unitLiteral=*((clauseset.clauses[unitClause].literals).begin());
                //cout<<unitLiteral<<"$$$"<<endl;
                assigned.insert(unitLiteral);
                done.erase(getvariable(unitLiteral));
                auto it = clauseset.literalMap.equal_range(unitLiteral); 
                for(auto clauseNum=it.first;clauseNum!=it.second;++clauseNum){
                    if(!clauseset.clauses[clauseNum->second].sat){
                        clauseset.clauses[clauseNum->second].sat=true;
                        if(clauseNum->second <= totalClauses)
                            satisfiedClauses++;
                        //cout<<satisfiedClauses<<" "<<totalClauses<<" "<<assigned.size()<<endl;
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
                    clauseset.clauses[clauseNum->second].literals.erase(compUnitLiteral);
                    if(clauseset.clauses[clauseNum->second].literals.size()==1 && clauseset.clauses[clauseNum->second].sat==false)
                        clauseset.unitClauses.push_back(clauseNum->second);
                    else if(clauseset.clauses[clauseNum->second].literals.size()==0){
                        //cout<<"UNSATISFIABLE"<<endl;
                        return false;            
                    }
                }     
            }           
            //clauseset.print();
            // chose pivot element implement heuristic here
            int pivot = *(done.begin());
            int selectedLiteral = 2*pivot;
            //cout<<pivot<<"??"<<endl;
            clauseset.addClause(clause(selectedLiteral),false);           
            if(dpll(clauseset,assigned,done,satisfiedClauses))
                return true;
            //here();
            clauseset.removeLastAddedUnitClause();
            clauseset.addClause(clause(complement(selectedLiteral)),false);
            return dpll(clauseset,assigned,done,satisfiedClauses);            
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
    for(int i=1;i<=n;i++)
        done.insert(i);
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
    clauses.pureLiteralElim();
    solver dpllsolver(n,m);
    vector<bool>assignment;
    assignment.reserve(2*n+5);
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

    return 0;
}