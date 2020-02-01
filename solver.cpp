#include<bits/stdc++.h>
//#define DEBUG
using namespace std;
void here(){
    cout<<"here\n";
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
        void addClause(clause cs){
            if(cs.tautology)
                return;
            else{
                clauses.push_back(cs); // 1 based indexing for literal map
                if(cs.literals.size() == 1){
                    unitClauses.push_back(clauses.size()-1);
                }

                for(auto lit:cs.literals){
                    literalMap.insert(make_pair(lit,clauses.size()-1));
                }
            }
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
class solver{
    // vector <bool> assignment; 
    public: 
        void dpll(clauseSet clauseset,vector<bool>assignment){ 
            for(auto unitClause:clauseset.unitClauses){
                int unitLiteral=*((clauseset.clauses[unitClause].literals).begin());
                // cout<<unitLiteral<<clauseset.literalMap[2]->size()<<endl;
                cout<<unitLiteral<<" "<<clauseset.literalMap.count(unitLiteral)<<"???"<<endl;
                auto it = clauseset.literalMap.equal_range(unitLiteral); 
                for(auto clauseNum=it.first;clauseNum!=it.second;++clauseNum){
                    clauseset.clauses[clauseNum->second].sat=true;
                    cout<<clauseNum->second<<endl;
                }
                // here();
                int compUnitLiteral=complement(unitLiteral);
                it = clauseset.literalMap.equal_range(compUnitLiteral); 
                for(auto clauseNum=it.first;clauseNum!=it.second;++clauseNum){
                    cout<<"????????????????????\n";
                    clauseset.clauses[clauseNum->second].literals.erase(compUnitLiteral);
                    if(clauseset.clauses[clauseNum->second].literals.size()==1 && clauseset.clauses[clauseNum->second].sat==false)
                        clauseset.unitClauses.push_back(clauseNum->second);
                    else if(clauseset.clauses[clauseNum->second].literals.size()==0){
                        cout<<"UNSATISFIABLE"<<endl;
                    }
                }
            }
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
    solver dpllsolver;
    vector<bool>assignment;
    dpllsolver.dpll(clauses,assignment);
    #ifdef DEBUG
    for(auto k:clauses.unitClauses)
        cout<<k<<" ";
    cout<<endl;
    clauses.print();
    clauses.printLiteral(6);
    #endif

    return 0;
}