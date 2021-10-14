sigma.utils.getRelationInfoAtDistance
    = function( nodes, edges, from, distance, options ) {
    var opt = options || {}
        , nodesAtDistance = []
        , pathGraph = { nodes: [], edges: [] }
        , nodesHash = {}
        , edgesHash = {}
        ;

    if ( !nodes || !edges || !from || distance < 0 ) {
        throw Error( 'sigma.utils.getRelationInfoAtDistance: wrong arguments' );
    }

    sigma.utils.widthTravel(
        nodes
        , edges
        , from
        , {
            onNode: function( node ) {
                node._wt_children.forEach( function( child ) {
                    child._wt_parent = node;
                } );
            }
        }
    );

    sigma.utils.depthTravel(
        nodes
        , edges
        , from
        , {
            onNode: function( node ) {
                if ( node._wt_level == distance + 1 ) {
                    nodesAtDistance.push( node );
                }
            }
        }
    );

    nodesAtDistance.forEach( function( node ) {
        var subPathGraph = _getPathGraphToNode( node );

        subPathGraph.nodes.forEach( function( _node ) {
            if ( !nodesHash[ _node.id ] ) {
                pathGraph.nodes.push( _node );
                nodesHash[ _node.id ] = 1;
            }
        } );
        subPathGraph.edges.forEach( function( _edge ) {
            if ( !edgesHash[ _edge.id ] ) {
                pathGraph.edges.push( _edge );
                edgesHash[ _edge.id ] = 1;
            }
        } );
    } );

    return {
        nodes: nodesAtDistance
       , pathGraph: pathGraph
    };

    function _getPathGraphToNode( to ) {
        var subGraph = { nodes: [], edges: [] }
            , prevNode
            , currentNode = sigma.utils.getNodeById( nodes, to.id )
            ;
        do {
            subGraph.nodes.push( currentNode );
            if ( prevNode ) {
                subGraph.edges.push( _getEdges( currentNode, prevNode ) );
            }
            prevNode = currentNode;
            currentNode = currentNode._wt_parent;
        } while ( currentNode );

        if ( prevNode.id != from.id ) {
            throw Error( 
                'getRelationInfoAtDistance:_getPathGraphToNode: something wrong occur' 
            );
        }
        subGraph.nodes = subGraph.nodes.reverse();
        subGraph.edges = subGraph.edges.reverse();
        return subGraph;
    }

    function _getEdges( sourceNode, targetNode ) {
        var i = 0, len = edges.length, edge;
        while( i < len ) {
            edge = edges[ i ];
            if ( edge.source == sourceNode.id
                    && edge.target == targetNode.id
                || edge.target == sourceNode.id
                    && edge.source == targetNode.id ) {
                return edge;
            }
            i++;
        }
        throw Error( 
            'getRelationInfoAtDistance:_getEdges: something wrong occur' 
        );
    }
};
