/******************************************
*    AUTHOR:         Atharva Sarage       *
*    INSTITUITION:   IIT HYDERABAD        *
******************************************/
#include<bits/stdc++.h>
using namespace std;
// Global Variables for storing finalAssignment and totalNumberofClauses and Variables
vector<bool>finalAssignment;
int totalClauses,totalVariables; // These are initialized in main()
/**
 * Class for representing a Clause in the input
 */
class clause{
    private:
        bool tautology=false; // This is true when a clause contains a literal and its negation.
        int totalLiterals=0;  // Count of total number of literals in the clause
    public:
        vector<int>literals;  // This vector stores all the literals 
        clause(){}            // default constructor
        // Create a clause object with one literal 
        clause(int unitLiteral){
            literals.emplace_back(unitLiteral); // emplace_back is faster than push_back
        }
        /**
        * addLiteral function adds a literal to the clause
        * while adding it increments the totalLiterals
        * and also checks for its negated literal, if it
        * finds it it sets tautology to true
        * positive literal are represented by even numbers +1->2,+2->4,...
        * negative literal are represented by odd numbers -1->1,-2->3,...
        */

        inline void addLiteral(int x){
            totalLiterals++;
            if(x<0){
                // convert negative literal to corresponding odd number
                literals.emplace_back(-2*x-1);
                tautology|=(find(literals.begin(),literals.end(),2*x)!=literals.end()); // bitwise or 
            }
            else{
                // convert positive literal to corresponding even number
                literals.emplace_back(2*x);
                tautology|=(find(literals.begin(),literals.end(),-2*x-1)!=literals.end());
            }
        }
        // Returns total Literals
        int getCountLiteralsInClause(){
            return totalLiterals;
        }
        // Returns whether clause is a tautology
        bool isTautology(){
            return tautology;
        }
};
/**
 * class clauseSetCurrentState
 * It maintains state of the clauses 
 * It stores count of unsatified literals in each clause,
 * whether a clause is satisfied and 
 * stores literals of clauses that are UnitClauses.(clauses containing only one unsatisfied literal)
 * Count of each literal in clauses
 */
class clauseSetCurrentState{
    public:
        vector <int> countClause;   // stores count of unsatified literals
        vector <bool> isSatisfied;  //whether a clause is satisfied
        vector <int> unitLiterals;  // stores literals of all unitClauses for unitPropogation
        vector <int> countLiteral;  // stores count of each literal
    clauseSetCurrentState(){        // default constructor
      init();
    }
    clauseSetCurrentState(const clauseSetCurrentState& state){  // copy constructor
        countClause=state.countClause;
        isSatisfied=state.isSatisfied;
        unitLiterals=state.unitLiterals;
        countLiteral=state.countLiteral;        
    }
    void init(){
        // We need to update value at an index so we resize the vector so that we can access
        // each index (to update literal count)
        countLiteral.resize(2*totalVariables+5);
        // both isSatisfied and countClause are 1 indexed so 
        // we add value at zero index. this is never accessed                           
        isSatisfied.emplace_back(false);
        countClause.emplace_back(0);
    }
};
/**
 * Clause clauseSet
 * Stores all the clauses information
 */
class clauseSet{
    public:
        vector <clause> clauses; // vector of clause
        // for each literal we keep a pointer to a list(vector) which stores in which clauses it occurs.
        vector<vector<int>*> literalClauseMap;    
        // state stores the initial state of all the clauses
        clauseSetCurrentState state;
        clauseSet(){                 
            // clauses are also 1 indexed put a dummy clause at 0 index
            clauses.emplace_back(clause(0));     
            // push the pointer's of list to the literalClauseMap                 
            for(int i=0;i<2*totalVariables+5;i++)
                literalClauseMap.push_back(new vector<int>());           
        }
        /**
         * Add clause method add a clause to the clauseset only if it not
         * a tautology. It then updates countClause and the literalClauseMap
         */
        void addClause(clause cs){
            if(cs.isTautology())
                return;
            else{
                clauses.emplace_back(cs); // add to clauses
                state.isSatisfied.emplace_back(false); // set satisfied to false
                state.countClause.emplace_back(cs.getCountLiteralsInClause()); // add number of literals in clauses
                if(cs.literals.size() == 1) // It is a unit Clause
                    state.unitLiterals.emplace_back(cs.literals.front());
                for(auto lit:cs.literals){ // update the literal clause map
                    literalClauseMap[lit]->emplace_back(state.countClause.size()-1);
                    state.countLiteral[lit]++;
                }
            }
        }
        /**
         * Pure Literal Elimination 
         * When only one instance of a variable (negated or positive) appears we 
         * can set it to true. So we add this literal to the unitLiterals which
         * will be processed in unit propogation.
         */
        void pureLiteralElim(){
            for(int i=1;i<2*totalVariables;i+=2){
                // only one of them occurs then add the literal to unitLiterals
                if(literalClauseMap[i]->size() && literalClauseMap[i+1]->size()==0){
                    state.unitLiterals.emplace_back(i);
                }
                else if(literalClauseMap[i+1]->size() && literalClauseMap[i]->size()==0){
                    state.unitLiterals.emplace_back(i+1);
                }
            }
        }
        // helper function to print all the indices where this literal occurs
        void printLiteral(int lit){
            for(auto k:(*literalClauseMap[lit]))
                cout<<k<<" ";
            cout<<endl;
        }
};
// Given a literal return its complement literal
inline int complement(int i){
    if(i&1)
        return i+1;
    else
        return i-1;
}
// Given a literal return its variable
inline int getvariable(int i){
    return (i+1)/2;
}
/**
 * SATsolver Class
 * Stores a pointer to clauseset object to access literalMap and all the clauses  
 */
class SATsolver{
    private:        
        clauseSet* clauseset; // a pointer to clauseset object
    public: 
        SATsolver(clauseSet* cs):clauseset(cs){} // constructor which takes a pointer to clauseset
        /**
         * dpll function which takes argument. All arguments are passed as refrences except
         * satisfiedClauses which is an int
         * currentState object(count of unsatisfied literal, which clauses are yet to be satisfied),
         * Current assignment of the literals and 
         * number of satisfied clauses(for termination)   
         * returns whether clause is satisfied with current state of clauses
         * and assignment to variables      
         */
        bool dpll(clauseSetCurrentState& state,vector<bool>&assigned,int satisfiedClauses){ 

            /////////////UNIT PROPOGATION STARTS/////////////////
            
            // to keep track which unitLiterals are processed            
            vector <bool> visited(2*totalVariables+5);
            for(int i=0;i<state.unitLiterals.size();i++){
                int unitLiteral=state.unitLiterals[i];
                if(visited[unitLiteral]) // if processed then continue;
                    continue;
                visited[unitLiteral]=true; // mark visited to true
                assigned[unitLiteral]=true; // mark that literal set to true
                if(clauseset->literalClauseMap[unitLiteral]->size()>0){
                    // iterate over all clauses where this unitliteral occurs
                    for(auto clauseNum:*(clauseset->literalClauseMap[unitLiteral])){ 
                        if(!state.isSatisfied[clauseNum]){ // is clause is not satisfied yet then visit
                            state.isSatisfied[clauseNum]=true; // mark true
                            // if it was one of the original clauses which have indices from [1..totalClauses] 
                            // then increment satisfiedClauses
                            for(auto lit: clauseset->clauses[clauseNum].literals){
                                state.countLiteral[lit]--;   
                                /**
                                 * pure literal elimination
                                 * once count of literal becomes zero and its complement
                                 * count is greater than 0 and the corresponding variable 
                                 * is not assigned (both itself and its complement)
                                 * then we can add the complement literal to unit 
                                 * literals list
                                 */                           
                                if(state.countLiteral[lit]==0 && state.countLiteral[complement(lit)]>0
                                    && assigned[lit]==false && assigned[complement(lit)]==false)
                                state.unitLiterals.emplace_back(complement(lit));  
                            }   

                            if(clauseNum <= totalClauses)  
                                satisfiedClauses++;
                            // Check whether all clasuses are satisfied if yes WE ARE DONE!!!
                            if(satisfiedClauses==totalClauses){
                                finalAssignment=assigned;// store the current assignement in the global variable
                                return true;
                            }
                        }
                    }   
                }   
                // Now we look at all the clauses where complement of this literal occurs   
                int compUnitLiteral=complement(unitLiteral);  
                state.countLiteral[compUnitLiteral]=0;
                if(clauseset->literalClauseMap[compUnitLiteral]->size()>0){  
                    // iterate over all the clauses containing complement of that literal                       
                    for(auto clauseNum:*(clauseset->literalClauseMap[compUnitLiteral])){
                        if(state.isSatisfied[clauseNum])continue;
                        state.countClause[clauseNum]--; // decrease clauseCount
                        /**
                         * When size of clause goes from 2 to 1 that clause becomes a unit clause
                         * provided that clause is not already satisfied
                         * so we find the literal such that it and its complement are not assigned
                         * yet and add the literal to unitLiterals
                         */
                        if(state.countClause[clauseNum]==1){
                            for(auto lit: clauseset->clauses[clauseNum].literals){
                                if(!assigned[lit] && !assigned[complement(lit)]){
                                    state.unitLiterals.emplace_back(lit);
                                }
                            }
                        }
                        /**
                         * When the clause size becomes 0 and that clause is still unsatified then
                         * we know that this assignment will not work as we were not able to satisfy 
                         * this clause so we return false
                         */
                        else if(state.countClause[clauseNum]==0){
                            return false;                                
                        }
                    }   
                }  
            }       
            /////////////UNIT PROPOGATION COMPLETED/////////////////

            // clear the unitLiterals list as all of them been taken care of

            state.unitLiterals.clear();    
            int temp=0;
          
            ///////////// HEURISTIC START ////////////////////////////
            /**
             * Chose among the unassigned variables that occurs the most (both positive and negated literal)
             * This way we deal with a lot of clauses most of them are satisfied and others
             * reach near unit propogation.
             * This heuristic gave the best performance over other heuristics such as
             * literal satifying shortest clauses, first unasigned literal 
             * and randomly selecting literal.             * 
             */
            // bestLiteral denotes the literal corresponding to highest valueof
            // literal+complement(literal) given by heuristic and
            // bestValue is the corresponding value for that literal
            // we then flip polarity of literal with 1/2 probablity
            int bestLiteral=0,bestValue=-1;
            for(int i=1;i<2*totalVariables;i+=2){
                if(!assigned[i] && !assigned[i+1]){ // both the literals are unassigned
                    if(state.countLiteral[i]+state.countLiteral[i+1]>bestValue){
                        bestLiteral=i;bestValue=state.countLiteral[i]+state.countLiteral[i+1];
                    }
                }
            }
            int random=rand()%2;
            // randomly choose either positive or negated literal
            if(random%2==0)
                bestLiteral=complement(bestLiteral);

            ///////////// HEURISTIC ENDS ////////////////////////////

            // add the selected literal to unitLiterals
            state.unitLiterals.emplace_back(bestLiteral);
            /**
             * make a copy of current state and assigned vector   
             * all arguments are passed by refrence.
             * Doing this saves 1 copy. At each call only one copy is created
             * And this and the original are passed as refrences
             */

            // create a copy of current state
            clauseSetCurrentState duplicateState=clauseSetCurrentState(state); 
            // create a copy of assigned vector
            vector<bool>duplicateAssigned=assigned;
            // 1st DPLL call
            if(dpll(duplicateState,duplicateAssigned,satisfiedClauses))
                return true;
   
            // remove the literal that was selected earlier and instead 
            // add the negation of that literal in unitLiterals
            state.unitLiterals.pop_back();
            state.unitLiterals.emplace_back(complement(bestLiteral));
            // 2nd DPLL call
            return dpll(state,assigned,satisfiedClauses);            
        }
};
int main(){
    ios::sync_with_stdio(0);cin.tie(0);cout.tie(0); // fast IO
    srand(time(NULL)); // seed
    string strOneLine,str;
    int inp;
    char ch; 
    cin>>ch;
    // ignore lines starting with 'c'
    while (ch=='c'){
        getline(cin,strOneLine); 
        cin>>ch;
    }
    cin>>str>>totalVariables>>totalClauses;
    clauseSet clauses; // clauseset object
    vector<int>input; // stores literals
    while(cin>>inp){
        if(inp==0){
            clause cl;
            for(auto literal:input)
                cl.addLiteral(literal);
            clauses.addClause(cl); // add clause
            input.clear();
        }else{
            input.emplace_back(inp); // add literal
        }    
    }
    clauses.pureLiteralElim(); // do pure literal elimination
    SATsolver dpllsolver(&clauses); // solver object
    vector<bool>assigned(2*totalVariables+5); // assigned vector initially all false   

    // CALL TO SOLVER
    int ret=dpllsolver.dpll(clauses.state,assigned,0); 
    if(!ret)
        cout<<"UNSAT\n";
    else{
        cout<<"SAT\n";
        for(int i=1;i<=totalVariables*2;i+=2){
            if(finalAssignment[i])
                cout<<-1*getvariable(i)<<" ";              
            else
                cout<<getvariable(i)<<" ";     
        }
        cout<<"0\n";      
    }
    return 0; 
}