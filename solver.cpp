#include<bits/stdc++.h>
#define DEBUG
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
        vector <unordered_set<int>* > literalMap;
        vector <int> unitClauses;

        clauseSet(int numliteral,int numclause):numClauses(numclause),numLiterals(numliteral){      
            clauses.reserve(numClauses+5);
            literalMap.reserve(2*(numLiterals+5));
            visited.reserve(2*(numLiterals)+5);
        }
        void addClause(clause cs){
            if(cs.tautology)
                return;
            else{
                clauses.push_back(cs); // 1 based indexing for literal map
                if(cs.literals.size() == 1){
                    unitClauses.push_back(clauses.size());
                }

                for(auto lit:cs.literals){
                    if(!visited[lit])
                        literalMap[lit] = new unordered_set<int>;
                    literalMap[lit]->insert(clauses.size());
                    visited[lit]=true;
                }
            }
        }
        void pureLiteralElim(){
            for(int i=1;i<2*numLiterals;i+=2){
                if(literalMap[i]!=NULL && literalMap[i+1]==NULL){
                    addClause(clause(i));
                }
                else if(literalMap[i]==NULL && literalMap[i+1]!=NULL){
                    addClause(clause(i+1));
                }
                //cout<<i<<" "<<literalMap[i]->empty()<<" "<<literalMap[i+1]->empty()<<endl;
            }
        }
        void print(){
            for(auto k:clauses){
                cout<<k<<endl;
            }
        }
        void printLiteral(int lit){
            for(auto k:(*literalMap[lit]))
                cout<<k<<" ";
            cout<<endl;
        }
};
class solver{
    vector <bool> assignment;  
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
    #ifdef DEBUG
    clauses.pureLiteralElim();
    for(auto k:clauses.unitClauses)
        cout<<k<<" ";
    cout<<endl;
   clauses.print();
    #endif

    return 0;
}