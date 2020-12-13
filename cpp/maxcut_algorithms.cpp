
// maxcut_algorithms.cpp //

#include "graph.h"

#include <math.h>
#include <iostream>

using namespace std;


u2 Graph::cut() const {
    u2 out = 0;
    for ( auto e : E )
        if ( e->points[0]->side != e->points[1]->side )
            out++ ;
    return out ;
}

u2 UnitIntervalGraph::cutArrangement( const vector<u2> & cut ) const {
    
    if ( cut.size() != k() )
        throw MismatchedParameters();

    for ( u2 s = 0; s < k(); s++ ) {
        u2 i = 0;
        for ( auto v : S.at(s)->V )
            v->side = (vertex::Side)( i < cut.at(s) );
    }
    
    return this->cut();
}




///////////////////////
// Graph Brute Force //
///////////////////////

// Brute force Max-Cut algorithm. Time complexity: O*( 2^v ).
// u2 Graph::BF_maxcut() const {
    // if ( V.size() > 22 ) throw GraphTooLarge() ;

    // u4 max = 0 ;
    // u2 cut = 0 ;
    // size_t arrangement = 0 ;


    // // for each vertex arrangement, arrange each vertex then store the cut-value if it is bigger than max
    // for ( size_t i = 0; i < pow( 2, V.size() ); i++ ) {
    //     for ( u2 v = 0; v < V.size(); v++ )
    //         V.at(v)->side = (vertex::Side) (( i >> v ) & 1) ;
    //     cut = this->cut();
    //     if ( cut > max ) { max = cut ; arrangement = i ; }
    // }

    // // revert to maximal arrangement
    // for ( u2 v = 0; v < V.size(); v++ )
    //     V.at(v)->side = (vertex::Side) (( arrangement >> v ) & 1) ;

    // // return max
    // return max;
// }





// Initialize Maximal Cliques.
void UnitIntervalGraph::makeMaximalCliques() const {
    // for each V[j], make maximal clique
    // push if it does not already exist
}


// Initialize Sects.
void UnitIntervalGraph::makeSects() const {
    makeMaximalCliques() ;
    // for each C[j], remove all verticies in another clique
    // push
}






// Max-Cut algorithm. Time complexity: O*( (s+1)^k ).
u2 UnitIntervalGraph::ES_maxcut() const {
    makeSects() ;
    const u2 num_sects = S.size() ;

    // MAX_ASSIGNMENT is the number of comparisons to make
    size_t MAX_ASSIGNMENT = 1 ;
    for ( unsigned short s = 0; s < num_sects; s++ ) MAX_ASSIGNMENT *= S.at(s)->n()+1 ;

    // cout << "Number of Comparisons to Make: " << MAX_ASSIGNMENT << endl ;

    if ( MAX_ASSIGNMENT >= pow(2,22) ) throw GraphTooLarge() ;

    vector<size_t> solutions ;
    u4 max = 0 ;
    
    u2 cuts[num_sects] ;


    size_t assignment ;
    size_t best_assignment ;
    for ( assignment = 0; assignment < MAX_ASSIGNMENT; assignment++ ) {
        // according to assignment, arrange G into L and R
        size_t each_assignment = assignment ;
        for ( u2 s = 0; s < num_sects; s++ ) {
            cuts[s] = each_assignment % (S.at(s)->n()+1) ;
            each_assignment /= S.at(s)->n()+1 ;
        }
    for ( u2 s = 0; s < k(); s++ ) {
        u2 i = 0;
        for ( auto v : S.at(s)->V )
            v->side = (vertex::Side)( i < cuts[s] );
    }
        
        // cut
        u2 cut = this->cut();
        // save the result if it is equal
        if ( cut == max ) { solutions.push_back( assignment ) ; }
        // replace the result if it is greater
        if ( cut  > max ) {
            max = cut ; best_assignment = assignment ;
            solutions.clear() ;
            solutions.push_back( assignment ) ;
        }
    }

    // according to found best_assignment, arrange G into L and R so that G can be looked at later
    for ( u2 s = 0; s < num_sects; s++ ) {
        cuts[s] = best_assignment % (S.at(s)->n()+1) ;
        best_assignment /= S.at(s)->n()+1 ;
    }
    for ( u2 s = 0; s < k(); s++ ) {
        u2 i = 0;
        for ( auto v : S.at(s)->V )
            v->side = (vertex::Side)( i < cuts[s] );
    }

    print() ; cout << endl ;
    cout << "Printing all solutions: \n";
    for ( size_t i = 0 ; i < solutions.size() ; i++ ) {
        size_t each_assignment = solutions.at( i ) ;
        for ( u2 s = 0; s < num_sects; s++ ) {
            cout << ( each_assignment % (S.at(s)->n()+1) ) << " " ;
            each_assignment /= S.at(s)->n()+1 ;
        }
        cout << endl ;
    }
    cout << endl << max << "\n\n";
    
    return max;
}


// // INCORRECT Every-Other max cut algorithm for sorted indifference graph
// // O( v )
// u2 UnitIntervalGraph::EO_maxcut() const {
//     for ( u2 v = 0; v < V.size(); v++ )
//         V.at(v)->side = (vertex::Side) (v%2) ;
//     return cut();
// }

