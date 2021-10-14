sigma.utils.getGraphFromTree
        = function( tree, allEdges ) {
        var nodes = []
            , nodesHash = {}
            , edges = []
            ;

        allEdges = allEdges || [];
        depthTravel( tree );
        allEdges.forEach( function( edge ) {
            if ( nodesHash[ edge.source ]
                || nodesHash[ edge.target ] ) {
                edges.push( edge );
            }
        } );
        return {
            nodes: nodes
            , edges: edges
        };

        function depthTravel( node ) {
            nodes.push( node );
            nodesHash[ node.id ] = 1;
            if ( node._wt_children ) {
                node._wt_children.forEach( function( child ) {
                    depthTravel( child );
                });
            }
        }
    };
