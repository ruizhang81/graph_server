sigma.utils.getNodesWithinDistance
    = function( nodes, edges, from, distance, options ) {
    var opt = options || {}
        , nodesWithinDistance = []
        ;

    if ( !nodes || !edges || !from || distance < 0 ) {
        throw Error( 'sigma.utils.getRelationInfoAtDistance: wrong arguments' );
    }

    sigma.utils.widthTravel(
        nodes
        , edges
        , from
    );

    sigma.utils.depthTravel(
        nodes
        , edges
        , from
        , {
            onNode: function( node ) {
                if ( node._wt_level <= distance + 1 ) {
                    nodesWithinDistance.push( node );
                }
            }
        }
    );

    return nodesWithinDistance;
};
