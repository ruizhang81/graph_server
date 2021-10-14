sigma.classes.graph.addMethod( 'allNeighborNodes', function( node ) {
    var me = this
        , allNeighborIds = me.allNeighborIds( node )
        , allNeighbors = []
        ;

    allNeighborIds.forEach( function( nodeId ) {
        allNeighbors.push( me.nodes( nodeId ) );
    } );
    return allNeighbors;
} );
