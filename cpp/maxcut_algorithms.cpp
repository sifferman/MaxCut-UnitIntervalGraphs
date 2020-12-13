
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

    for ( u2 tc = 0; tc < k(); tc++ ) {
        u2 i = 0;
        for ( auto v : TC.at(tc)->V )
            v->side = (vertex::Side)( i < cut.at(tc) );
    }
    
    return this->cut();
}




///////////////////////
// Graph Brute Force //
///////////////////////

// Brute force Max-Cut algorithm. Time complexity: O*( 2^v ).
u2 Graph::BF_maxcut() const {
    if ( V.size() > 22 ) throw GraphTooLarge() ;

    u4 max = 0 ;
    u2 cut = 0 ;
    size_t arrangement = 0 ;

    u2 v_i;


    // for each vertex arrangement, arrange each vertex then store the cut-value if it is bigger than max
    v_i = 0;
    for ( size_t i = 0; i < pow( 2, V.size() ); i++ ) {
        for ( auto v : V ) {
            v->side = (vertex::Side) (( i >> v_i ) & 1) ;
            v_i++;
        }
        cut = this->cut();
        if ( cut > max ) { max = cut ; arrangement = i ; }
    }

    // revert to maximal arrangement
    v_i = 0;
    for ( auto v : V ) {
        v->side = (vertex::Side) (( arrangement >> v_i ) & 1) ;
        v_i++;
    }

    // return max
    return max;
}




// Initialize Maximal Cliques.
void UnitIntervalGraph::makeMaximalCliques() const {
    // for each V[j], make maximal clique
    // push if it does not already exist
}


// Initialize Sects.
void UnitIntervalGraph::makeTwinClasses() const {
    makeMaximalCliques() ;
    // for each C[j], remove all verticies in another clique
    // push
}






// Max-Cut algorithm. Time complexity: O*( (s+1)^k ).
u2 UnitIntervalGraph::ES_maxcut() const {
    makeTwinClasses() ;
    const u2 num_twinclasses = TC.size() ;

    // MAX_ASSIGNMENT is the number of comparisons to make
    size_t MAX_ASSIGNMENT = 1 ;
    for ( unsigned short tc = 0; tc < num_twinclasses; tc++ ) MAX_ASSIGNMENT *= TC.at(tc)->n()+1 ;

    // cout << "Number of Comparisons to Make: " << MAX_ASSIGNMENT << endl ;

    if ( MAX_ASSIGNMENT >= pow(2,22) ) throw GraphTooLarge() ;

    vector<size_t> solutions ;
    u4 max = 0 ;
    
    u2 cuts[num_twinclasses] ;


    size_t assignment ;
    size_t best_assignment ;
    for ( assignment = 0; assignment < MAX_ASSIGNMENT; assignment++ ) {
        // according to assignment, arrange G into L and R
        size_t each_assignment = assignment ;
        for ( u2 tc = 0; tc < num_twinclasses; tc++ ) {
            cuts[tc] = each_assignment % (TC.at(tc)->n()+1) ;
            each_assignment /= TC.at(tc)->n()+1 ;
        }
    for ( u2 tc = 0; tc < k(); tc++ ) {
        u2 i = 0;
        for ( auto v : TC.at(tc)->V )
            v->side = (vertex::Side)( i < cuts[tc] );
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
    for ( u2 tc = 0; tc < num_twinclasses; tc++ ) {
        cuts[tc] = best_assignment % (TC.at(tc)->n()+1) ;
        best_assignment /= TC.at(tc)->n()+1 ;
    }
    for ( u2 tc = 0; tc < k(); tc++ ) {
        u2 i = 0;
        for ( auto v : TC.at(tc)->V )
            v->side = (vertex::Side)( i < cuts[tc] );
    }

    print() ; cout << endl ;
    cout << "Printing all solutions: \n";
    for ( size_t i = 0 ; i < solutions.size() ; i++ ) {
        size_t each_assignment = solutions.at( i ) ;
        for ( u2 tc = 0; tc < num_twinclasses; tc++ ) {
            cout << ( each_assignment % (TC.at(tc)->n()+1) ) << " " ;
            each_assignment /= TC.at(tc)->n()+1 ;
        }
        cout << endl ;
    }
    cout << endl << max << "\n\n";
    
    return max;
}


// // Every-Other 1/2 Appropriation Algorithm for sorted Unit Interval Graphs
// O( v )
u2 UnitIntervalGraph::EO_maxcut() const {
    u1 A = 0;
    for ( auto v : V ) {
        v->side = (vertex::Side) A;
        A = ~A;
    }
    return cut();
}
