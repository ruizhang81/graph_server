sigma.utils.getCircleForest
    = function(nodes, edges, options){

    var opt = options || {} 
        ;

    if ( opt.makeMaxDegreeNodeRoot ) {
        nodes = sigma.utils.sortByNodesDegree( nodes, edges, 1 );
    }

    var circuits
        , circuit
        , tree = opt.root || nodes[0]
        , excludes
        , nodesVisited = {}
        , forest = []
        ;

    edges = edges || [];

    do {
        if ( opt.useComplicatedLoop ) {
            // todo: performance
            circuits = sigma.utils.getComplicatedLoops( nodes, edges, { root: tree } )
                        .complicated;
        }
        else {
            circuits = sigma.utils.getCircuits(nodes, edges, tree);
        }

        excludes = {};

        if(circuits.length > 0){
            circuits.sort(function(a, b){
                return b.length - a.length;
            });

            // the longest circuit
            tree._circuit = circuit = circuits[0]; 

            circuit.forEach(function(_node){
                excludes[_node.id] = 1;
            });

            circuit.forEach(function(_node){
                sigma.utils.widthTravel(
                    nodes
                    , edges
                    , _node
                    , {
                        onNode: function(__node){
                            nodesVisited[__node.id] = 1;

                            if ( 'function' == typeof opt.sortChildren ) {
                                __node._wt_children.sort( opt.sortChildren );
                            }
                            else {
                                __node._wt_children.sort( function( a, b ) {
                                    var _ea = a._extra_info
                                        , _eb = b._extra_info
                                        , _eaL, _ebL
                                        ;

                                    if ( !_ea || !_eb ) {
                                        return 0;
                                    }

                                    _eaL = _ea.edgeLabels;
                                    _ebL = _eb.edgeLabels;
                                    if ( !_eaL || !_ebL ) {
                                        return 0;
                                    }

                                    _eaL = _eaL[ __node.id ];
                                    _ebL = _ebL[ __node.id ];
                                    if ( !_eaL || !_ebL ) {
                                        return 0;
                                    }

                                    return _eaL.localeCompare( _ebL );
                                } );
                            }

                        }
                    }
                    , excludes
                );
            });
        }
        else {
            sigma.utils.widthTravel(
                nodes
                , edges
                , tree
                , {
                    onNode: function(__node){
                        nodesVisited[__node.id] = 1;

                        if ( 'function' == typeof opt.sortChildren ) {
                            __node._wt_children.sort( opt.sortChildren );
                        }
                        else {
                            __node._wt_children.sort( function( a, b ) {
                                var _ea = a._extra_info
                                    , _eb = b._extra_info
                                    , _eaL, _ebL
                                    ;

                                if ( !_ea || !_eb ) {
                                    return 0;
                                }

                                _eaL = _ea.edgeLabels;
                                _ebL = _eb.edgeLabels;
                                if ( !_eaL || !_ebL ) {
                                    return 0;
                                }

                                _eaL = _eaL[ __node.id ];
                                _ebL = _ebL[ __node.id ];
                                if ( !_eaL || !_ebL ) {
                                    return 0;
                                }

                                return _eaL.localeCompare( _ebL );
                            } );
                        }

                    }
                }
            );
        }

        forest.push(tree);

    } while((tree = hasMore()));

    function hasMore(){
        for(var i=0; i<nodes.length; i++){
            if(!nodesVisited[nodes[i].id]){
                return nodes[i];
            }
        }
        return 0;
    } 

    return forest;

};
