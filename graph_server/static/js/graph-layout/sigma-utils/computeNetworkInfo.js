sigma.utils.computeNetworkInfo
    = function( nodes, edges, options ) {

    var nodesIndex = {}
        , edgesIndex = {}
        , inNeighborsIndex = {}
        , outNeighborsIndex = {}
        , allNeighborsIndex = {}
        , inNeighborsCount = {}
        , outNeighborsCount = {}
        , allNeighborsCount = {}
        ;

    nodes.forEach( function( node ) {
        var id = node.id;
        nodesIndex[ id ] = node;

        inNeighborsIndex[ id ] = {}; 
        outNeighborsIndex[ id ] = {}; 
        allNeighborsIndex[ id ] = {}; 
        inNeighborsCount[ id ] = 0; 
        outNeighborsCount[ id ] = 0; 
        allNeighborsCount[ id ] = 0; 
    } );

    edges.forEach( function( edge ) {
        var id = edge.id;
        edgesIndex[ id ] = edge;

        inNeighborsIndex[ edge.target ][ edge.source ] =
            inNeighborsIndex[ edge.target ][ edge.source ] || {};
        inNeighborsIndex[ edge.target ][ edge.source ][ id ] = edge;

        outNeighborsIndex[ edge.source ][ edge.target ] =
            outNeighborsIndex[ edge.source ][ edge.target ] || {};
        outNeighborsIndex[ edge.source ][ edge.target ][ id ] = edge;

        allNeighborsIndex[ edge.target ][ edge.source ] =
            allNeighborsIndex[ edge.target ][ edge.source ] || {};
        allNeighborsIndex[ edge.target ][ edge.source ][ id ] = edge;

        allNeighborsIndex[ edge.source ][ edge.target ] =
            allNeighborsIndex[ edge.source ][ edge.target ] || {};
        allNeighborsIndex[ edge.source ][ edge.target ][ id ] = edge;

        inNeighborsCount[ edge.target ]++;
        outNeighborsCount[ edge.source ]++;
        allNeighborsCount[ edge.target ]++;
        allNeighborsCount[ edge.source ]++;
    } );

    return {
        nodes: function( nodeId ) {
            if ( typeof nodeId !== undefined ) {
                return nodesIndex[ nodeId ];
            }
            else {
                return nodes;
            }
        }
        , edges: function( edgeId ) {
            if ( typeof edgeId !== undefined ) {
                return edgesIndex[ edgeId ];
            }
            else {
                return edges;
            }
        }
        , degree: function( nodeId ) {
            return allNeighborsCount[ nodeId ];
        }
        , allNeighborsIndex: function( nodeId ) {
            var indices = [];
            for ( var key in allNeighborsIndex[ nodeId ] ) {
                indices.push( key );
            }
            return indices;
        }
        , allNeighbors: function( nodeId ) {
            var me = this
                , indices = me.allNeighborsIndex( nodeId )
                , neighbors = []
                ;
            indices.forEach( function( index ) {
                neighbors.push( me.nodes( index ) );
            } );
            return neighbors;
        }
    };
};  
