
// graph.cpp //

#include "graph.h"


#include <iostream>
#include <stdlib.h>
#include <set>
#include <string>

using namespace std;




//////////
// EDGE //
//////////

edge::edge() {
    points[0] = NULL ;
    points[1] = NULL ;
}

void edge::assignPoints( std::shared_ptr<vertex> v1, std::shared_ptr<vertex> v2) {
    points[0] = v1 ;
    points[1] = v2 ;
}

void edge::print() const {
    points[0]->print() ;
    cout << " --- " ;
    points[1]->print() ;
}






////////////
// Vertex //
////////////

// O( v::degree )
std::shared_ptr<edge> vertex::connectsTo( const std::shared_ptr<vertex> & v ) const {
    for ( auto e : branches )
        if ( v == e->points[0] || v == e->points[1] )
            return e ;
    return NULL ;
}

void vertex::print() const {
    cout << "("<<name<<":"<<this<<", "<< ((side==left)?"L":"R") <<")" ;
}








///////////
// GRAPH //
///////////

// O( n )
bool Graph::hasVertex( const std::shared_ptr<vertex> & check ) const {
    return ( V.find(check) != V.end() ) ;
}


void Graph::print() const {
    cout << "graph here\n" ;
}



// if two verticies have a connection in their branches, the edge is added to E
// O( m·logm )
void Graph::updateEdges() {
    E.clear();
    set< std::shared_ptr<edge> > temp_edges ;

    // for all verticies
    for ( auto v : V )
        // go through each branch
        for ( auto e : v->branches ) {
            std::shared_ptr<edge> edge_pointer = e ;
            auto edge_it = temp_edges.find( edge_pointer ) ;

            if ( edge_it == temp_edges.end() ) { // if other vertex not already added, add to temp_edges
                temp_edges.insert( edge_pointer ) ;
            } else {                            // if found, erase it and add it to E
                temp_edges.erase( edge_it );
                E.push_back( edge_pointer );
            }

        }
    

}









///////////////////////
// UnitIntervalGraph //
///////////////////////

UnitIntervalGraph::UnitIntervalGraph() {
    if ( MC.size() != 0 ) return ;
    vector<u2> sizes , connections ;

    u2 k = 3 + (rand() % 7) ;

    while ( sizes.size() < k )
        sizes.push_back( 1 + (rand() % 7) ) ;

    if ( k == 0 ) throw BadConnectionsFormat() ;
    if ( k >= 1 ) connections.push_back(1) ;
    if ( k >= 2) connections.push_back(1) ;
    while ( connections.size() < k )
        connections.insert( connections.begin()+1, 2 ) ;

    // switch ( k ) {
    //     case (0) : throw BadConnectionsFormat() ;
    //     case (3) : connections.push_back(2) ;
    //     case (2) : connections.insert(connections.begin(),1) ;
    //     default  : connections.push_back(1) ; break ;
    // } while ( connections.size() < k ) { // case k > 3
    //     if      (   connections.size() == 0
    //              || connections.size() == k-1 ) connections.push_back(1) ;
    //     else if (   connections.size() == 1
    //              || connections.size() == k-2 ) connections.push_back(2) ;

    //     else if ( connections.back() == 1 )
    //         connections.push_back( 2 ) ;
    //     else if ( connections.back() == k+1-connections.size() )
    //         connections.push_back( connections.back()-1 ) ;
        
    //     else if ( connections.back() == k-connections.size() )
    //         connections.push_back( connections.back()-1 + (rand()%2) ) ;

    //     else
    //         connections.push_back( connections.back()-1 + (rand()%3) ) ;
    // }

    fill_MC_and_S( sizes, connections ) ;
}
UnitIntervalGraph::UnitIntervalGraph( const vector<u2> & sizes, const vector<u2> & connections ) {
    fill_MC_and_S( sizes, connections ) ;
}


void UnitIntervalGraph::fill_MC_and_S( const vector<u2> & sizes, const vector<u2> & connections ) {
    if ( sizes.size() != connections.size() ) throw MismatchedParameters() ;
    if ( sizes.size() == 0 ) return ;

    if (   ( connections.at(0)  != 1 )
        || ( connections.back() != 1 )
    ) throw BadConnectionsFormat() ;

    // for ( u2 s = 0; s < S.size(); s++ ) delete S.at(s);
    // for ( u2 mc = 0; mc < MC.size(); mc++ ) delete MC.at(mc);
    // MC.clear();
    // S.clear();

    // add the inputed number of sects
    for ( u2 s = 0 ; s < sizes.size() ; s++ ) {
        S.emplace_back();

        // while the new sect's size is less than the inputed size
        while ( S.at(s)->V.size() < sizes.at(s) ) {
            
            // add vertex
            auto next_vertex = make_shared<vertex>();
            V.insert( next_vertex ) ;

            // add it to sect and set its branches
            S.at(s)->addVertex( next_vertex ) ;
        }

    }


    MC.emplace_back();
    S.at(0)->addToMaximalClique( MC.back() ) ;


    for ( u2 s = 1 ; s < connections.size() ; s++ ) {
        if ( connections.at(s) == 0 ) throw BadConnectionsFormat() ;

        // mc starts as the first MaximalCLique containing S.at(s-1)
        u2 mc = 0 ;
        while ( !MC.at(mc)->contains( S.at(s-1) ) ) mc++ ;

        char delta = connections.at(s) - connections.at(s-1) ;

        switch ( delta ) {
            case  1 : // add first mc
                S.at(s)->addToMaximalClique( MC.at(mc) ) ;
            case  0 : // push new mc
                MC.emplace_back();
            case -1 : // add remaining 
                mc++;
                while ( mc < MC.size() ) {
                    S.at(s)->addToMaximalClique( MC.at(mc) ) ;
                    mc++ ;
                }
                break;
            default: throw BadConnectionsFormat() ;
        }

    }
    cout << "sect size: ";
    for ( auto s : S ) cout << s->n() << " "; cout << endl;
    cout << "connects : ";
    for ( auto s : S ) cout << s->numConnections() << " "; cout << endl;

    updateEdges() ;


}

string line( const u2 & , const u2 & ) ;

void UnitIntervalGraph::print() const {
    cout << "                        Sect sizes :   " ;
    for ( u2 s = 0 ; s < S.size() ; s++ )
        cout << S.at(s)->n() << " " ; cout << "\n\n" ;

    cout << "Number of MCs Each Sect Belongs To :   ";
    for ( u2 s = 0 ; s < S.size() ; s++ )
        cout << S.at(s)->numConnections() << " " ; cout << endl ;
    
    string next_line = line( 0 , MC.at(0)->S.size() ) ;
    while ( next_line.length() < 2*S.size()+2 ) next_line += " " ;

    cout << "Number of Sects in Each Mxml Clq :   "
         << next_line << MC.at(0)->n() 
         << endl ;
    for ( u2 mc = 1, s = 0 ; mc < MC.size() ; mc++ ) {
        while ( !S.at(s)->isMemberOf( MC.at(mc) )  ) s++ ;

        next_line = line( s , MC.at(mc)->S.size() ) ;
        while ( next_line.length() < 2*S.size()+2 ) next_line += " " ;

        cout << "                                     "
             << next_line << MC.at(mc)->n() 
             << endl ;        
    }
    for ( u4 i = 0 ; i < k() ; i++ )
        cout << 
        S.at(i)->n() * ( S.at(i)->numBranches() - S.at(i)->n() + 1 )
        << " " ; cout << endl ;
    for ( u4 i = 0 ; i < k() ; i++ )
        cout << 
        ( S.at(i)->n() * ( S.at(i)->n() - 1) ) / 2
        << " " ; cout << endl ;
    for ( u4 i = 0 ; i < k() ; i++ )
        cout << 
        ( S.at(i)->n() * ( S.at(i)->numBranches() - S.at(i)->n() + 1 ) ) +
        ( ( S.at(i)->n() * ( S.at(i)->n() - 1) ) / 2 )
        << " " ; cout << endl ;
}

string line( const u2 & s , const u2 & mc_size ) {
    string out ;
    u2 length = 2*s ;
    while ( out.length() < length ) out += " " ;
    length += 2*mc_size - 1 ;
    while ( out.length() < length ) out += (out.length() % 2)? "-" : "o" ;
    return (char)(mc_size+'0') + (" " + out) ;
}









////////////
// CLIQUE //
////////////

// O( C::n )
void Clique::addVertex( shared_ptr<vertex> & next ) {
    if ( hasVertex(next) ) return ;


    // for every vertex in the clique
    for ( auto v : V ) {
        shared_ptr<edge> new_edge = next->connectsTo( v ) ;
        // if a connection does not exist
        if ( !new_edge ) {
            // make new edge
            new_edge = make_shared<edge>() ;
            new_edge->assignPoints( v, next ) ;
 
            // add it to the vertex branches
            v->addBranch( new_edge ) ;
            next->addBranch( new_edge ) ;
        }
        // add edge to E
        E.push_back( new_edge ) ;
    }
    // add vertex to V
    V.insert( next ) ;
 

}











///////////////////
// MAXIMALCLIQUE //
///////////////////

// O( S::n )
void MaximalClique::addSect( std::shared_ptr<Sect> & sect ) {
    // add all verticies in sect to V
    for ( auto v : sect->V )
        addVertex( v ) ;
    // add this to sect.MC;
    auto THIS = getThis();
    if ( !sect->isMemberOf( THIS ) )
        sect->MC.insert( THIS );
    // add sect to S
    if ( !contains( sect ) )
        S.insert( sect ) ;
}

// O( log( MC::numSects ) )
bool MaximalClique::contains( std::shared_ptr<Sect> & sect ) const { 
    return ( S.find( sect ) != S.end() ) ;
}










//////////
// SECT //
//////////

// O( S::n )
void Sect::addToMaximalClique( std::shared_ptr<MaximalClique> & mc) {
    auto THIS = getThis();
    mc->addSect( THIS ) ;
}

// O( log( S::numConnections ) )
bool Sect::isMemberOf( std::shared_ptr<MaximalClique> & mc ) const {
    return ( MC.find( mc ) != MC.cend() ) ;
}
