sigma.classes.graph.addMethod( 'allNeighborIds', function( node ) {
    var nodeId = node
        , neighborsId = []
        ;
    if ( typeof node != 'string' ) {
        nodeId = nodeId.id;
    }

    for ( var key in this.allNeighborsIndex[ nodeId ] ) {
        neighborsId.push( key );
    } 
    return neighborsId;
} );  
