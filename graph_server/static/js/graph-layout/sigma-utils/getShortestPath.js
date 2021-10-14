sigma.utils.getShortestPath
    = function( nodes, edges, from, to, options ) {
    var opt = options || {}
        , forest
        , extend = sigma.utils.extend
        , prevNode
        , currentNode
        , subGraph = { nodes: [], edges: [] } 
        ;

    if ( !nodes || !edges || !from || !to ) {
        throw Error( 'sigma.utils.getShortestPath: wrong arguments' );
    }

    forest = sigma.utils.getLayoutForest( 
        nodes
        , edges
        , extend( opt, {
            root: from
            , onNode: function( node ) {
                node._wt_children.forEach( function( child ) {
                    child._wt_parent = node;
                } );
            } 
        } )
    );

    currentNode = sigma.utils.getNodeById( nodes, to.id );
    do {
        subGraph.nodes.push( currentNode );
        if ( prevNode ) {
            subGraph.edges.push( _getEdges( currentNode, prevNode ) );
        }
        prevNode = currentNode;
        currentNode = currentNode._wt_parent;
    } while ( currentNode );

    if ( prevNode.id != from.id ) {
        subGraph.nodes.length = 0;
        subGraph.edges.length = 0;
    }
    subGraph.nodes = subGraph.nodes.reverse();
    subGraph.edges = subGraph.edges.reverse();

    return subGraph;

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
        throw Error( 'getShortestPath:_getEdges: no matched edge found.' );
    }
};
