sigma.utils.computeNodesExtraInfo
    = function( nodes, edges, options ){

    if ( !nodes || !nodes.length || !edges || !edges.length ) {
        return;
    } 

    var nodeHashes = {};

    nodes.forEach( function( node ) {
        delete node._extra_info;
        nodeHashes[ node.id ] = node;
    } );

    edges.forEach( function( edge ) {
        var source = edge.source
            , target = edge.target
            , label = edge.label || ''
            , sourceNode = nodeHashes[ source ]
            , targetNode = nodeHashes[ target ]
            , _info, _labels
            ;

        // maybe nodes and edges are not matched
        if ( sourceNode) {
            _info = sourceNode._extra_info = sourceNode._extra_info || {};
            _labels = _info.edgeLabels = _info.edgeLabels || {}; 
            _labels[ target ] = label;
        }

        if ( targetNode) {
            _info = targetNode._extra_info = targetNode._extra_info || {};
            _labels = _info.edgeLabels = _info.edgeLabels || {}; 
            _labels[ source ] = label;
        }
    } );
}
